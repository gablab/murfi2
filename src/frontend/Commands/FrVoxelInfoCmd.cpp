#include "FrVoxelInfoCmd.h"
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

#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


//Some defines
#define INVALIDE_RENDERER_NUM -1


FrVoxelInfoCmd::FrVoxelInfoCmd()
: m_isMouseXY(false), m_Action(FrVoxelInfoCmd::Undefined), m_PointPicker(0) {

}

void FrVoxelInfoCmd::SetMouseXY(int x, int y){
    m_mouseX = x; m_mouseY = y;
    m_isMouseXY = true;
}

bool FrVoxelInfoCmd::Execute(){
    if(!this->GetMainController() ||
        m_Action == FrVoxelInfoCmd::Undefined ||
        !m_isMouseXY) return false;

    bool result = false;
    switch(m_Action){
        case Update:
            result = UpdateVoxelInfo();
            break;
        case Reset:
            result = ResetVoxelInfo();
            break;
        default:
            // Do nothing
            break;
    }
    return result;
}

bool FrVoxelInfoCmd::UpdateVoxelInfo(){
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
            //GetLayerSettings(ts->GetSliceViewSettings(), layers);
            break;
        case Views::MosaicView:
            mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetMosaicView()->GetImage();
            //GetLayerSettings(ts->GetMosaicViewSettings(), layers);
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
                    //GetLayerSettings(ts->GetOrthoViewSettings(), layers, imgNumber);
                }
                else{
                    ResetVoxelInfo();
                    return false;
                }
                break;
            }
    } // end switch(view)

    // we should get any layer with visible actor
    int visibleLayerNum = GetVisibleLayer(doc);
    vtkRenderer* renderer = renCollection[visibleLayerNum];	

    if (!m_PointPicker->Pick(m_mouseX, m_mouseY, 0, renderer)) {
        ResetVoxelInfo();
        return false;
    }

    // get image data
    vtkImageData* pImageData = lim->GetImageInput();        // TODO: check
    if(!pImageData) return false;

    // Get the mapped position of the mouse using the picker.
    double ptMapped[3];
    m_PointPicker->GetMapperPosition(ptMapped);
    ptMapped[2] = 0;

    double dSpacing[3];	
    pImageData->GetSpacing(dSpacing);		

    // Get the volume index within the entire volume now.
    int nPointIdx = pImageData->FindPoint(ptMapped);

    // We have to handle different number of scalar components.
    unsigned short usPix;
    unsigned char usPixR;
    unsigned char usPixG;
    unsigned char usPixB;
    usPix = usPixR = usPixG = usPixB = 0;

    VoxelData vd;
    vd.name = "test";   // get layer (image?) name
    vd.timepoint = 44;  // get timepoint

    // TODO: fix for side values
    GetRealImagePosition(viewDO, pImageData, vd.Index, imgNumber);
    vd.Position[0] = int(vd.Index[0] * dSpacing[0]);
    vd.Position[1] = int(vd.Index[1] * dSpacing[1]);
    vd.Position[2] = int(vd.Index[2] * dSpacing[2]);


    // Get voxel index, position
    // this should be done for all layers

    // get all layer settings
    FrLayerDocObj* layerDO = 0L;
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    if(layers.size() > 0){
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);

            // get image data
            pImageData = lim->GetLayerByID(layerDO->GetID())->GetInput();

            switch (pImageData->GetNumberOfScalarComponents()) {
                case 1:
                    {
                            unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
                            usPix = pPix[nPointIdx];
                    }
                    break;
                case 3:
                    {
                            unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
                            usPixR = pPix[nPointIdx * 3 + 0];
                            usPixG = pPix[nPointIdx * 3 + 1];
                            usPixB = pPix[nPointIdx * 3 + 2];

                            usPix = (usPixR + usPixG + usPixB)/3;
                    }
                    break;

                default:
                    break;
            }

            // Set table values
            Voxel v;
            v.name = layerDO->GetSettings()->Name;

            // get voxel value from current layer
            v.value = usPix;
            vd.voxels.push_back(v);
        }
    }    

    mv->GetVoxelInfoWidget()->SetVoxelData(vd);
    return false;
}

bool FrVoxelInfoCmd::ResetVoxelInfo(){
	// reset main properties
	FrMainWindow* mv = this->GetMainController()->GetMainView();
	mv->GetVoxelInfoWidget()->Clear();	// clear table of voxel values

    return false;
}

void FrVoxelInfoCmd::GetVoxelInfo(){
}

int FrVoxelInfoCmd::GetVisibleLayer(FrMainDocument* doc){
    // get all layer settings
    FrLayerDocObj* layerDO = 0L;
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    
    
    //std::vector<FrDocumentObj*>::iterator it, itEnd(layers.end());
    if(layers.size() > 0){
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);

            if (layerDO->GetVisibility()){
                return layerDO->GetID();
            }
        }
//        layerDO = (FrLayerDocObj*)layers[0];
    }    

    return 0;   // there are no visible actors atm
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrVoxelInfoCmd::CanUndo(){
    return false;
}

bool FrVoxelInfoCmd::Undo(){
    return false;
}

bool FrVoxelInfoCmd::Redo(){
    return false;
}
