#include "FrBaseView.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"
#include "FrSettings.h"
#include "QVTKWidget.h"

#include "vtkRenderWindow.h"

FrBaseView::FrBaseView(FrMainWindow* mainWindow){
    m_MainWindow = mainWindow;    
    m_QtView = mainWindow->GetQVTKWidget();
    m_RenderWindow = m_QtView->GetRenderWindow();
}

FrBaseView::~FrBaseView(){
}

FrLayerSettings* FrBaseView::GetActiveLayer(
    std::vector<FrLayerSettings*>& layers, int activeLayerID){
    FrLayerSettings* result = 0L;
    
    if(activeLayerID != ALL_LAYERS_ID){
        // Find proper layer
        std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
        for(it = layers.begin(); it != itEnd; ++it){
            if( (*it)->ID == activeLayerID ){
                result = (*it);
                break;
            }
        }
    }

    return result;
}

void FrBaseView::GetRoiIDs(FrMainDocument* document, std::vector<int>& ids){
    std::vector<FrDocumentObj*> objects;
    document->GetObjectsByType(objects, FrDocumentObj::RoiObject);

    // Iterate through all found rois
    std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
    for(itr = objects.begin(); itr != itrEnd; ++itr){
        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
        ids.push_back(roiDO->GetID());
    }
}

bool FrBaseView::GetRoiParams(FrMainDocument* document, int roiID, 
                               bool& roiVisibility, double& roiOpacity){
    std::vector<FrDocumentObj*> objects;
    document->GetObjectsByType(objects, FrDocumentObj::RoiObject);

    // Iterate through all found rois
    std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
    for(it = objects.begin(); it != itEnd; ++it){
        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
        if(roiDO->GetID() == roiID){
            roiVisibility = roiDO->GetVisibility();
            roiOpacity = roiDO->GetOpacity();
            return true;
        }
    }
    return false;
}