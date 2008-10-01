#ifndef FR_SPECIAL_LAYER
#define FR_SPECIAL_LAYER

#include "FrBaseLayer.h"
#include "Fr2DSliceActor.h"
#include "FrTabSettingsDocObj.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "vtkTextActor.h"
#include "vtkLookupTable.h"


class FrSpecialLayer : public FrBaseLayer{
public:
	FrSpecialLayer(vtkRenderWindow* renWindow);
	~FrSpecialLayer();

	void Initialize();

	void SetInput(vtkImageData* input);
	void SetCamera(CameraSettings& camSettings);
	void SetLayer(int value);

	void Update();

private:
	Fr2DSliceActor* m_actor;
	vtkImageData* m_inputData;	

};

#endif