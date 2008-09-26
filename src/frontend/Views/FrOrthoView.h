#ifndef FR_ORTHO_VIEW
#define FR_ORTHO_VIEW

class vtkRenderer;
class vtkTextActor;

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

    FrSliceExtractor* GetSliceExtractor(int i) { return m_SliceExtractor[i]; }
    FrTBCFilter* GetTBCFilter(int i) { return m_tbcFilter[i]; }
    vtkRenderer* GetRenderer(int i){ return m_renderer[i]; }
    Fr2DSliceActor* GetActor(int i){ return m_actor[i]; }

private:
    // Render pipline
    FrDocumentReader* m_docReader;
    FrSliceExtractor* m_SliceExtractor[RENDERER_COUNT];
    FrTBCFilter* m_tbcFilter[RENDERER_COUNT];
    vtkRenderer* m_renderer[RENDERER_COUNT];

    // Actors
	Fr2DSliceActor* m_actor[RENDERER_COUNT];
	vtkTextActor* m_tactor[RENDERER_COUNT];	

	int x, y;

private:
	double* GetLocalRendererPoint(vtkRenderer* ren, int x, int y);
	int GetCoronalSlice(int ren, int dx, int dy);
	int GetSagitalSlice(int ren, int dx, int dy);
	int GetAxialSlice(int ren, int dx, int dy);
	
};

#endif


