#include "FrSpecialLayer.h"


// VTK stuff
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkTextActor.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"

vtkStandardNewMacro(FrSpecialLayer);

FrSpecialLayer::FrSpecialLayer() 
: m_TextMapper(0),	m_TextActor(0),
  m_BorderPts(0), m_BorderMapper(0), m_BorderActor(0){
    this->InitializeText();
    this->InitializeBorder();
}

FrSpecialLayer::~FrSpecialLayer() {
    // Free text objects
    if(m_TextMapper) m_TextMapper->Delete();
    if(m_TextActor) m_TextActor->Delete();
    // Free border objectsBorder support
    if(m_BorderPts) m_BorderPts->Delete();
    if(m_BorderMapper) m_BorderMapper->Delete();
    if(m_BorderActor) m_BorderActor->Delete();
}

void FrSpecialLayer::InitializeText(){
    // Create mapper
    m_TextMapper = vtkTextMapper::New();
    m_TextMapper->SetInput("");

    // Init property
    vtkTextProperty* pProperty = m_TextMapper->GetTextProperty();
	pProperty->SetBold(1);
    pProperty->SetItalic(0);
    pProperty->SetFontSize(20);
    pProperty->SetFontFamily(VTK_ARIAL);
    pProperty->SetColor(100.0, 100.0, 0.0);
    pProperty->SetJustification(VTK_TEXT_LEFT);
    pProperty->SetVerticalJustification(VTK_TEXT_CENTERED);
    
    // Create actor
    m_TextActor = vtkActor2D::New();
    m_TextActor->SetMapper(m_TextMapper);
    m_TextActor->SetPosition(20, 20);
    m_TextActor->PickableOff();
    m_Renderer->AddActor2D(m_TextActor);
}

void FrSpecialLayer::InitializeBorder(){
    int minx, miny, maxx, maxy;
    minx = miny = maxx = maxy = 0;

    m_BorderPts = vtkPoints::New();
    m_BorderPts->InsertPoint(0, minx, miny, 0);
    m_BorderPts->InsertPoint(1, maxx, miny, 0);
    m_BorderPts->InsertPoint(2, maxx, maxy, 0);
    m_BorderPts->InsertPoint(3, minx, maxy, 0);

    vtkCellArray* rect = vtkCellArray::New();
    rect->InsertNextCell(5);
    rect->InsertCellPoint(0);
    rect->InsertCellPoint(1);
    rect->InsertCellPoint(2);
    rect->InsertCellPoint(3);
    rect->InsertCellPoint(0);

    vtkPolyData* selectRect = vtkPolyData::New();
    selectRect->SetPoints(m_BorderPts);
    selectRect->SetLines(rect);

    m_BorderMapper = vtkPolyDataMapper2D::New();
    m_BorderMapper->SetInput(selectRect);
        
    m_BorderActor = vtkActor2D::New();
    m_BorderActor->PickableOff();
    m_BorderActor->SetMapper(m_BorderMapper);
    m_Renderer->AddActor2D(m_BorderActor);

    // Free this stuff
    rect->Delete();
    selectRect->Delete();
}

void FrSpecialLayer::SetText(const char* text){
    m_TextMapper->SetInput(text);
}

void FrSpecialLayer::UpdateBorder(int winWidth, int winHeight){
    int minx = 0;
    int miny = 0;
    int maxx = winWidth;
    int maxy = winHeight;

    m_BorderPts->InsertPoint(0, minx, miny, 0);
    m_BorderPts->InsertPoint(1, maxx, miny, 0);
    m_BorderPts->InsertPoint(2, maxx, maxy, 0);
    m_BorderPts->InsertPoint(3, minx, maxy, 0);
    m_BorderPts->Modified();
}
