#ifndef FR_VIEW_2D
#define FR_VIEW_2D

class QWidget;
class vtkInteractorStyle;

class vtkRenderer;
class vtkImageViewer2;
class vtkImageActor;
class vtkTextActor;

class QVTKWidget;
class FrMainWindow;
class FrTBCFilter;
class Fr2DSliceActor;
class FrDocumentReader;

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

private:
    // Pipeline
    FrDocumentReader* m_docReader;
    FrTBCFilter* m_tbcFilter;
    vtkRenderer* m_renderer;
    // Actors
    vtkImageActor* m_actor;
	Fr2DSliceActor* m_actor2;
	vtkTextActor* m_tactor;

	int m_slice;
	int m_dims[3];
	unsigned int matrixSize, numSlices;
};

#endif


