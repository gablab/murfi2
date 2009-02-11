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
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    bool result = false;

    if(viewDO->GetActiveView() == OrthoView){      
      result = ChangeOrthoViewSliceNums();
    }

    // reset all params
    m_isXY = false;
    m_isSlice = false;

    if(result){
        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
    }

    return result;
}

bool FrChangeSliceCmd::ChangeOrthoViewSliceNums(){
  if( !m_isXY ) return false;
 
  FrMainDocument* doc = this->GetMainController()->GetMainDocument();
  FrMainWindow* mv = this->GetMainController()->GetMainView(); 
  FrOrthoView* ov = mv->GetOrthoView();
  FrViewDocObj* viewDO = doc->GetCurrentViewObject();
 
  bool result = false;
 
  // Find Image where click's occured
  int imgIndex = INVALIDE_IMAGE_NUM;
  for(int idx=0; idx < ORTHO_VIEWS_CNT; ++idx){
    if (ov->GetImage(idx)->IsInViewport(m_X, m_Y)){
      imgIndex = idx; 
    }
  }
 
  if(imgIndex != INVALIDE_IMAGE_NUM){        
    // Get local renderer point
    double* point;
    double localPoint[3];
    vtkCoordinate* coordinate = vtkCoordinate::New();
    coordinate->SetCoordinateSystemToDisplay();
    coordinate->SetValue(m_X, m_Y, 0.0);

    std::vector<vtkRenderer*> renderers;
    ov->GetImage(imgIndex)->GetRenderers(renderers);
    point = coordinate->GetComputedWorldValue(renderers[0]);

    localPoint[0] = point[0];
    localPoint[1] = point[1];
    localPoint[2] = point[2];
    coordinate->Delete();
       
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL] = 
      GetCoronalSlice(localPoint[0], localPoint[1], imgIndex, viewDO);
    viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL] = 
      GetSagitalSlice(localPoint[0], localPoint[1], imgIndex, viewDO);
    viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL]   = 
      GetAxialSlice(localPoint[0], localPoint[1], imgIndex, viewDO);

    result = true;
  }
  return result;
}
 
int FrChangeSliceCmd::GetCoronalSlice(double x, double y, int imgNum, FrViewDocObj* viewDO){
  FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
         
  double spacing[3];
  ov->GetImage(imgNum)->GetImageInput()->GetSpacing(spacing);
 
  double* bounds = ov->GetImage(imgNum)->GetActorBounds();
  double result = double(viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL]);
  switch(imgNum)
    {
    case DEF_SAGITAL: result = ClampValue(x, 0.0, bounds[1]) / spacing[0];
      break;
    case DEF_AXIAL:  result = ClampValue(y, 0.0, bounds[3]) / spacing[1];
      break;
    }
  return int(result);
}
 
int FrChangeSliceCmd::GetSagitalSlice(double x, double y, int imgNum, FrViewDocObj* viewDO){
  FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    
  double spacing[3];
  ov->GetImage(imgNum)->GetImageInput()->GetSpacing(spacing);
 
  double* bounds = ov->GetImage(imgNum)->GetActorBounds();
  double result = double(viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL]);
  switch(imgNum)
    {
    case DEF_CORONAL: result = ClampValue(x, 0.0, bounds[1]) / spacing[0];
      break;
    case DEF_AXIAL: result = ClampValue(x, 0.0, bounds[1]) / spacing[0];
      break;
    }
  return int(result);
}
 
int FrChangeSliceCmd::GetAxialSlice(double x, double y, int imgNum, FrViewDocObj* viewDO){
  FrOrthoView* ov = this->GetMainController()->GetMainView()->GetOrthoView();
    
  double spacing[3];
  ov->GetImage(imgNum)->GetImageInput()->GetSpacing(spacing);
 
  double* bounds = ov->GetImage(imgNum)->GetActorBounds();
  double result = double(viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL]);
  switch(imgNum)
    {	
    case DEF_CORONAL: result = ClampValue(y, 0.0, bounds[3]) / spacing[1];
      break;
    case DEF_SAGITAL: result = ClampValue(y, 0.0, bounds[3]) / spacing[1];
      break;
    }
  return int(result);
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
