#ifndef FR_ORTHO_VIEW
#define FR_ORTHO_VIEW

class vtkRenderer;
class vtkImageActor;

class FrMainWindow;
class FrDocumentReader;
class FrTBCFilter;

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
    virtual void UpdatePipeline(int point);
    virtual void SetupRenderers();
    virtual void RemoveRenderers();

private:
    vtkRenderer* m_renderer[RENDERER_COUNT];
    FrDocumentReader* m_docReader;
    FrTBCFilter* m_tbcFilter;    
    vtkImageActor* m_actor;
};

#endif


