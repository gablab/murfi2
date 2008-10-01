#include "FrSpecialLayer.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkCamera.h"


FrSpecialLayer::FrSpecialLayer(vtkRenderWindow *renWindow)
: FrBaseLayer(renWindow){

	m_actor = Fr2DSliceActor::New();
	GetRenderer()->SetLayer(0);
}

FrSpecialLayer::~FrSpecialLayer(){
	if(m_actor) m_actor->Delete();
	if(m_inputData) m_inputData->Delete();
}

void FrSpecialLayer::Initialize(){
    GetRenderer()->GetActiveCamera()->ParallelProjectionOn();
    GetRenderer()->SetBackground(0.0, 0.0, 0.0);

	// add actors
	GetRenderer()->AddActor(m_actor);
}

void FrSpecialLayer::SetInput(vtkImageData *input){
	// TODO: check here if input data has another dimensions than before, else return
	m_inputData = input;

	m_actor->SetInput(input);
}

void FrSpecialLayer::SetCamera(CameraSettings &camSettings){
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

void FrSpecialLayer::SetLayer(int value){
	GetRenderer()->SetLayer(value);

	Update();
}

void FrSpecialLayer::Update(){
	GetRenderer()->Render();
}