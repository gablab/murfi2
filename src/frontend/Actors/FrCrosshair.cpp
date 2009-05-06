#include "FrCrosshair.h"


// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageMapToColors.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkActor.h"
#include "vtkPointData.h"
#include "vtkActor2D.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkSphereSource.h"
#include "vtkRegularPolygonSource.h"
#include "vtkPixel.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkImageActor.h"
#include "vtkPNGReader.h"

vtkStandardNewMacro(FrCrosshair);

// Construct object with no children.
FrCrosshair::FrCrosshair() {
  
  m_mapper = vtkPolyDataMapper::New();
  m_actor = vtkActor::New();


  // make the single valued lookup
  m_lut = vtkLookupTable::New();
  m_lut->SetNumberOfTableValues(1);
  m_lut->SetTableRange(0, 0);
  m_lut->SetTableValue(0, 0.0, 1.0, 0.0, 1.0);
  m_lut->Build();

  m_mapper->UseLookupTableScalarRangeOn();

  m_actor->SetVisibility(false);
  
  this->AddPart(m_actor);
  this->PickableOff();
}

FrCrosshair::~FrCrosshair(){
  if (m_actor) m_actor->Delete();
  if (m_mapper) m_mapper->Delete();
  if (m_lut) m_lut->Delete();
}

void FrCrosshair::SetCrosshairParams(CrosshairParams params){
  m_params = params;
}

void FrCrosshair::DrawCrosshair(){

  vtkPoints *linePoints = vtkPoints::New();
  linePoints->InsertPoint(0, m_params.x, m_params.ymin, 0);
  linePoints->InsertPoint(1, m_params.x, m_params.ymax, 0);
  linePoints->InsertPoint(2, m_params.xmin, m_params.y, 0);
  linePoints->InsertPoint(3, m_params.xmax, m_params.y, 0);


  vtkCellArray* lines = vtkCellArray::New();
  // x
  lines->InsertNextCell(2);
  lines->InsertCellPoint(0);
  lines->InsertCellPoint(1);

  // y
  lines->InsertNextCell(2);
  lines->InsertCellPoint(2);
  lines->InsertCellPoint(3);

  vtkPolyData* lineData = vtkPolyData::New();
  lineData->SetPoints(linePoints);
  lineData->SetLines(lines);
  m_mapper->SetInput(lineData);

  // set the color
  vtkFloatArray *colors = vtkFloatArray::New();
  lineData->GetCellData()->SetScalars(colors);
  colors->InsertTuple1(0, 0.0);
  colors->InsertTuple1(1, 0.0);

  m_actor->GetProperty()->SetLineWidth(m_params.lineWidth);

  m_mapper->SetLookupTable(m_lut);
  m_actor->SetVisibility(true);   
  m_actor->SetMapper(m_mapper);
  m_actor->Modified();

  colors->Delete();  
  linePoints->Delete();
  lines->Delete();
  lineData->Delete();
} 

void FrCrosshair::HideAll(){
  m_actor->SetVisibility(false);
}
