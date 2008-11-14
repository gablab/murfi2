#ifndef FR_ROITOOL_WIDGET
#define FR_ROITOOL_WIDGET

// Forward declarations
class FrBaseWidget;
class FrEmptyToolWidget;
class FrSpinToolWidget;
class FrListToolWidget;
class FrSpinSliderWidget;

class QLabel;
class QComboBox;
class QGroupBox;
class QHBoxLayout;
class QStackedLayout;


#include "FrMacro.h"
#include "Qt/qwidget.h"

// Class represents widgets with all ROI tools
// together with params.
class FrROIToolWidget: public QWidget{
    Q_OBJECT
public:
    enum ToolType { 
        Info, Rectangle, FreeShape, Invert, 
        Copy, Pen, Sphere, DilateErode,
        Intersect, Subtract, Union
    };
public:
    FrROIToolWidget(QWidget *parent=0L);

    // Accessors
    ToolType GetCurrentToolType();
    int GetCurrentValue();

Q_SIGNALS:
    void CurrentToolChanged(ToolType toolType);
    void CurrentToolParamChanged(int value);

private Q_SLOTS:
    void OnToolChanged(int index);
    void OnToolParamChanged();

private:
    // helpers
    QHBoxLayout* CreateSelectionLayout();
    void InitAdditionalParams(QGroupBox* gb);
    
private:
    QLabel* m_label;
    QComboBox* m_cmbTool;
    QStackedLayout* m_layout;

    FrEmptyToolWidget* m_InfoWidget;
    FrEmptyToolWidget* m_RectangleWidget;
    FrEmptyToolWidget* m_FreeShapeWidget;
    FrSpinToolWidget* m_PenWidget;
    FrSpinToolWidget* m_SphereWidget;
    FrSpinToolWidget* m_DilateErodeWidget;
    FrEmptyToolWidget* m_InvertWidget;
    FrListToolWidget* m_IntersectWidget;
    FrListToolWidget* m_SubtractWidget;
    FrListToolWidget* m_UnionWidget;
    FrEmptyToolWidget* m_CopyWidget;
};


#endif