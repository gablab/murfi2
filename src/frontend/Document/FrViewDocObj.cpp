#include "FrViewDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"
#include "FrTabSettingsDocObj.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>

#define DEFAULT_TP 0
#define DEFAULT_TS 0

FrViewDocObj::FrViewDocObj()
: m_TimePoint(DEFAULT_TP), m_ActiveView(OrthoView) {
    // Create view settings
    m_SliceViewSettings = new FrSliceViewSettings();
    m_MosaicViewSettings = new FrMosaicViewSettings();
    m_OrthoViewSettings = new FrOrthoViewSettings();
}

FrViewDocObj::~FrViewDocObj(){
    delete m_SliceViewSettings;
    delete m_MosaicViewSettings;
    delete m_OrthoViewSettings;
}

void FrViewDocObj::OnAdd(FrDocument* doc){
    // Probably need the command updating current view
    FrChangeViewCmd* cmd = FrCommandController::CreateCmd<FrChangeViewCmd>();
    cmd->SetTargetView(FrChangeViewCmd::Synchronize);
    FrCommandController::Execute(cmd);
    delete cmd;
}

void FrViewDocObj::OnRemove(FrDocument* doc){
    // NOTE: Do nothing here!
}

FrDocumentObj::ObjTypes FrViewDocObj::GetType(){
    return FrDocumentObj::ViewObject;
}

bool FrViewDocObj::IsSliceView(){
    return (m_ActiveView == SliceView);
}

bool FrViewDocObj::IsMosaicView(){
    return (m_ActiveView == MosaicView);
}

bool FrViewDocObj::IsOrthoView(){
    return (m_ActiveView == OrthoView);
}

// init methods
void FrViewDocObj::CopySettingsFrom(FrTabSettingsDocObj* src){
    m_TimePoint = src->GetTimePoint();
    m_ActiveView = src->GetActiveView();
    *(m_SliceViewSettings) = *(src->GetSliceViewSettings());
    *(m_MosaicViewSettings) = *(src->GetMosaicViewSettings());
    *(m_OrthoViewSettings) = *(src->GetOrthoViewSettings());
}

void FrViewDocObj::SaveSettingsTo(FrTabSettingsDocObj* dst){
    dst->SetTimePoint(m_TimePoint);
    dst->SetActiveView(m_ActiveView);
    *(dst->GetSliceViewSettings()) = *(m_SliceViewSettings);
    *(dst->GetMosaicViewSettings()) = *(m_MosaicViewSettings);
    *(dst->GetOrthoViewSettings()) = *(m_OrthoViewSettings);
}

unsigned long FrViewDocObj::GetActiveLayerID(){
    unsigned long result = BAD_LAYER_ID;

    switch(this->GetActiveView()){
        case SliceView:
	  result = this->GetSliceViewSettings()->GetActiveLayerID();
	  break;
        case MosaicView:
	  result = this->GetMosaicViewSettings()->GetActiveLayerID();
	  break;
        case OrthoView:
	  result = this->GetOrthoViewSettings()->GetActiveLayerID();
	  break;
        default:break;
    }
    return result;
}


void FrViewDocObj::TransformCoordinatesToIndices(int point[2], vtkImageData* img, int orientation){

  double x = point[0];
  double y = point[1];
  TransformCoordinatesToIndices(x, y, img, orientation);
  point[0] = (int) x;
  point[1] = (int) y;
}

void FrViewDocObj::TransformCoordinatesToIndices(double point[2], vtkImageData* img, int orientation){

  double x = point[0];
  double y = point[1];
  TransformCoordinatesToIndices(x, y, img, orientation);
  point[0] = x;
  point[1] = y;
}

void FrViewDocObj::TransformCoordinatesToIndices(double &x, double &y, vtkImageData* img, int orientation){

    if(!img) return;

    double dSpacing[3];
    img->GetSpacing(dSpacing);

    x = (x-dSpacing[0]/2) / dSpacing[0] + 0.5;
    y = (y-dSpacing[1]/2) / dSpacing[1] + 0.5;

//    // calc current indices of point
//    switch(GetActiveView()){
//      case SliceView:
//	x = int((x + 1) / dSpacing[0]);
//	y = int((y + 1) / dSpacing[1]);
//	break;
//      case MosaicView:
//	x = int((x + 1) / dSpacing[0]);
//	y = int((y + 1) / dSpacing[1]);
//	break;
//      case OrthoView:
//	// Renderer number in ortho view
//	switch(orientation){
//	  case DEF_CORONAL:
//	    x = int((x + 1) / dSpacing[0]);
//	    y = int((y + 1) / dSpacing[1]);
//	    break;
//	  case DEF_SAGITAL:
//	    x = int((x + 1) / dSpacing[0]);
//	    y = int((y + 1) / dSpacing[1]);
//	    break;
//	  case DEF_AXIAL:
//	    x = int((x + 1) / dSpacing[0]);
//	    y = int((y + 1) / dSpacing[1]);
//	    break;
//	}
//	break;
//    } // end switch view
}

void FrViewDocObj::TransformIndicesToCoordinates(int point[2], vtkImageData* img, int orientation){

  double x = point[0];
  double y = point[1];
  TransformIndicesToCoordinates(x, y, img, orientation);
  point[0] = (int) rint(x);
  point[1] = (int) rint(y);
}

void FrViewDocObj::TransformIndicesToCoordinates(double point[2], vtkImageData* img, int orientation){

  double x = point[0];
  double y = point[1];
  TransformIndicesToCoordinates(x, y, img, orientation);
  point[0] = x;
  point[1] = y;
}

void FrViewDocObj::TransformIndicesToCoordinates(double &x, double &y, vtkImageData* img, int orientation){
    if(!img) return;

    double dSpacing[3];
    img->GetSpacing(dSpacing);
    double dOrigin[3];
    img->GetOrigin(dOrigin);

    x = dSpacing[0]*(x - 0.5) + dSpacing[0]/2 + dOrigin[0];
    y = dSpacing[1]*(y - 0.5) + dSpacing[0]/2 + dOrigin[1];

//    // calc current coords of point
//    switch(GetActiveView()){
//      case SliceView:
//	x = dSpacing[0]*x + dSpacing[0]/2;
//	y = dSpacing[1]*y + dSpacing[0]/2;
//	break;
//      case MosaicView:
//	x = dSpacing[0]*x - 1;
//	y = dSpacing[1]*y - 1;
//	break;
//      case OrthoView:
//	// Renderer number in ortho view
//	switch(orientation){
//	  case DEF_CORONAL:
//	    x = dSpacing[0]*x - 1;
//	    y = dSpacing[1]*y - 1;
//	    break;
//	  case DEF_SAGITAL:
//	    x = dSpacing[0]*x - 1;
//	    y = dSpacing[1]*y - 1;
//	    break;
//	  case DEF_AXIAL:
//	    x = dSpacing[0]*x - 1;
//	    y = dSpacing[1]*y - 1;
//	    break;
//	}
//	break;
//    } // end switch view
}

