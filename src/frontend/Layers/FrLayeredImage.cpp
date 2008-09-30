#include "FrLayeredImage.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"


// Default constructor
FrLayeredImage::FrLayeredImage(vtkRenderWindow *renWindow){
	m_RenderWindow = renWindow;
	
	// set some vars
	m_ActiveLayer = 0;
	
	this->AddLayer();	// default layer
	this->AddLayer();	// default layer
	this->AddLayer();	// default layer
	this->AddLayer();	// default layer
	this->AddLayer();	// default layer
}

FrLayeredImage::~FrLayeredImage(){
//	if(m_inputData) m_inputData->Delete();
}

void FrLayeredImage::Initialize(){
	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->Initialize();
	}

//	Update();
}

void FrLayeredImage::SetupRenderers(){
	for (int i = 0; i<m_Layers.size(); i++){
		GetRenderWindow()->AddRenderer(m_Layers[i]->GetRenderer());
	}
}

void FrLayeredImage::RemoveRenderers(){
	for (int i = 0; i<m_Layers.size(); i++){
		GetRenderWindow()->GetRenderers()->RemoveItem(m_Layers[i]->GetRenderer());
	}
	GetRenderWindow()->GetRenderers()->InitTraversal();
}


void FrLayeredImage::AddLayer(){
	int num = m_Layers.size();
	FrLayer* layer = new FrLayer(m_RenderWindow, num);

	m_Layers.push_back(layer);
	m_ActiveLayer = num;			// new layer becomes active by default
	
	GetRenderWindow()->SetNumberOfLayers(num+1);	// renew number of layers in RenderWindow	
}

void FrLayeredImage::RemoveLayer(){
	// TODO: implement..
}

void FrLayeredImage::SetInput(vtkImageData *input){
	m_inputData = input;

	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->SetInput(input);
	}

//	Update();
}

void FrLayeredImage::SetCamera(CameraSettings& camSettings){
	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->SetCamera(camSettings);
	}
	
//	Update();
}

void FrLayeredImage::SetVisibility(bool value){
	m_Layers[m_ActiveLayer]->SetVisibility(value);
}

void FrLayeredImage::SetOpacity(float value){
	m_Layers[m_ActiveLayer]->SetOpacity(value);
}

void FrLayeredImage::SetLookupTable(vtkLookupTable* ltable){
	m_Layers[m_ActiveLayer]->SetLookupTable(ltable);
}

void FrLayeredImage::Update(){
	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->Update();
	}
}



