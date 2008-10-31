#include "FrChangeSliceCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrOrthoView.h"
#include "FrLayeredImage.h"
#include "FrTabSettingsDocObj.h"
#include "FrUtils.h"


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
    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
    switch(sets->GetActiveView()){
        case FrTabSettingsDocObj::SliceView:
        {
            if(m_isSlice){
                sets->GetSliceViewSettings()->SliceNumber += (int)m_Slice;
                result = true;
            }
            break;
        }
        case FrTabSettingsDocObj::MosaicView:
        {
            // NOTE: do nothing here
            result = true;
            break;
        }
        case FrTabSettingsDocObj::OrthoView:
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
        mv->GetCurrentView()->UpdatePipeline(FRP_SLICE);
    }
    return result;
}

bool FrChangeSliceCmd::ChangeOrthoViewSliceNums(){
    if( !m_isXY ) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 
    FrOrthoView* ov = mv->GetOrthoView();

    // Find Image where click's occured
    int imgNumber = INVALIDE_IMAGE_NUM;
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        if (ov->GetImage(i)->GetRenderer()->IsInViewport(m_X, m_Y)){
            imgNumber = i; break;
        }
    }

    bool result = false;
    if(imgNumber != INVALIDE_IMAGE_NUM){        
        // Get local renderer point
        double* point;
        int localPoint[3];
        vtkCoordinate* coordinate = vtkCoordinate::New();
	    coordinate->SetCoordinateSystemToDisplay();
	    coordinate->SetValue(m_X, m_Y, 0.0);
        point = coordinate->GetComputedWorldValue(ov->GetImage(imgNumber)->GetRenderer());

        localPoint[0] = int(point[0]);
        localPoint[1] = int(point[1]);
        localPoint[2] = int(point[2]);
        coordinate->Delete();

        for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
            double bounds[6];
            ov->GetImage(i)->GetActor()->GetBounds(bounds);
        }

        FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
        sets->GetOrthoViewSettings()->CoronalSlice = 
            GetCoronalSlice(localPoint[0], localPoint[1], imgNumber);
        sets->GetOrthoViewSettings()->SagitalSlice = 
            GetSagitalSlice(localPoint[0], localPoint[1], imgNumber);
        sets->GetOrthoViewSettings()->AxialSlice   = 
            GetAxialSlice(localPoint[0], localPoint[1], imgNumber);

        result = true;
    }
    return result;
}

int FrChangeSliceCmd::GetCoronalSlice(int x, int y, int imgNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrTabSettingsDocObj* sets = this->GetMainController()->
        GetMainDocument()->GetCurrentTabSettings();
     
    double* bounds = ov->GetImage(imgNum)->GetActor()->GetBounds();
    int result = sets->GetOrthoViewSettings()->CoronalSlice;
    switch(imgNum)
	{
        case SAGITAL_IMAGE: result = ClampValue(x, 0, int(bounds[1]));
			break;
		case AXIAL_IMAGE:  result = ClampValue(y, 0, int(bounds[3]));
			break;
	}
    return result;
}

int FrChangeSliceCmd::GetSagitalSlice(int x, int y, int imgNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrTabSettingsDocObj* sets = this->GetMainController()->
        GetMainDocument()->GetCurrentTabSettings();
     
    double* bounds = ov->GetImage(imgNum)->GetActor()->GetBounds();
    int result = sets->GetOrthoViewSettings()->SagitalSlice;
    switch(imgNum)
	{
        case CORONAL_IMAGE: result = ClampValue(x, 0, int(bounds[1]));
			break;
		case AXIAL_IMAGE: result = ClampValue(x, 0, int(bounds[1]));
			break;
	}
    return result;
}

int FrChangeSliceCmd::GetAxialSlice(int x, int y, int imgNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrTabSettingsDocObj* sets = this->GetMainController()->
        GetMainDocument()->GetCurrentTabSettings();
     
    double* bounds = ov->GetImage(imgNum)->GetActor()->GetBounds();
    int result = sets->GetOrthoViewSettings()->AxialSlice;
    switch(imgNum)
	{	
	    case CORONAL_IMAGE: result = ClampValue(y, 0, int(bounds[3]));
			break;
		case SAGITAL_IMAGE: result = ClampValue(y, 0, int(bounds[3]));
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