#include "FrGraphPaneWidget.h"
#include "FrQwtPlotWidget.h"
#include "FrGraphListWidget.h"
#include "FrPlayControlWidget.h"
#include "FrMainDocument.h"

// Qt stuff
#include "Qt/qstring.h"
#include "Qt/qlayout.h"
#include "Qt/qpushbutton.h" 
#include "Qt/qthread.h"

// class FrPlayThread
class FrPlayThread : public QThread {

public:
    FrPlayThread(FrQwtPlotWidget* plotWidget)
        : QThread(), m_PlotWidget(plotWidget),
          m_MaxTimePoint(1), m_tpPerSecond(1){
    }
    
    void SetParams(int maxTimePoint, int timePointPerSec){
        m_MaxTimePoint = maxTimePoint;
        m_tpPerSecond = timePointPerSec;
    }

protected: 
    virtual void run(){
        if(m_PlotWidget == 0) return;

        // In milliseconds
        unsigned long interval = 1000 / m_tpPerSecond;

        for(int timePoint = 0; 
            timePoint < m_MaxTimePoint; 
            ++timePoint){

                // Setup current time point
                m_PlotWidget->SetMarkerPosition(timePoint);
                this->msleep(interval);
        }
    }

private:
    FrQwtPlotWidget* m_PlotWidget;
    int m_MaxTimePoint;
    int m_tpPerSecond;
};

// Implementation of FrGraphPaneWidget
FrGraphPaneWidget::FrGraphPaneWidget(QWidget* parent, FrMainDocument* doc) 
: QWidget(parent), m_Document(doc){

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

    // Create play thread 
    m_PlayThread = new FrPlayThread(m_QwtPlotWidget);

    // Connect signals
    connect(m_QwtPlotWidget, SIGNAL(markerPositionChange(int)),
        this, SLOT(OnGraphMarkerPositionChanged(int)));

    // bind player widget
    connect(m_PlayControlWidget, SIGNAL(LifeModeChanged(bool)), 
            this, SLOT(OnLifeModeChanged(bool)));
    connect(m_PlayControlWidget, SIGNAL(Play()), this, SLOT(OnPlayClicked()));
    connect(m_PlayControlWidget, SIGNAL(Pause()), this, SLOT(OnPauseClicked()));
    connect(m_PlayControlWidget, SIGNAL(Reset()), this, SLOT(OnResetClicked()));
    connect(m_PlayThread, SIGNAL(finished()), this, SLOT(OnPlayFinished()));
}

void FrGraphPaneWidget::SetDocument(FrMainDocument* doc){

    if(m_Document != doc){
        m_Document = doc;
        this->Update();
    }
}

void FrGraphPaneWidget::Update(){
    
    if (m_Document == 0) return;
    
    std::vector<FrDocumentObj*> graphs;
    m_Document->GetObjectsByType(graphs, FrDocumentObj::GraphObject);

    m_GraphListWidget->RemoveAll();
    m_QwtPlotWidget->RemoveAll();
    
    // NOTE: for testing 
    QColor colors[] = { QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255) };
    for(int i=0; i < 3; ++i){

        QString& name = QString("Graph %1").arg(i);
        m_GraphListWidget->AddGraphWidget(i, name, colors[i]);
        m_QwtPlotWidget->AddGraph(i, name, colors[i]);
    }

}

void FrGraphPaneWidget::UpdateTimePoint(){
    // TODO: implement timepoint updating...
}

// Slots
void FrGraphPaneWidget::OnGraphMarkerPositionChanged(int position){

    emit TimePointChanged(position);
}

void FrGraphPaneWidget::OnLifeModeChanged(bool value){

}

void FrGraphPaneWidget::OnPlayClicked(){

}

void FrGraphPaneWidget::OnPauseClicked(){

}

void FrGraphPaneWidget::OnResetClicked(){
    
}

void FrGraphPaneWidget::OnPlayFinished(){
    // TODO: reset player controls
    m_PlayControlWidget->SetState(FrPlayControlWidget::Normal);
}
