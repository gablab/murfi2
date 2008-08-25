#ifndef FR_VIEW_2D
#define FR_VIEW_2D

class QWidget;
class vtkInteractorStyle;

class vtkRenderer;
class vtkImageViewer2;

class QVTKWidget;
class FrMainWindow;

// Wrapper for QVTKWidget.
class FrView2D
{ 
public:
    FrView2D(FrMainWindow* mainWindow, QWidget* parent);
    virtual ~FrView2D();

    QWidget* GetWidget();
    void SetInteractorStyle(vtkInteractorStyle* style);

    void UpdateScene();

private:
    FrMainWindow* m_mainWindow;

    QVTKWidget* m_qtView;
    vtkRenderer* m_renderer;
    vtkImageViewer2* m_imageViewer;
};

#endif


