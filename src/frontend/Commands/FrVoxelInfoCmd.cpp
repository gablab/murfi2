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
#include "FrImageDocObj.h"
#include "FrPointsDocObj.h"
#include "FrGraphSettings.h"
#include "FrGraphDocObj.h"

// VTK stuff
#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


//Some defines
#define INVALIDE_RENDERER_NUM -1


FrVoxelInfoCmd::FrVoxelInfoCmd()
  : m_Action(FrVoxelInfoCmd::Undefined), m_PointPicker(0), m_isMouseXY(false),
    m_isVoxel(false) {
  v[0] = 0;
  v[1] = 0;
  v[2] = 0;
}

void FrVoxelInfoCmd::SetMouseXY(int x, int y){
    m_mouseX = x; m_mouseY = y;
    m_isMouseXY = true;
}

void FrVoxelInfoCmd::SetVoxel(int _v[3]){
  v[0] = _v[0];
  v[1] = _v[1];
  v[2] = _v[2];
  m_isVoxel = true;
}

bool FrVoxelInfoCmd::Execute(){
    if(!this->GetMainController() ||
        m_Action == FrVoxelInfoCmd::Undefined ||
       !(m_isMouseXY || m_isVoxel) ) return false;

    bool result = false;
    switch(m_Action){
        case Update:
            result = UpdateVoxelInfoMouse(false);
            break;
        case Add:
            result = UpdateVoxelInfoMouse(true);
            break;
        case Set:
            result = UpdateVoxelInfoDirect(true);
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

bool FrVoxelInfoCmd::UpdateVoxelInfoMouse(bool addPoint){
    // get renderer    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    std::vector<vtkRenderer*> renCollection;
    int imgNumber = -1;
    FrLayeredImage* lim = 0;

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    switch(viewDO->GetActiveView()){
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

    VoxelData vd;
    vd.name = "Timeseria 1";   // get timeseria
    vd.timepoint = viewDO->GetTimePoint();  // get timepoint
    
    vd.Index[0] = int(ptMapped[0]);
    vd.Index[1] = int(ptMapped[1]);
    vd.Index[2] = int(ptMapped[2]);

    // TODO: fix for side values
    GetRealImagePosition(viewDO, pImageData, vd.Index, imgNumber);
    vd.Position[0] = int(vd.Index[0] * dSpacing[0]);
    vd.Position[1] = int(vd.Index[1] * dSpacing[1]);
    vd.Position[2] = int(vd.Index[2] * dSpacing[2]);

    return UpdateVoxelInfo(vd, pImageData, addPoint);
}

bool FrVoxelInfoCmd::UpdateVoxelInfoDirect(bool addPoint){
    // get renderer    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    std::vector<vtkRenderer*> renCollection;
    int imgNumber = -1;
    FrLayeredImage* lim = 0;

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    switch(viewDO->GetActiveView()){
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
                    ResetVoxelInfo();
                    return false;
                }
                break;
            }
    } // end switch(view)

    // get image data
    vtkImageData* pImageData = lim->GetImageInput();        // TODO: check
    if(!pImageData) return false;

    double dSpacing[3];	
    pImageData->GetSpacing(dSpacing);		

    VoxelData vd;
    vd.name = "Timeseria 1";   // get timeseria
    vd.timepoint = viewDO->GetTimePoint();  // get timepoint
    
    vd.Index[0] = int(v[0]);
    vd.Index[1] = int(v[1]);
    vd.Index[2] = int(v[2]);

    // TODO: fix for side values
    //GetRealImagePosition(viewDO, pImageData, vd.Index, imgNumber);
    vd.Position[0] = int(vd.Index[0] * dSpacing[0]);
    vd.Position[1] = int(vd.Index[1] * dSpacing[1]);
    vd.Position[2] = int(vd.Index[2] * dSpacing[2]);

    return UpdateVoxelInfo(vd, pImageData, addPoint);
}

bool FrVoxelInfoCmd::UpdateVoxelInfo(VoxelData &vd, vtkImageData* pImageData, bool addPoint) {
    // get renderer    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // Get the volume index within the entire volume now.
    int nPointIdx = pImageData->FindPoint(vd.Index[0], vd.Index[1], vd.Index[2]);

    // We have to handle different number of scalar components.
    unsigned short usPix;
    unsigned char usPixR;
    unsigned char usPixG;
    unsigned char usPixB;
    usPix = usPixR = usPixG = usPixB = 0;

    // Get voxel index, position
    // this should be done for all layers

    // get all layer settings
    FrLayerDocObj* layerDO = 0L;
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    if(layers.size() > 0){
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);

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

    bool result = false;
    if (addPoint){
        result = AddPoint(vd.Index, pImageData);   
    }

    return result;
}

bool FrVoxelInfoCmd::AddPoint(int* Index, vtkImageData* pImageData){
  


    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    Views view = viewDO->GetActiveView();

    if(view == MosaicView && !pImageData) return false;

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
                case FrVoxelInfoCmd::Add:
                case FrVoxelInfoCmd::Set:
                    pointsDO->AddPoint(point);
                    break;
           //     case FrVoxelInfoCmd::Remove:
                    //pointsDO->RemovePoint(point);
           //         break;
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
    
    
    return result;
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
                return i;
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
