#include "FrGraphPaneWidget.h"
#include "FrQwtPlotWidget.h"
#include "FrGraphListWidget.h"
#include "FrPlayControlWidget.h"

// Qt stuff
#include "Qt/qstring.h"
#include "Qt/qlayout.h"
#include "Qt/qpushbutton.h" 


FrGraphPaneWidget::FrGraphPaneWidget(QWidget* parent) 
: QWidget(parent){
    // Create left layout
    m_GraphListWidget = new FrGraphListWidget(this);
    QPushButton* btnAddGraph = new QPushButton(QString("Add Graph..."), this);
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_GraphListWidget);
    leftLayout->addWidget(btnAddGraph);

    // Create right layout
    m_QwtPlotWidget = new FrQwtPlotWidget(this);
    m_PlayControlWidget = new FrPlayControlWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(m_QwtPlotWidget);
    rightLayout->addWidget(m_PlayControlWidget);

    // Setup main layout
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
}