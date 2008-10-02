// class combining renderer and actors for single layer display
#ifndef FR_LAYER
#define FR_LAYER

#include "Fr2DSliceActor.h"
#include "FrBaseLayer.h"
#include "FrTabSettingsDocObj.h"
#include "FrMacro.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "vtkTextActor.h"
#include "vtkLookupTable.h"
#include "vtkImageActor.h"


class FrLayer : public FrBaseLayer 
{
public:
	FrLayer(vtkRenderWindow* renWindow, int layNum = 0);
	~FrLayer();
	
	void Initialize();
	
	void SetInput(vtkImageData* input);
	void SetCamera(CameraSettings& camSettings);

	void SetOpacity(float value);
	void SetLookupTable(vtkLookupTable* table);
	void SetVisibility(bool value);
    
	void Update();

	// slice parameters
	FrPropMacro(int, CMThreshold);	// multi colormap threshold
	FrPropMacro(int, Threshold);
	FrPropMacro(int, Brightness);
	FrPropMacro(int, Contrast);

private:

private:
    // Actors
	vtkImageActor* m_actor;
	vtkImageData* m_inputData;
	
	float m_Opacity;

};

#endif