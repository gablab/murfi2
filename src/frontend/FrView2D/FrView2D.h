#ifndef FR_VIEW_2D
#define FR_VIEW_2D

class QWidget;
class vtkInteractorStyle;

class vtkRenderer;
class vtkImageViewer2;
class vtkImageActor;

class QVTKWidget;
class FrMainWindow;
class FrTBCFilter;

// Wrapper for QVTKWidget.
class FrView2D
{ 
public:
    FrView2D(FrMainWindow* mainWindow, QWidget* parent);
    virtual ~FrView2D();

    QWidget* GetWidget();
    void SetInteractorStyle(vtkInteractorStyle* style);

    void UpdateScene();
    void UpdateTBC();
	void UpdateSlice();

    void SetThreshold(double value);
    void SetBrightness(double value);
    void SetContrast(double value);

private:
    FrMainWindow* m_mainWindow;

    QVTKWidget* m_qtView;
    vtkRenderer* m_renderer;
    vtkImageViewer2* m_imageViewer;

    vtkImageActor* m_actor;
    FrTBCFilter* m_tbcFilter;

	int m_slice;
	int m_dims[3];

};

#endif


