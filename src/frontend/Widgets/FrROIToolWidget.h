#ifndef FR_ROITOOL_WIDGET
#define FR_ROITOOL_WIDGET

#define DFN_TOOL_NUMBER 11

#include "FrMacro.h"

#include "Qt/qtoolbox.h"

class FrBaseWidget;
class FrEmptyToolWidget;
class FrSpinToolWidget;
class FrListToolWidget;


typedef FrEmptyToolWidget FrInfoWidget;
typedef FrEmptyToolWidget FrRectToolWidget;
typedef FrEmptyToolWidget FrFreeShapeToolWidget;
typedef FrEmptyToolWidget FrInvertSelectionToolWidget;
typedef FrEmptyToolWidget FrCopyToAdjucentToolWidget;

typedef FrSpinToolWidget FrPenToolWidget;
typedef FrSpinToolWidget FrSphereToolWidget;
typedef FrSpinToolWidget FrDilationErosionToolWidget;

typedef FrListToolWidget FrIntersectMaskToolWidget;
typedef FrListToolWidget FrSubtractMaskToolWidget;
typedef FrListToolWidget FrUnionMaskToolWidget;


class FrROIToolWidget: public QToolBox{
    Q_OBJECT
public:
    FrROIToolWidget(QWidget *parent);

    FrPropMacro(int, CurrentId);     

    FrGetPropMacro(FrInfoWidget*, InfoWidget);
    FrGetPropMacro(FrRectToolWidget*, RectToolWidget);
    FrGetPropMacro(FrFreeShapeToolWidget*, FreeShapeToolWidget);
    FrGetPropMacro(FrInvertSelectionToolWidget*, InvertSelectionToolWidget);
    FrGetPropMacro(FrCopyToAdjucentToolWidget*, CopyToAdjucentToolWidget);

    FrGetPropMacro(FrPenToolWidget*, PenToolWidget);
    FrGetPropMacro(FrSphereToolWidget*, SphereToolWidget);
    FrGetPropMacro(FrDilationErosionToolWidget*, DilationErosionToolWidget);

    FrGetPropMacro(FrIntersectMaskToolWidget*, IntersectMaskToolWidget);
    FrGetPropMacro(FrSubtractMaskToolWidget*, SubtractMaskToolWidget);
    FrGetPropMacro(FrUnionMaskToolWidget*, UnionMaskToolWidget);

signals:

private Q_SLOTS:
    void toolChanged(int index);

private:
    void Initialize();

private:
    FrBaseWidget* widgets[DFN_TOOL_NUMBER];

};


#endif