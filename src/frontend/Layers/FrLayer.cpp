#include "FrLayer.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkCamera.h"


// Default constructor
FrLayer::FrLayer(vtkRenderWindow *renWindow, int layNum)
: FrBaseLayer(renWindow){
	//m_renderWindow = renWindow;

//	m_defaultActor = Fr2DSliceActor::New();		// test
	m_actor = vtkImageActor::New();
	GetRenderer()->SetLayer(layNum);

	// some vars
	m_Opacity = 1.0;
}

FrLayer::~FrLayer(){
	if(m_actor) m_actor->Delete();
	if(m_inputData) m_inputData->Delete();
}

void FrLayer::Initialize(){
    GetRenderer()->GetActiveCamera()->ParallelProjectionOn();
    GetRenderer()->SetBackground(0.0, 0.0, 0.0);

	// add actors
	GetRenderer()->AddActor(m_actor);
//	GetRenderer()->AddActor(m_defaultActor);	// test

//	Update();
}

void FrLayer::SetInput(vtkImageData* input){
	m_inputData = input;
	m_actor->SetInput(input);

//	m_defaultActor->SetInput(input);	// test
//	Update();
}

void FrLayer::SetCamera(CameraSettings& camSettings){
    vtkCamera* cam = 0L;    
	
	// Setup camera here 
    cam = GetRenderer()->GetActiveCamera();
    cam->ParallelProjectionOn();
    cam->SetParallelScale(camSettings.Scale);
    cam->SetFocalPoint(camSettings.FocalPoint);
    cam->SetViewUp(camSettings.ViewUp);
    cam->SetPosition(camSettings.Position);

	Update();
}

void FrLayer::SetOpacity(float value){
	m_Opacity = value;
	m_actor->SetOpacity(m_Opacity);

	Update();
}

void FrLayer::SetVisibility(bool value){
	m_actor->SetVisibility(value);

	Update();
}

void FrLayer::SetLookupTable(vtkLookupTable* ltable){
	//m_actor->SetLookupTable(ltable);

	Update();
}


void FrLayer::Update(){
//	GetRenderer()->ResetCamera();
	GetRenderer()->Render();
}
