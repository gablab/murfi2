#include "FrSelection.h"
#include "FrUtils.h"


// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageMapToColors.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkActor.h"
#include "vtkPointData.h"
#include "vtkActor2d.h"
#include "vtkCellArray.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkSphereSource.h"
#include "vtkRegularPolygonSource.h"


vtkStandardNewMacro(FrSelection);

// Construct object with no children.
FrSelection::FrSelection(){
    m_mapper = vtkPolyDataMapper::New();
	m_actor = vtkActor::New();
//    m_actor->PickableOff();

	this->AddPart(m_actor);
	this->PickableOff();
}

FrSelection::~FrSelection(){
	if (m_actor) m_actor->Delete();
    if (m_mapper) m_mapper->Delete();
}

void FrSelection::SetSelectionParams(SelectionParams params){
    m_params = params;

    switch (m_params.type){
        case 0:
            // display nothing
            break;
        case 1:          // rectangle
            DrawRectangle();
            break;
        case 2:         // circle
            DrawCircle();
            break;
        case 3:         // polygon
            DrawPolygon();
            break;
    }
}

void FrSelection::DrawRectangle(){
    int minx, miny, maxx, maxy;
    minx = m_params.firstPoint[0];
    miny = m_params.firstPoint[1];
    maxx = m_params.secondPoint[0];
    maxy = m_params.secondPoint[1];

    vtkPoints* m_BorderPts = vtkPoints::New();
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

    m_mapper->SetInput(selectRect);
//    m_mapper->Modified();
   
    m_actor->SetMapper(m_mapper);
    m_actor->Modified();

    // cleanup
    rect->Delete();
    selectRect->Delete();
    m_BorderPts->Delete();

	this->AddPart(m_actor);
	this->PickableOff();
}

void FrSelection::DrawCircle(){
	vtkRegularPolygonSource *circle = vtkRegularPolygonSource::New();
	circle->GeneratePolygonOff();
	circle->SetNumberOfSides(360);
	circle->SetRadius(100);

	m_mapper->SetInputConnection(circle->GetOutputPort());
    m_mapper->Modified();

	m_actor->SetMapper(m_mapper);
    m_actor->Modified();

    // cleanup
    circle->Delete();

    this->AddPart(m_actor);
	this->PickableOff();
}

void FrSelection::DrawPolygon(){
    int size = m_params.points.size();

    vtkPoints* m_BorderPts = vtkPoints::New();

    vtkCellArray* rect = vtkCellArray::New();
    rect->InsertNextCell(size+1);

    // get all points from params
    for (int i = 0; i<size; i++){
        m_BorderPts->InsertPoint(i, m_params.points[i].x, m_params.points[i].y, m_params.points[i].z);
        rect->InsertCellPoint(i);
    }

    rect->InsertCellPoint(0);

	vtkPolyData* selectRect = vtkPolyData::New();
	selectRect->SetPoints(m_BorderPts);
    selectRect->SetLines(rect);

    m_mapper->SetInput(selectRect);
    m_mapper->Modified();
   
    m_actor->SetMapper(m_mapper);
    m_actor->Modified();

    // cleanup
    rect->Delete();
    selectRect->Delete();
    m_BorderPts->Delete();
}
