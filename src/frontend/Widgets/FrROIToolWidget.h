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
class QPushButton;


#include "FrMacro.h"
#include "Qt/qwidget.h"
#include <vector>

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
    typedef struct _roiInfo { unsigned long ID; QString Name; } RoiInfo;
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
    void ClearCurrentRoiAction();

private Q_SLOTS:
    void OnToolChanged(int index);
    void OnToolParamChanged();
    void OnClearBtnPressed();

private:
    // helpers
    QHBoxLayout* CreateSelectionLayout();
    void InitAdditionalParams(QGroupBox* gb);
    
private:
    QLabel* m_label;
    QComboBox* m_cmbTool;
    QPushButton* m_clearBtn;
    QStackedLayout* m_layout;
};


#endif
