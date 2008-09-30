#include "FrChangeSliceCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrOrthoView.h"
#include "FrTabSettingsDocObj.h"
#include "Fr2DSliceActor.h"
#include "FrUtils.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkCoordinate.h"
#include "vtkPointPicker.h"

//Some defines
#define INVALIDE_RENDERER_NUM -1


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
    case ActiveView::Slice:
        {
            if(m_isSlice){
                sets->GetSliceViewSettings().SliceNumber += m_Slice;
                result = true;
            }
            break;
        }
    case ActiveView::Mosaic:
        {
            if(m_isSlice){
                sets->GetMosaicViewSettings().SliceNumber += m_Slice;
                result = true;
            }
            break;
        }
    case ActiveView::Ortho:
        {
            result = ChangeOrthoViewSliceNums();
        }
        break;
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

typedef union dbl3{
    struct{ double x, y, z; };
    double raw[3];
} Dbl3;
std::vector<Dbl3> pts;
bool FrChangeSliceCmd::ChangeOrthoViewSliceNums(){
    if( !m_isXY ) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 
    FrOrthoView* ov = mv->GetOrthoView();

    // Find renderer
    int renNumber = INVALIDE_RENDERER_NUM;
    for(int i=0; i < RENDERER_COUNT-1; ++i){
        if (ov->GetRenderer(i)->IsInViewport(m_X, m_Y)){
            renNumber = i; break;
        }
    }

    bool result = false;
    if(renNumber != INVALIDE_RENDERER_NUM){        
        // Get local renderer point
        double* point;
        double localPoint[3];
        vtkCoordinate* coordinate = vtkCoordinate::New();
	    coordinate->SetCoordinateSystemToDisplay();
	    coordinate->SetValue(m_X, m_Y, 0.0);
	    point = coordinate->GetComputedWorldValue(ov->GetRenderer(renNumber));

        double* bounds = ov->GetActor(renNumber)->GetBounds();
        double b0 = bounds[0];
        double b1 = bounds[1];
        double b2 = bounds[2];
        double b3 = bounds[3];
        double b4 = bounds[4];
        double b5 = bounds[5];

        localPoint[0] = point[0];
        localPoint[1] = point[1];
        localPoint[2] = point[2];
        coordinate->Delete();
    
        FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
        sets->GetOrthoViewSettings().CoronalSlice = GetCoronalSlice(localPoint[0], localPoint[1], renNumber);
        sets->GetOrthoViewSettings().SagitalSlice = GetSagitalSlice(localPoint[0], localPoint[1], renNumber);
        sets->GetOrthoViewSettings().AxialSlice   = GetAxialSlice(localPoint[0], localPoint[1], renNumber);

        result = true;
    }
    return result;
}

int FrChangeSliceCmd::GetCoronalSlice(int x, int y, int renNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrTabSettingsDocObj* sets = this->GetMainController()->
        GetMainDocument()->GetCurrentTabSettings();
     
    double* bounds = ov->GetActor(renNum)->GetBounds();
    int result = sets->GetOrthoViewSettings().CoronalSlice;
    switch(renNum)
	{
        case SAGITAL_RENDERER: result = ClampValue(x, 0, int(bounds[1]));
			break;
		case AXIAL_RENDERER:  result = ClampValue(y, 0, int(bounds[3]));
			break;
	}
    return result;
}

int FrChangeSliceCmd::GetSagitalSlice(int x, int y, int renNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrTabSettingsDocObj* sets = this->GetMainController()->
        GetMainDocument()->GetCurrentTabSettings();
     
    double* bounds = ov->GetActor(renNum)->GetBounds();
    int result = sets->GetOrthoViewSettings().SagitalSlice;
    switch(renNum)
	{
        case CORONAL_RENDERER: result = ClampValue(x, 0, int(bounds[1]));
			break;
		case AXIAL_RENDERER: result = ClampValue(x, 0, int(bounds[1]));
			break;
	}
    return result;
}

int FrChangeSliceCmd::GetAxialSlice(int x, int y, int renNum){
    FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    FrTabSettingsDocObj* sets = this->GetMainController()->
        GetMainDocument()->GetCurrentTabSettings();
     
    double* bounds = ov->GetActor(renNum)->GetBounds();
    int result = sets->GetOrthoViewSettings().AxialSlice;
    switch(renNum)
	{	
	    case CORONAL_RENDERER: result = ClampValue(y, 0, int(bounds[3]));
			break;
		case SAGITAL_RENDERER: result = ClampValue(y, 0, int(bounds[3]));
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