#include "FrVoxelSelectionCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"
#include "FrTabSettingsDocObj.h"
#include "Fr2DSliceActor.h"
#include "FrUtils.h"
#include "FrVoxelInfoWidget.h"
#include "FrLayeredImage.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"
#include "FrPointsDocObj.h"
#include "FrGraphSettings.h"

#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"

//Some defines
#define INVALIDE_RENDERER_NUM -1
#define ALL_ITEMS_COUNT 5


FrVoxelSelectionCmd::FrVoxelSelectionCmd()
: m_isMouseXY(false), m_Action(FrVoxelSelectionCmd::Undefined), m_PointPicker(0) {

}

void FrVoxelSelectionCmd::SetMouseXY(int x, int y){
    m_mouseX = x; m_mouseY = y;
    m_isMouseXY = true;
}

bool FrVoxelSelectionCmd::Execute(){
    if(!this->GetMainController() ||
        m_Action == FrVoxelSelectionCmd::Undefined ||
        !m_isMouseXY) return false;

    bool result = false;

    result = AddPoint();

    return result;
}

bool FrVoxelSelectionCmd::AddPoint(){
    // get renderer    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    std::vector<vtkRenderer*> renCollection;
    int imgNumber = -1;
    FrLayeredImage* lim = 0;

    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    Views view = viewDO->GetActiveView();
    switch(view){
        case Views::SliceView:
            mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetSliceView()->GetImage();
            break;
        case Views::MosaicView:
            mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetMosaicView()->GetImage();
            break;
        case Views::OrthoView:
            {
                FrOrthoView* ov =  mv->GetOrthoView();

                // Find Image where click's occured
                for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
                    if (ov->GetImage(i)->IsInViewport(m_mouseX, m_mouseY)){
                        imgNumber = i; 
                        break;
                    }
                }
                if (imgNumber != -1){
                    ov->GetImage(imgNumber)->GetRenderers(renCollection);
                    lim = ov->GetImage(imgNumber);
                }
                else{
                    return false;
                }
                break;
            }
    } // end switch(view)

    // we should get any layer with visible actor
    int visibleLayerNum = GetVisibleLayer(doc);
    vtkRenderer* renderer = renCollection[visibleLayerNum];	

    if (!m_PointPicker->Pick(m_mouseX, m_mouseY, 0, renderer)) {
        return false;
    }

    // get image data
    vtkImageData* pImageData = lim->GetImageInput();        // TODO: check
    if(!pImageData) return false;

    // Get the mapped position of the mouse using the picker.
    double ptMapped[3];
    m_PointPicker->GetMapperPosition(ptMapped);
    ptMapped[2] = 0;

/*    double dSpacing[3];	
    pImageData->GetSpacing(dSpacing);	*/	

    // Get the volume index within the entire volume now.
    int nPointIdx = pImageData->FindPoint(ptMapped);

    int Index[3];
    Index[0] = ptMapped[0];
    Index[1] = ptMapped[1];
    Index[2] = ptMapped[2];

    GetRealImagePosition(viewDO, pImageData, Index, imgNumber);

    // convert coordinates if we have mosaic view selected
    if (view = Views::MosaicView){
        // we need to now number of slices in a row, or width/height of one slice
        int sliceDims[3];
        mv->GetSliceView()->GetImage()->GetImageInput()->GetDimensions(sliceDims);
        
        int mosaicDims[3];
        pImageData->GetDimensions(mosaicDims);

        int num = mosaicDims[0]/sliceDims[0];
        int col, row;
        col = Index[0]/sliceDims[0];
        row = Index[1]/sliceDims[1];
        int x, y, z;
        x = Index[0] - col*sliceDims[0];
        y = Index[1] - row*sliceDims[1];
        z = row*num + col;

        Index[0] = x; Index[1] = y; Index[2] = z;
    }

    // get points doc obj, insert point and update view
    FrPointsDocObj* pointsDO = 0L;
    FrDocument::DocObjCollection pointObjects;
    doc->GetObjectsByType(pointObjects, FrDocumentObj::PointsObject);    
    
    if(pointObjects.size() > 0){
        pointsDO = (FrPointsDocObj*)pointObjects[0];
        // TODO: get color from any widget/dialog/settings
        FrPoint* point = new FrPoint(Index[0], Index[1], Index[2], QColor(0, 255, 0));
        
        switch (m_Action){
            case Action::Add:
                pointsDO->AddPoint(point);
                break;
            case Action::Remove:
                pointsDO->RemovePoint(point);
                break;
        }

        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
    }
    else
        return false;
        
    return true;
}

int FrVoxelSelectionCmd::GetVisibleLayer(FrMainDocument* doc){
    // get all layer settings
    FrLayerDocObj* layerDO = 0L;
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    
    
    //std::vector<FrDocumentObj*>::iterator it, itEnd(layers.end());
    if(layers.size() > 0){
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);

            if (layerDO->GetVisibility()){
                return i;
            }
        }
//        layerDO = (FrLayerDocObj*)layers[0];
    }    

    return 0;   // there are no visible actors atm
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrVoxelSelectionCmd::CanUndo(){
    return false;
}

bool FrVoxelSelectionCmd::Undo(){
    return false;
}

bool FrVoxelSelectionCmd::Redo(){
    return false;
}
