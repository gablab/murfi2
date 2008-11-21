#include "FrGraphPaneWidget.h"
#include "FrQwtPlotWidget.h"

// Qt stuff
#include "Qt/qtoolbox.h"
#include "Qt/qlayout.h"


FrGraphPaneWidget::FrGraphPaneWidget(QWidget* parent) 
: QWidget(parent){
    m_QwtPlotWidget = new FrQwtPlotWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_QwtPlotWidget);
}