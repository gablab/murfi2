#include "FrBaseView.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"
#include "FrSettings.h"
#include "QVTKWidget.h"

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkgl.h" // ohinds: 2009-02-01, added 

FrBaseView::FrBaseView(FrMainWindow* mainWindow){
    m_MainWindow = mainWindow;    
    m_QtView = mainWindow->GetQVTKWidget();
    m_RenderWindow = m_QtView->GetRenderWindow();
}

FrBaseView::~FrBaseView(){
}
//
//FrLayerSettings* FrBaseView::GetActiveLayer(
//    std::vector<FrLayerSettings*>& layers, int activeLayerID){
//    FrLayerSettings* result = 0L;
//    
//    if(activeLayerID != ALL_LAYER_ID){
//        // Find proper layer
//        std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
//        for(it = layers.begin(); it != itEnd; ++it){
//            if( (*it)->ID == activeLayerID ){
//                result = (*it);
//                break;
//            }
//        }
//    }
//
//    return result;
//}

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

//bool FrBaseView::GetRoiParams(FrMainDocument* document, int roiID, 
//                               bool& roiVisibility, double& roiOpacity){
//    std::vector<FrDocumentObj*> objects;
//    document->GetObjectsByType(objects, FrDocumentObj::RoiObject);
//
//    // Iterate through all found rois
//    std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
//    for(it = objects.begin(); it != itEnd; ++it){
//        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
//        if(roiDO->GetID() == roiID){
//            roiVisibility = roiDO->GetVisibility();
//            roiOpacity = roiDO->GetOpacity();
//            return true;
//        }
//    }
//    return false;
//}

#include "vtkToolkits.h"
#include "vtkOpenGL.h"

#if defined Q_WS_X11
#if defined(VTK_USE_OPENGL_LIBRARY) || (!defined(QVTK_HAVE_VTK_4_5) && defined(OPENGL_LIBRARY))
#include "vtkXOpenGLRenderWindow.h"
#endif
#ifdef VTK_USE_MANGLED_MESA
#include "vtkXMesaRenderWindow.h"
#endif
#endif

void FrBaseView::ResetCurrentContext(vtkRenderWindow* renWin)
{
    if(!renWin) return;

#ifdef WIN32
    // Windows
    wglMakeCurrent(NULL, NULL);
#else
    // LINUX
    #if defined(VTK_USE_OPENGL_LIBRARY) || (!defined(QVTK_HAVE_VTK_4_5) && defined(OPENGL_LIBRARY))
    vtkXOpenGLRenderWindow* oglWin = vtkXOpenGLRenderWindow::SafeDownCast(renWin);
    if(oglWin)
    {
        glXMakeCurrent(oglWin->GetDisplayId(), NULL, NULL);
    }
    #endif
    #ifdef VTK_USE_MANGLED_MESA
    vtkXMesaRenderWindow* mglWin = vtkXMesaRenderWindow::SafeDownCast(renWin);
    if(mgl_win)
    {
        // NOTE: not implemented yet
        // OsMesaMakeCurrent...
    }
    #endif
#endif
}
