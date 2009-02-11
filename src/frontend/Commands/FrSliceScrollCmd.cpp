#include "FrSliceScrollCmd.h"
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

FrSliceScrollCmd::FrSliceScrollCmd()
  : m_isSlice(false), m_Slice(0), m_changingSlice(0) {
    
}

void FrSliceScrollCmd::SetChangingSlice(int value){
  m_changingSlice = value;
}

void FrSliceScrollCmd::SetSliceDelta(double value){
    m_Slice = value;
    m_isSlice = true;
}

void FrSliceScrollCmd::SetMouseXY(int x, int y){
    m_X = x; m_Y = y;
    m_isXY = true;
}

void FrSliceScrollCmd::GetSelectedVoxel(int result[3]) {
  if(!this->GetMainController()) return;
  FrMainDocument* doc = this->GetMainController()->GetMainDocument();
  FrViewDocObj* viewDO = doc->GetCurrentViewObject();

  result[0] = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL];
  result[1] = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL];
  result[2] = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL];

  switch(m_changingSlice) {
    case DEF_SAGITAL:
      result[0] += (int)m_Slice;
      break;
    case DEF_CORONAL:
      result[1] += (int)m_Slice;
      break;
    case DEF_AXIAL:
      result[2] += (int)m_Slice;
      break;
  }
}

bool FrSliceScrollCmd::Execute(){
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    bool result = false;

    switch(viewDO->GetActiveView()){
        case SliceView:
        {
            if(m_isSlice){
                viewDO->GetSliceViewSettings()->SliceNumber += (int)m_Slice;
                result = true;
            }
            break;
        }
        case MosaicView:
        {
            // NOTE: do nothing here
            result = true;
            break;
        }
        case OrthoView:
        {
            if(m_isSlice && m_changingSlice != INVALIDE_IMAGE_NUM){
	      viewDO->GetOrthoViewSettings()->SliceNumber[m_changingSlice] 
		+= (int)m_Slice;
	      result = true;
	    }
            break;
        }
        default:
            result = false;
    }

    // reset all params
    m_isXY = false;
    m_isSlice = false;

    if(result){
        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
    }

    return result;
}


int FrSliceScrollCmd::GetOrthoViewSliceNum(int x, int y){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 
    FrOrthoView* ov = mv->GetOrthoView();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // Find Image where click's occured    
    for(int idx=0; idx < ORTHO_VIEWS_CNT; ++idx){
        if (ov->GetImage(idx)->IsInViewport(m_X, m_Y)){
	  return idx;
        }
    }

    return INVALIDE_IMAGE_NUM;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSliceScrollCmd::CanUndo(){
    return false;
}

bool FrSliceScrollCmd::Undo(){
    return false;
}

bool FrSliceScrollCmd::Redo(){
    return false;
}
