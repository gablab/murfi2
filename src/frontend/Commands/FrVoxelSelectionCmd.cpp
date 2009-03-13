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
#include "FrGraphDocObj.h"
#include "FrGraphSettings.h"
#include "FrGraphPaneWidget.h"
#include "FrImageDocObj.h"

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

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    Views view = viewDO->GetActiveView();
    switch(view){
        case SliceView:
            mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetSliceView()->GetImage();
            break;
        case MosaicView:
            mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetMosaicView()->GetImage();
            break;
        case OrthoView:
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
    Index[0] = int(ptMapped[0]);
    Index[1] = int(ptMapped[1]);
    Index[2] = int(ptMapped[2]);

    GetRealImagePosition(viewDO, pImageData, Index, imgNumber);


    FrImageDocObj* imgDO = 0;
    std::vector<FrDocumentObj*> images;
    doc->GetObjectsByType(images, FrDocumentObj::ImageObject);

     FrLayerDocObj* layerDO = doc->GetLayerDocObjByID(viewDO->GetActiveLayerID());

    RtDataID id = layerDO->GetSettings()->DataID;
    int dimensions[3];

    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){
        FrImageDocObj* imgDO = (FrImageDocObj*)(*it);
        if(imgDO->GetDataID() == id){
            RtMRIImage* img = imgDO->GetTimePointData(imgDO->GetLastTimePoint());
            dimensions[0] = img->getDim(0);
            dimensions[1] = img->getDim(1);
            dimensions[2] = img->getDim(2);
	    break;
        }
    }

    // convert coordinates if we have mosaic view selected
    if (view == MosaicView){
        // we need to know number of slices in a row, or width/height of one slice
        int mosaicDims[3];
        pImageData->GetDimensions(mosaicDims);

        int num = mosaicDims[0]/dimensions[0];
        int col, row;
        col = Index[0]/dimensions[0];
        row = Index[1]/dimensions[1];
        int x, y, z;
        x = Index[0] - col*dimensions[0];
        y = Index[1] - row*dimensions[1];
        z = row*num + col;

        Index[0] = x; Index[1] = y; Index[2] = z;
    }

    // get points doc obj, insert point and update view
    FrPointsDocObj* pointsDO = 0L;
    FrDocument::DocObjCollection pointObjects;
    doc->GetObjectsByType(pointObjects, FrDocumentObj::PointsObject);    

    bool result = false;    

    if(pointObjects.size() > 0){
        pointsDO = (FrPointsDocObj*)pointObjects[0];
        // TODO: get color from any widget/dialog/settings
        // due to requirements we have only 1 point at the same time
        pointsDO->ClearAll();

        // check if point has correct coordinates
        if (Index[0] < dimensions[0] && Index[1] < dimensions[1] &&
            Index[2] < dimensions[2]){
            result = true;

            FrPoint* point = new FrPoint(Index[0], Index[1], Index[2], QColor(0, 255, 0));

            switch (m_Action){
                case FrVoxelSelectionCmd::Add:
                    pointsDO->AddPoint(point);
                    break;
                case FrVoxelSelectionCmd::Remove:
                    //pointsDO->RemovePoint(point);
                    break;
            }

            FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
        }
    }
    else
        return false;
        
    if (result){ // point was succesfully added

        // update all graphs that depends on point position (that is Intencity graphs only for now)
        FrGraphDocObj* graphDO = 0L;
        FrDocument::DocObjCollection graphObjects;
        doc->GetObjectsByType(graphObjects, FrDocumentObj::GraphObject);    
        
        for (int i = 0; i < graphObjects.size(); i++){
            graphDO = (FrGraphDocObj*)graphObjects[i];

            if (graphDO->GetSettings()->GetType() == FrGraphSettings::GT_Intencity){
                // update point
                ((FrIntencityGraphSettings*)graphDO->GetSettings())->I = Index[0];
                ((FrIntencityGraphSettings*)graphDO->GetSettings())->J = Index[1];
                ((FrIntencityGraphSettings*)graphDO->GetSettings())->K = Index[2];
            }
        }
    }

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
