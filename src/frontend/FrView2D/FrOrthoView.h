#ifndef FR_ORTHO_VIEW
#define FR_ORTHO_VIEW

class vtkRenderer;

#include "FrBaseView.h"

#define RENDERER_COUNT 3
#define CORONAL_RENDERER 0
#define SAGITAL_RENDERER 1
#define AXIAL_RENDERER   2


// Wrapper for QVTKWidget.
class FrOrthoView : public FrBaseView
{ 
public:
    FrOrthoView(FrMainWindow* mainWindow);
    virtual ~FrOrthoView();

    virtual void Initialize();
    virtual void UpdateScene();
    virtual void SetupRenderers();
    virtual void RemoveRenderers();

private:
    vtkRenderer* m_renderer[RENDERER_COUNT];
};

#endif


