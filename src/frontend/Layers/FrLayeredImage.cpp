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

	m_defaultLayer = new FrSpecialLayer(GetRenderWindow());		// test
	this->AddLayer();	// first layer
}

FrLayeredImage::~FrLayeredImage(){
//	if(m_inputData) m_inputData->Delete();
}

void FrLayeredImage::Initialize(){
	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->Initialize();
	}
	
	m_defaultLayer->Initialize();	// test

//	Update();
}

void FrLayeredImage::SetupRenderers(){
	for (int i = 0; i<m_Layers.size(); i++){
		GetRenderWindow()->AddRenderer(m_Layers[i]->GetRenderer());
	}

	GetRenderWindow()->AddRenderer(m_defaultLayer->GetRenderer());	// test
}

void FrLayeredImage::RemoveRenderers(){
	for (int i = 0; i<m_Layers.size(); i++){
		GetRenderWindow()->GetRenderers()->RemoveItem(m_Layers[i]->GetRenderer());
	}
	
	GetRenderWindow()->GetRenderers()->RemoveItem(m_defaultLayer->GetRenderer());	// test

	GetRenderWindow()->GetRenderers()->InitTraversal();
}


void FrLayeredImage::AddLayer(){
	int num = m_Layers.size();
	FrLayer* layer = new FrLayer(GetRenderWindow(), num);

	m_Layers.push_back(layer);
	m_ActiveLayer = num;			// new layer becomes active by default
	
	this->GetRenderWindow()->SetNumberOfLayers(num+2);	// renew number of layers in RenderWindow	
	m_defaultLayer->SetLayer(num+1);
}

void FrLayeredImage::RemoveLayer(){
	// TODO: implement..
}

void FrLayeredImage::SetInput(vtkImageData *input){
	m_inputData = input;

	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->SetInput(input);
	}

	m_defaultLayer->SetInput(input);	// test

//	Update();
}

void FrLayeredImage::SetCamera(CameraSettings& camSettings){
	for (int i = 0; i<m_Layers.size(); i++){
		m_Layers[i]->SetCamera(camSettings);
	}
	
	m_defaultLayer->SetCamera(camSettings);		// test

//	Update();
}

void FrLayeredImage::SetText(const char* text){
	m_defaultLayer->SetText(text);
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

	m_defaultLayer->Update();		// test
}



