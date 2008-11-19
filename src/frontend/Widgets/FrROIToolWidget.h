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

    // Methods
    typedef struct _roiInfo { int ID; QString Name; } RoiInfo;
    void UpdateRoiInfo(std::vector<RoiInfo>& roiInfos);

    // Properties
    FrGetPropMacro(FrEmptyToolWidget*, InfoWidget);
    FrGetPropMacro(FrEmptyToolWidget*, RectangleWidget);
    FrGetPropMacro(FrEmptyToolWidget*, FreeShapeWidget);
    FrGetPropMacro(FrSpinToolWidget*,  PenWidget);
    FrGetPropMacro(FrSpinToolWidget*,  SphereWidget);
    FrGetPropMacro(FrSpinToolWidget*,  DilateErodeWidget);
    FrGetPropMacro(FrEmptyToolWidget*, InvertWidget);
    FrGetPropMacro(FrListToolWidget*,  IntersectWidget);
    FrGetPropMacro(FrListToolWidget*,  SubtractWidget);
    FrGetPropMacro(FrListToolWidget*,  UnionWidget);
    FrGetPropMacro(FrEmptyToolWidget*, CopyWidget);

Q_SIGNALS:    
    void CurrentToolChanged();
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
};


#endif
