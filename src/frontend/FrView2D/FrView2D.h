#ifndef FR_VIEW_2D
#define FR_VIEW_2D

class QWidget;
class vtkInteractorStyle;

class QVTKWidget;

// Wrapper for QVTKWidget.
class FrView2D
{ 
public:
    FrView2D(QWidget* parent);
    virtual ~FrView2D();

    QWidget* GetWidget();
    void SetInteractorStyle(vtkInteractorStyle* style);

private:
    QVTKWidget* m_view;
};

#endif


