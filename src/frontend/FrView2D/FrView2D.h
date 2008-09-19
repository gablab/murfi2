#ifndef FR_VIEW_2D
#define FR_VIEW_2D

class QWidget;
class vtkInteractorStyle;

class vtkRenderer;
class vtkImageViewer2;
class vtkImageActor;
class vtkTextActor;
class vtkOutlineFilter;
class vtkPolyDataMapper;
class vtkFollower;

class QVTKWidget;
class FrMainWindow;
class FrTBCFilter;
class FrMosaicFilter;	// test
class Fr2DSliceActor;
class FrDocumentReader;
class FrSliceExtractor;

#include "FrBaseView.h"

// Wrapper for QVTKWidget.
class FrView2D : public FrBaseView
{ 
public:
    FrView2D(FrMainWindow* mainWindowt);
    virtual ~FrView2D();

    virtual void Initialize();
    virtual void UpdatePipeline(int point);
    virtual void SetupRenderers();
    virtual void RemoveRenderers();

    FrSliceExtractor* GetSliceExtractor() { return m_SliceExtractor; }
    FrTBCFilter* GetTBCFilter() { return m_tbcFilter; }
    vtkRenderer* GetRenderer(){ return m_renderer; }
    

private:
    // Pipeline
    FrDocumentReader* m_docReader;
    FrSliceExtractor* m_SliceExtractor;
    FrTBCFilter* m_tbcFilter;
    vtkRenderer* m_renderer;

    // Actors
	Fr2DSliceActor* m_actor;
    vtkTextActor* m_tactor;
};

#endif


