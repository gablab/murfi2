#ifndef FR_MOSAIC_VIEW
#define FR_MOSAIC_VIEW

class QWidget;
class vtkInteractorStyle;

class vtkRenderer;
class vtkImageViewer2;
class vtkImageActor;

class QVTKWidget;
class FrMainWindow;
class FrTBCFilter;

// Wrapper for QVTKWidget.
class FrMosaicView
{ 
public:
    FrMosaicView(FrMainWindow* mainWindow, QWidget* parent);
    virtual ~FrMosaicView();

    QWidget* GetWidget();
    void SetInteractorStyle(vtkInteractorStyle* style);

    void UpdateScene();
    void UpdateTBC();
	void UpdateSlice();

private:
    FrMainWindow* m_mainWindow;

    QVTKWidget* m_qtView;
    vtkRenderer* m_renderer;
    vtkImageViewer2* m_imageViewer;

    vtkImageActor* m_actor;
    FrTBCFilter* m_tbcFilter;

	int m_slice;
	int m_dims[3];
	unsigned int matrixSize, numSlices;

};

#endif


