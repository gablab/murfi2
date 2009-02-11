#ifndef FR_CROSSHAIR
#define FR_CROSSHAIR

#include "FrBaseActor.h"
#include "FrMacro.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include "vtkPlaneSource.h"
#include "vtkTexture.h"
#include "vtkProperty.h"
#include "vtkExtractVOI.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor2D.h"

#include <vector>

class vtkImageData;
class vtkImageActor;

typedef class _crosshairParams{
 public:

  double x;
  double ymin;
  double ymax;

  double y;
  double xmin;
  double xmax;

  int lineWidth;

_crosshairParams() :
  x(0), ymin(0), ymax(0), y(0), xmin(0), xmax(0), lineWidth(2){};
} CrosshairParams;


class FrCrosshair : public FrBaseActor {

 public:
  static FrCrosshair *New();
  vtkTypeMacro(FrCrosshair, FrBaseActor);

  // Properties
  void SetCrosshairParams(CrosshairParams params);
  FrGetPropMacro(CrosshairParams, params);
  FrSetPropMacro(vtkImageData*, Data);
  void DrawCrosshair();

 protected:
  FrCrosshair();
  virtual ~FrCrosshair();
  
 private:
  FrCrosshair(const FrCrosshair&) {};
  void operator=(const FrCrosshair&) {};

 private:
  void HideAll();
  
 private:
  vtkActor* m_actor;
  vtkImageActor* m_testActor;
  vtkPolyDataMapper* m_mapper;
  vtkLookupTable *m_lut;
};

#endif





