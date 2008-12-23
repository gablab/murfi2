#include "FrGraphPaneWidget.h"
#include "FrQwtPlotWidget.h"
#include "FrGraphListWidget.h"
#include "FrPlayControlWidget.h"

#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrViewDocObj.h"
#include "FrGraphDocObj.h"
#include "FrAppSettingsDocObj.h"

#include "RtDataID.h"

// Qt stuff
#include "Qt/qstring.h"
#include "Qt/qlayout.h"
#include "Qt/qpushbutton.h" 
#include "Qt/qthread.h"

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

    // Setup playback values
    m_TimerID = -1;
    m_IsPaused = false;
    m_PlayingTimePoint = 0;
        
    // Connect signals
    connect(m_QwtPlotWidget, SIGNAL(markerPositionChange(int)),
        this, SLOT(OnGraphMarkerPositionChanged(int)));

    // bind player widget
    connect(m_PlayControlWidget, SIGNAL(LiveModeChanged(bool)), 
            this, SLOT(OnLiveModeChanged(bool)));
    connect(m_PlayControlWidget, SIGNAL(Play()), this, SLOT(OnPlayClicked()));
    connect(m_PlayControlWidget, SIGNAL(Pause()), this, SLOT(OnPauseClicked()));
    connect(m_PlayControlWidget, SIGNAL(Reset()), this, SLOT(OnResetClicked()));
    
}

void FrGraphPaneWidget::SetDocument(FrMainDocument* doc){

    if(m_Document != doc){
        m_Document = doc;
        this->Update();
    }
}

void FrGraphPaneWidget::Update(){
    // Clear all
    m_GraphListWidget->RemoveAll();
    m_QwtPlotWidget->RemoveAll();
    
    if (m_Document == 0) return;
    this->UpdateLiveMode();
    
    // Get data from document
    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;

    int numberOfTimePoints = -1;

    std::vector<FrDocumentObj*> images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){

        FrImageDocObj* img = (FrImageDocObj*)(*it);    
        if(img->GetSeriesNumber() == viewDO->GetTimeSeries()){

            numberOfTimePoints = 
                img->GetNumberOfTimePoints();
            break;
        }
    }
    if(numberOfTimePoints < 0) return;

    m_QwtPlotWidget->SetNumberOfTimePoints(
        numberOfTimePoints);

    std::vector<FrDocumentObj*> graphs;
    m_Document->GetObjectsByType(graphs, FrDocumentObj::GraphObject);
    int id = 0;
    for(it = images.begin(); it != itEnd; ++it){
        
        FrGraphDocObj* gr = (FrGraphDocObj*)(*it); 

        int graphID = id++;
        QString& name = gr->GetSettings()->Name;
        QColor& color = gr->GetSettings()->Color;
        bool visibility = gr->GetSettings()->Visibility;

        m_GraphListWidget->AddGraphWidget(graphID, name, color, visibility);
        m_QwtPlotWidget->AddGraph(graphID, name, color);
        m_QwtPlotWidget->SetVisibility(graphID, visibility);
    }
}


//NOTE: MAY BE HAVE TO BE REMOVED
void FrGraphPaneWidget::UpdateTimePoint(){
    // Check
    if(m_Document == 0) return;

    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;
    
    int timePoint = int(viewDO->GetTimePoint());
    if(timePoint > m_QwtPlotWidget->GetMaxTimePoint()){

        m_QwtPlotWidget->SetNumberOfTimePoints(timePoint + 1);
    }

    m_QwtPlotWidget->
        SetMarkerPosition(
        timePoint, true);
}

void FrGraphPaneWidget::UpdateLiveMode(){
    // Check
    if(m_Document == 0 ||
       m_Document->GetAppSettings() == 0) return;

    if(m_Document->GetAppSettings()->GetLiveMode()){

        m_PlayControlWidget->SetState(FrPlayControlWidget::LiveMode);
    }
    else {

        m_PlayControlWidget->SetState(FrPlayControlWidget::Normal);
    }
}

// Slots
void FrGraphPaneWidget::OnGraphMarkerPositionChanged(int position){

    emit TimePointChanged(position);
}

// Player interface
void FrGraphPaneWidget::timerEvent(QTimerEvent *event){
    // Move here farward by timepoints
    m_PlayingTimePoint++;

    // oops! have to finish playback
    if(m_PlayingTimePoint > 
       m_QwtPlotWidget->GetMaxTimePoint()){

        this->OnPlayFinished();
        //m_PlayingTimePoint = 0;
    }
    m_QwtPlotWidget->SetMarkerPosition(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(
        QString("Current Time Point: %1")
        .arg(m_PlayingTimePoint));
}

void FrGraphPaneWidget::OnLiveModeChanged(bool value){
    
    emit LiveModeChanged(value);
}

void FrGraphPaneWidget::OnPlayClicked(){
    // create new timer
    if(!m_IsPaused){
        m_PlayingTimePoint = 0;
    }

    int tpPerSecond = m_PlayControlWidget->GetTimePointPerSecond();
    int interval = 1000 / tpPerSecond;
    
    m_TimerID = this->startTimer(interval);
    m_IsPaused = false;

    m_PlayControlWidget->SetAdditionalInfo(
        QString("Current Time Point: %1")
        .arg(m_PlayingTimePoint));
}

void FrGraphPaneWidget::OnPauseClicked(){
    // Check
    if(m_TimerID == -1) return;

    this->killTimer(m_TimerID);
    m_TimerID = -1;
    m_IsPaused = true;
}

void FrGraphPaneWidget::OnResetClicked(){
    this->OnPlayFinished();

    m_PlayingTimePoint = 0;
    m_QwtPlotWidget->SetMarkerPosition(m_PlayingTimePoint);
}

void FrGraphPaneWidget::OnPlayFinished(){
    // Check
    if(m_TimerID != -1){

        this->killTimer(m_TimerID);
    }

    m_TimerID = -1;
    m_IsPaused = false;
    //m_PlayingTimePoint = 0;
    
    m_PlayControlWidget->SetState(FrPlayControlWidget::Normal);
}
