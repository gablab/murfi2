#include "FrROIToolWidget.h"
#include "FrBaseToolWidget.h"
#include "FrEmptyToolWidget.h"
#include "FrSpinToolWidget.h"
#include "FrListToolWidget.h"


FrROIToolWidget::FrROIToolWidget(QWidget *parent)
: QToolBox(parent){

    Initialize();    

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(toolChanged(int)));
}

void FrROIToolWidget::toolChanged(int index){
    m_CurrentId = index;
}

void FrROIToolWidget::Initialize(){
    // create tools
    m_InfoWidget = new FrInfoWidget(this);
    m_RectToolWidget = new FrRectToolWidget(this);
    m_FreeShapeToolWidget = new FrFreeShapeToolWidget(this);
    m_InvertSelectionToolWidget = new FrInvertSelectionToolWidget(this);
    m_CopyToAdjucentToolWidget = new FrCopyToAdjucentToolWidget(this);

    m_PenToolWidget = new FrPenToolWidget(this);
    m_SphereToolWidget = new FrSphereToolWidget(this);
    m_DilationErosionToolWidget = new FrDilationErosionToolWidget(this);

    m_IntersectMaskToolWidget = new FrIntersectMaskToolWidget(this);
    m_SubtractMaskToolWidget = new FrSubtractMaskToolWidget(this);
    m_UnionMaskToolWidget = new FrUnionMaskToolWidget(this);
    
    // add tools
    this->addItem(m_InfoWidget, "Info");
    this->addItem(m_PenToolWidget, "Pen");
    this->addItem(m_RectToolWidget, "Rectangle");
    this->addItem(m_FreeShapeToolWidget, "Free shape");
    this->addItem(m_SphereToolWidget, "Sphere");
    this->addItem(m_DilationErosionToolWidget, "Dilation/Erosion");
    this->addItem(m_IntersectMaskToolWidget, "Intersect two masks");
    this->addItem(m_UnionMaskToolWidget, "Union two masks");
    this->addItem(m_SubtractMaskToolWidget, "Subtruct two masks");
    this->addItem(m_InvertSelectionToolWidget, "Invert to selection");
    this->addItem(m_CopyToAdjucentToolWidget, "Copy to adjucent");

}