#include "FrLayer.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkCamera.h"


// Default constructor
FrLayer::FrLayer(LayerSettings ls, vtkRenderWindow *renWindow, int layNum)
: FrBaseLayer(renWindow){
	//m_renderWindow = renWindow;
	
	m_actor = vtkImageActor::New();
	GetRenderer()->SetLayer(layNum);
	m_Id = layNum;

	// some vars
	m_Opacity = (float)ls.Opacity/255.0f;	// range is 0..1
	m_CMThreshold = ls.Threshold;
	m_Visible = ls.Visible;
	m_PxMin = 0;				// should be included to params
	m_PxMax = 255;				// should be included to params
	m_CMType = ls.ColormapType;
	m_Color = ls.color;

	m_Threshold = 0;			// should be included to params
	m_Brightness = 0;			// should be included to params
	m_Contrast = 0;				// should be included to params

	SetOpacity(m_Opacity);
	SetVisibility(m_Visible);
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

	m_cmfilter = FrColormapFilter::New();	// test

//	Update();
}

void FrLayer::SetInput(vtkImageData* input){
	m_inputData = input;

	if (m_CMType){
		m_cmfilter->SetInput(input);				// test
		m_cmfilter->SetThreshold(m_CMThreshold);
		m_cmfilter->SetPxMin(m_PxMin);
		m_cmfilter->SetPxMax(m_PxMax);
		m_cmfilter->SetCMType(m_CMType);
		m_cmfilter->SetColor(m_Color);
		m_cmfilter->Update();						// test

		m_actor->SetInput(m_cmfilter->GetOutput());
	}
	else
		m_actor->SetInput(input);
	
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

//	Update();
}

void FrLayer::SetOpacity(float value){
	m_Opacity = value;
	m_actor->SetOpacity(m_Opacity);

//	Update();
}

void FrLayer::SetVisibility(bool value){
	m_actor->SetVisibility(value);

//	Update();
}

void FrLayer::SetParams(LayerSettings& lSettings){
	// TODO: not finished
	m_Color = lSettings.color;
	m_CMThreshold = lSettings.Threshold;
	m_Visible = lSettings.Visible;
}

void FrLayer::SetLookupTable(vtkLookupTable* ltable){
	//m_actor->SetLookupTable(ltable);

//	Update();
}


void FrLayer::Update(){
//	GetRenderer()->ResetCamera();
	GetRenderer()->Render();
}

