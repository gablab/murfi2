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

// Wrapper for QVTKWidget.
class FrView2D
{ 
public:
    FrView2D(FrMainWindow* mainWindow, QWidget* parent);
    virtual ~FrView2D();

    QWidget* GetWidget();
    void SetInteractorStyle(vtkInteractorStyle* style);

    void UpdateScene();
    void UpdateTCB();
	void UpdateSlice();

private:
    FrMainWindow* m_mainWindow;

    QVTKWidget* m_qtView;
    vtkRenderer* m_renderer;
    vtkImageViewer2* m_imageViewer;

    vtkImageActor* m_actor;
	Fr2DSliceActor* m_actor2;

	vtkTextActor* m_tactor;
    FrTBCFilter* m_tbcFilter;

	int m_slice;
	int m_dims[3];
	unsigned int matrixSize, numSlices;

};

#endif


