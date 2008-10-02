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
	m_tactor = vtkTextActor::New();
	GetRenderer()->SetLayer(0);

	dims[0] = 0; dims[1] = 0; dims[2] = 0;
}

FrSpecialLayer::~FrSpecialLayer(){
	if(m_actor) m_actor->Delete();
	if(m_tactor) m_tactor->Delete();
	if(m_inputData) m_inputData->Delete();
}

void FrSpecialLayer::Initialize(){
    GetRenderer()->GetActiveCamera()->ParallelProjectionOn();
    GetRenderer()->SetBackground(0.0, 0.0, 0.0);

	// add actors
	GetRenderer()->AddActor(m_actor);
	GetRenderer()->AddActor(m_tactor);

	// set text actor properties
	m_tactor->GetTextProperty()->SetColor(100, 100, 0);
	m_tactor->GetTextProperty()->SetBold(5);
	m_tactor->GetTextProperty()->SetFontSize(20);
	m_tactor->SetPosition(20, 20);	// probably we should have a set of standard positions
}

void FrSpecialLayer::SetInput(vtkImageData *input){
	// check here if input data has another dimensions than before, else return
	// this should be done to reduce lags while setting new input
	int new_dims[3];
	input->GetDimensions(new_dims);
	if (new_dims[0] != dims[0] || new_dims[1] != dims[1]){
		for (int i = 0; i<3; i++)
			dims[i] = new_dims[i];
	}
	else
		return;

	m_inputData = input;

	m_actor->SetInput(input);
}

void FrSpecialLayer::SetText(const char* text){
	m_tactor->SetInput(text);

	//Update();
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