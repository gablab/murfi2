#include "FrChangeSliceCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrOrthoView.h"
#include "FrLayeredImage.h"
#include "FrTabSettingsDocObj.h"
#include "FrUtils.h"
#include "FrViewDocObj.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkCoordinate.h"
#include "vtkPointPicker.h"
#include "vtkImageActor.h"

//Some defines
#define INVALIDE_IMAGE_NUM -1


// TODO: Slice settings should be relative !!!!!!

FrChangeSliceCmd::FrChangeSliceCmd()
: m_isSlice(false) {
    
}

void FrChangeSliceCmd::SetSliceDelta(double value){
    m_Slice = value;
    m_isSlice = true;
}

void FrChangeSliceCmd::SetMouseXY(int x, int y){
    m_X = x; m_Y = y;
    m_isXY = true;
}

bool FrChangeSliceCmd::Execute(){
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 

    bool result = false;
   
    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }
        
    Views view = viewDO->GetActiveView();

    switch(view){
        case Views::SliceView:
        {
            if(m_isSlice){
                viewDO->GetSliceViewSettings()->SliceNumber += (int)m_Slice;
                result = true;
            }
            break;
        }
        case Views::MosaicView:
        {
            // NOTE: do nothing here
            result = true;
            break;
        }
        case Views::OrthoView:
        {
            result = ChangeOrthoViewSliceNums();
            break;
        }
        default:
            result = false;
    }

    // reset all params
    m_isXY = false;
    m_isSlice = false;
    if(result){
        mv->GetCurrentView()->UpdatePipeline(FRP_READ);     // FRP_READ??
    }
    return result;
}

bool FrChangeSliceCmd::ChangeOrthoViewSliceNums(){
    if( !m_isXY ) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 
    FrOrthoView* ov = mv->GetOrthoView();

    // Find Image where click's occured
    int imgIndex = INVALIDE_IMAGE_NUM;
    for(int idx=0; idx < ORTHO_VIEWS_CNT; ++idx){
        if (ov->GetImage(idx)->IsInViewport(m_X, m_Y)){
            imgIndex = idx; 
            break;
        }
    }

    bool result = false;
    if(imgIndex != INVALIDE_IMAGE_NUM){        
        // Get local renderer point
        double* point;
        int localPoint[3];
        vtkCoordinate* coordinate = vtkCoordinate::New();
	    coordinate->SetCoordinateSystemToDisplay();
	    coordinate->SetValue(m_X, m_Y, 0.0);

        std::vector<vtkRenderer*> renderers;
        ov->GetImage(imgIndex)->GetRenderers(renderers);

        point = coordinate->GetComputedWorldValue(renderers[0]);

        localPoint[0] = int(point[0]);
        localPoint[1] = int(point[1]);
        localPoint[2] = int(point[2]);
        coordinate->Delete();

        //for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        //    double bounds[6];
        //    ov->GetImage(i)->GetActor()->GetBounds(bounds);
        //}

        FrViewDocObj* viewDO = 0L;
        FrDocument::DocObjCollection views;
        doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
        if(views.size() > 0){
            viewDO = (FrViewDocObj*)views[0];
        }

        viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL] = 
            GetCoronalSlice(localPoint[0], localPoint[1], imgIndex);
        viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL] = 
            GetSagitalSlice(localPoint[0], localPoint[1], imgIndex);
        viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL]   = 
            GetAxialSlice(localPoint[0], localPoint[1], imgIndex);

        result = true;
    }
    return result;
}

int FrChangeSliceCmd::GetCoronalSlice(int x, int y, int imgNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    double* bounds = ov->GetImage(imgNum)->GetActorBounds();
    int result = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL];
    switch(imgNum)
	{
        case DEF_SAGITAL: result = ClampValue(x, 0, int(bounds[1]));
			break;
		case DEF_AXIAL:  result = ClampValue(y, 0, int(bounds[3]));
			break;
	}
    return result;
}

int FrChangeSliceCmd::GetSagitalSlice(int x, int y, int imgNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
     
    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    double* bounds = ov->GetImage(imgNum)->GetActorBounds();
    int result = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL];
    switch(imgNum)
	{
        case DEF_CORONAL: result = ClampValue(x, 0, int(bounds[1]));
			break;
		case DEF_AXIAL: result = ClampValue(x, 0, int(bounds[1]));
			break;
	}
    return result;
}

int FrChangeSliceCmd::GetAxialSlice(int x, int y, int imgNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }
 
    double* bounds = ov->GetImage(imgNum)->GetActorBounds();
    int result = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL];
    switch(imgNum)
	{	
	    case DEF_CORONAL: result = ClampValue(y, 0, int(bounds[3]));
			break;
		case DEF_SAGITAL: result = ClampValue(y, 0, int(bounds[3]));
			break;
	}
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrChangeSliceCmd::CanUndo(){
    return false;
}

bool FrChangeSliceCmd::Undo(){
    return false;
}

bool FrChangeSliceCmd::Redo(){
    return false;
}
