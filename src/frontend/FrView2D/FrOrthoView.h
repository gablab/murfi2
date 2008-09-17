#ifndef FR_ORTHO_VIEW
#define FR_ORTHO_VIEW

class vtkRenderer;
class vtkImageActor;

class FrMainWindow;
class FrDocumentReader;
class FrTBCFilter;
class FrSliceExtractor;
class Fr2DSliceActor;

#include "FrBaseView.h"

#define RENDERER_COUNT   4
#define CORONAL_RENDERER 0
#define SAGITAL_RENDERER 1
#define AXIAL_RENDERER   2
#define DUMMY_RENDERER   3


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
    FrTBCFilter* m_tbcFilter[RENDERER_COUNT];    
    vtkImageActor* m_actor;

	Fr2DSliceActor* m_actor2[RENDERER_COUNT];
	FrSliceExtractor* m_SliceExtractor[RENDERER_COUNT];

	int x, y;

private:
	double* GetLocalRendererPoint(vtkRenderer* ren, int x, int y);
	
};

#endif


