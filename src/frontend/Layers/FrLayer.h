// class combining renderer and actors for single layer display
#ifndef FR_LAYER
#define FR_LAYER

#include "Fr2DSliceActor.h"
#include "FrBaseLayer.h"
#include "FrTabSettingsDocObj.h"
#include "FrMacro.h"
#include "FrColormapFilter.h"	// test

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "vtkTextActor.h"
#include "vtkLookupTable.h"
#include "vtkImageActor.h"

#include "Qt\QColor.h"


class FrLayer : public FrBaseLayer 
{
public:
	FrLayer(LayerSettings ls, vtkRenderWindow* renWindow, int layNum = 0);
	~FrLayer();
	
	void Initialize();
	void Delete();

	void SetInput(vtkImageData* input);
	void SetCamera(CameraSettings& camSettings);

	void SetOpacity(float value);
	void SetLookupTable(vtkLookupTable* table);
	void SetVisibility(bool value);
	void SetParams(LayerSettings& lSettings);
    
	void Update();

	// slice parameters
	FrPropMacro(int, CMThreshold);	// multi colormap threshold
	FrPropMacro(int, Threshold);
	FrPropMacro(int, Brightness);
	FrPropMacro(int, Contrast);
	FrPropMacro(int, Id);			// unique id of layer

private:

private:
    // Actors
	vtkImageActor* m_actor;
	vtkImageData* m_inputData;
	FrColormapFilter* m_cmfilter;
	
	float m_Opacity;
	bool m_Visible;
	int m_PxMin;
	int m_PxMax;
	int m_CMType;
	QColor m_Color;

};

#endif