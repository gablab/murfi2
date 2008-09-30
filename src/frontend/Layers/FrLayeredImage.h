// class controlling layers
#ifndef FR_LAYERED_IMAGE
#define FR_LAYERED_IMAGE

#include "FrLayer.h"
#include "FrTabSettingsDocObj.h"
#include "FrMacro.h"

// VTK stuff
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include <vector>

typedef std::vector<FrLayer*> LayerCollection;

class FrLayeredImage{
public:
	FrLayeredImage(vtkRenderWindow* renWindow);
	~FrLayeredImage();

	void Initialize();

	void AddLayer();
	void RemoveLayer();

	void SetInput(vtkImageData* input);
	void SetCamera(CameraSettings& camSettings);
	
	FrPropMacro(int, ActiveLayer);

	// active layer actions
	void SetVisibility(bool value);
	void SetOpacity(float value);
	void SetLookupTable(vtkLookupTable* ltable);

	void SetupRenderers();
	void RemoveRenderers();

	vtkRenderWindow* GetRenderWindow(){return m_RenderWindow;};

	void Update();

private:
	vtkRenderWindow* m_RenderWindow;
	LayerCollection m_Layers;
	vtkImageData* m_inputData;

};


#endif