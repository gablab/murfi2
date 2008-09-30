// base class for all types of layers

#ifndef FR_BASE_LAYER
#define FR_BASE_LAYER

#include "FrMacro.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

class FrBaseLayer{
public:
	FrBaseLayer(vtkRenderWindow *renWindow);
	~FrBaseLayer();

	virtual void Initialize();	// overwrite this in inherited classes

	FrGetPropMacro(vtkRenderer*, Renderer);
	FrGetPropMacro(vtkRenderWindow*, RenderWindow);

private:
	virtual void Update();	// overwrite this in inherited classes

private:

};

#endif