// class combining renderer and actors for single layer display
#ifndef FR_LAYER
#define FR_LAYER

#include "Fr2DSliceActor.h"
#include "FrBaseLayer.h"
#include "FrTabSettingsDocObj.h"

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

private:
    // Actors
//	Fr2DSliceActor* m_defaultActor;
	
	vtkImageActor* m_actor;
	vtkImageData* m_inputData;
	float m_Opacity;

};

#endif