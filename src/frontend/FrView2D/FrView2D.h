#ifndef FR_VIEW_2D
#define FR_VIEW_2D

class QWidget;
class vtkInteractorStyle;

class vtkRenderer;
class vtkImageViewer2;
class vtkImageActor;
class vtkActor;
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

private:
    // Pipeline
    FrDocumentReader* m_docReader;
    FrTBCFilter* m_tbcFilter;
	FrMosaicFilter* m_MosaicFilter; /// test
	FrSliceExtractor* m_SliceExtractor;

    vtkRenderer* m_renderer;
    // Actors
    vtkImageActor* m_actor;
	Fr2DSliceActor* m_actor2;
	vtkTextActor* m_tactor;

	int m_slice;
	int m_dims[3];
	unsigned int matrixSize, numSlices;
	int maxSliceNumber;
};

#endif


