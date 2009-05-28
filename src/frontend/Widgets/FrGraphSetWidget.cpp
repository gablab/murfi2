#include "FrGraphSetWidget.h"
#include "FrQwtPlotWidget.h"
#include "FrPlayControlWidget.h"
#include "FrGraphContextMenu.h"
#include "FrGraphBookmarkWidget.h"
#include "FrGraphPaneWidget.h"

#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrViewDocObj.h"
#include "FrGraphDocObj.h"
#include "FrLayerDocObj.h"
#include "FrPointsDocObj.h"
#include "FrAppSettingsDocObj.h"
#include "FrDataStore.h"
#include "FrAction.h"

// backend
#include "RtDataStore.h"
#include "RtDataID.h"
#include "RtMRIImage.h"

// Qt stuff
#include "Qt/qstring.h"
#include "Qt/qlayout.h"
#include "Qt/qpushbutton.h" 
#include "Qt/qthread.h"
#include "Qt/qevent.h"
#include "Qt/qboxlayout.h"
#include "Qt/qtoolbutton.h"
#include "Qt/qscrollarea.h"


// Implementation of FrGraphPaneWidget
FrGraphSetWidget::FrGraphSetWidget(QWidget* parent, FrMainDocument* doc) 
: QWidget(parent), m_Document(doc){
        
    // Setup playback values
    m_TimerID = -1;
    m_IsPaused = false;
    m_PlayingTimePoint = 0;

    scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);

    QWidget* container = new QWidget(scrollArea);     // container for all graph widgets
    scrollArea->setWidget(container);

    graphsLayout = new QVBoxLayout(container);

    // horizontal layout
    QHBoxLayout* layout = new QHBoxLayout();
    
    // add up and down arrow buttons
    m_btnUpArrow = new QToolButton();
    m_btnUpArrow->setToolTip("Move layer up");
    m_btnUpArrow->setArrowType(Qt::UpArrow);

    m_btnDownArrow = new QToolButton();
    m_btnDownArrow->setToolTip("Move layer down");
    m_btnDownArrow->setArrowType(Qt::DownArrow);
    
    // setup layout for arrow buttons
	QSpacerItem *spacerItem1;
	spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	QSpacerItem *spacerItem2;
	spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QVBoxLayout* arrowsLayout = new QVBoxLayout();
    arrowsLayout->addItem(spacerItem1);
    arrowsLayout->addWidget(m_btnUpArrow);
    arrowsLayout->addWidget(m_btnDownArrow);
    arrowsLayout->addItem(spacerItem2);

    layout->addLayout(arrowsLayout);
    layout->addWidget(scrollArea);
    //layout->addLayout(graphsLayout);

    // setup main layout
    QPushButton* addGraphPanelBtn = new QPushButton(tr("Add Graph Panel"));
    m_PlayControlWidget = new FrPlayControlWidget(this);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(addGraphPanelBtn);
    bottomLayout->addWidget(m_PlayControlWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(bottomLayout);

    this->setMinimumHeight(this->sizeHint().height());
    //this->setFixedWidth(this->sizeHint().width());

    m_Graphs.clear();

    // connect signals
    connect(addGraphPanelBtn, SIGNAL(clicked()), this, SLOT(OnAddGraphWidgetBtnClicked()));  // test

    connect(m_btnUpArrow, SIGNAL(clicked()), this, SLOT(OnUpArrowClicked()));
    connect(m_btnDownArrow, SIGNAL(clicked()), this, SLOT(OnDownArrowClicked()));
    connect(this, SIGNAL(UpdateSignal()), this, SLOT(OnUpdate()));

    // bind player widget
    connect(m_PlayControlWidget, SIGNAL(LiveModeChanged(bool)), 
            this, SLOT(OnLiveModeChanged(bool)));
    connect(m_PlayControlWidget, SIGNAL(Play()), this, SLOT(OnPlayClicked()));
    connect(m_PlayControlWidget, SIGNAL(Pause()), this, SLOT(OnPauseClicked()));
    connect(m_PlayControlWidget, SIGNAL(Reset()), this, SLOT(OnResetClicked()));
    connect(m_PlayControlWidget, SIGNAL(Previous()), this, SLOT(OnPreviousClicked()));
    connect(m_PlayControlWidget, SIGNAL(Next()), this, SLOT(OnNextClicked()));
}

FrGraphSetWidget::~FrGraphSetWidget(){
    m_Graphs.clear();

    // disconnect signals
}

void FrGraphSetWidget::AddGraphWidget(unsigned long id){
    FrGraphPaneWidget* widget = new FrGraphPaneWidget(id, scrollArea, m_Document);
    widget->SetPosition(m_Graphs.size());

    m_Graphs.push_back(widget);
    graphsLayout->addWidget(widget);    // add widget to the end of graph layout

    // connect signals
    connect(widget, SIGNAL(Clicked(unsigned long)), this, SLOT(OnWidgetSelect(unsigned long)));
    connect(widget, SIGNAL(DeleteWidget(unsigned long)), this, SLOT(OnWidgetDelete(unsigned long)));
    connect(widget, SIGNAL(GraphChanged(unsigned long, unsigned long, bool)), this, SLOT(OnGraphChanged(unsigned long, unsigned long, bool)));
    connect(widget, SIGNAL(TimePointChanged(int)), this, SLOT(OnTimePointChanged(int)));
}

void FrGraphSetWidget::RemoveGraphWidget(unsigned long id){
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        if ((*it)->GetID() == id) break;
    }    

    if(it != itEnd){
        FrGraphPaneWidget* widget = (*it);
        m_Graphs.erase(it);

        graphsLayout->removeWidget(widget);

        // disconnect signals
        disconnect(widget, SIGNAL(Clicked(unsigned long)), this, SLOT(OnWidgetSelect(unsigned long)));
        disconnect(widget, SIGNAL(DeleteWidget(unsigned long)), this, SLOT(OnWidgetDelete(unsigned long)));
        disconnect(widget, SIGNAL(GraphChanged(unsigned long, unsigned long, bool)), this, SLOT(OnGraphChanged(unsigned long, unsigned long, bool)));
        
        // we should set proper positions to other graphs now
        unsigned long removedPos = widget->GetPosition();
        
        GraphsCollection::iterator it, itEnd(m_Graphs.end());
        for(it = m_Graphs.begin(); it != itEnd; ++it){
            unsigned long pos = (*it)->GetPosition();
            if (pos > removedPos)
                (*it)->SetPosition(pos-1);
        }    

        //delete widget;
        widget->deleteLater();
    }
}

void FrGraphSetWidget::SetDocument(FrMainDocument* doc){
    if(m_Document != doc){
        m_Document = doc;
        this->Update();
    }
}

// HACK: signal will be emited to main thread
void FrGraphSetWidget::Update(){
    emit UpdateSignal();
}

void FrGraphSetWidget::OnUpdate(){
    // update all graphs
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        (*it)->Update();
    }    
}

void FrGraphSetWidget::OnAddGraphWidgetBtnClicked(){
    emit NewGraphWidget();
    //this->AddGraphWidget(111);
}

void FrGraphSetWidget::OnWidgetSelect(unsigned long id){
    // deselect all other widgets
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        if ((*it)->GetID() == id)
            (*it)->SetSelected(true);
        else 
            (*it)->SetSelected(false);
    } 
}

void FrGraphSetWidget::OnWidgetDelete(unsigned long id){
    emit DeleteGraphWidget(id);
    //this->RemoveGraphWidget(id);
}

void FrGraphSetWidget::OnTimePointChanged(int timePoint){
    QString info = QString("Current Time Point: %1").arg(timePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);

    emit TimePointChanged(timePoint);    
}

void FrGraphSetWidget::OnUpArrowClicked(){
    this->ChangeGraphPosition(-1);
}

void FrGraphSetWidget::OnDownArrowClicked(){
    this->ChangeGraphPosition(1);
}

void FrGraphSetWidget::ChangeGraphPosition(int inc){
    // find selected graph
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        if((*it)->GetSelected())
            break;
    }    

    if(it != itEnd){
        FrGraphPaneWidget* widget = (*it);
        int oldId = widget->GetPosition();
        int newId = oldId + inc;

        // find graph with new id
        if (newId >=0 && newId < m_Graphs.size()){
            FrGraphPaneWidget* widget2 = this->GetGraphByPos(newId);
            widget2->SetPosition(oldId);
            widget->SetPosition(newId);

            graphsLayout->removeWidget(widget);
            graphsLayout->insertWidget(newId, widget);
        }
    }
}

FrGraphPaneWidget* FrGraphSetWidget::GetGraphById(unsigned long id){
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        if((*it)->GetID() == id)
            return (*it);
    }    

    return 0;
}

FrGraphPaneWidget* FrGraphSetWidget::GetGraphByPos(unsigned long pos){
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        if((*it)->GetPosition() == pos)
            return (*it);
    }    

    return 0;
}

void FrGraphSetWidget::OnGraphChanged(unsigned long gID, unsigned long tsID, bool add){
    emit GraphChanged(gID, tsID, add);
}

//NOTE: MAY BE HAVE TO BE REMOVED
//void FrGraphSetWidget::UpdateTimePoint(){
//    // Check
//    if(m_Document == 0) return;
//
//    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
//    if(viewDO == 0) return;
//    
//    int timePoint = int(viewDO->GetTimePoint());
//    if(timePoint > m_QwtPlotWidget->GetMaxTimePoint()){
//        m_QwtPlotWidget->SetNumberOfTimePoints(timePoint + 1);
//    }
//
//    m_QwtPlotWidget->SetMarkerPosition(timePoint, true);
//
//    QString info = QString("Current Time Point: %1").arg(timePoint);
//    m_PlayControlWidget->SetAdditionalInfo(info);
//}

void FrGraphSetWidget::UpdateLiveMode(){
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

// Player interface
void FrGraphSetWidget::timerEvent(QTimerEvent *event){
    // Move here forward by timepoints
    m_PlayingTimePoint++;

    // oops! have to finish playback
    if(m_PlayingTimePoint > this->GetMaxTimePoint()){
        this->OnPlayFinished();
        //m_PlayingTimePoint = 0;
        return;
    }

    // set marker position for all graphs
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        (*it)->GetQwtPlotWidget()->SetMarkerPosition(m_PlayingTimePoint);
    }   

    QString info = QString("Current Time Point: %1").arg(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);
}

void FrGraphSetWidget::OnLiveModeChanged(bool value){
    emit LiveModeChanged(value);
}

void FrGraphSetWidget::OnPlayClicked(){
    // create new timer
    if(!m_IsPaused){
        m_PlayingTimePoint = 0;
    }

    int tpPerSecond = m_PlayControlWidget->GetTimePointPerSecond();
    int interval = 1000 / tpPerSecond;

    m_TimerID = this->startTimer(interval);
    m_IsPaused = false;
    
    QString info = QString("Current Time Point: %1").arg(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);
}

void FrGraphSetWidget::OnPauseClicked(){
    // Check
    if(m_TimerID == -1) return;

    this->killTimer(m_TimerID);
    m_TimerID = -1;
    m_IsPaused = true;
}

void FrGraphSetWidget::OnResetClicked(){
    //this->OnPlayFinished();

    //m_PlayingTimePoint = 0;
    //m_QwtPlotWidget->SetMarkerPosition(m_PlayingTimePoint);
}

void FrGraphSetWidget::OnPlayFinished(){
    // Check
    if(m_TimerID != -1){
        this->killTimer(m_TimerID);
    }

    m_TimerID = -1;
    m_IsPaused = false;
    //m_PlayingTimePoint = 0;
    
    m_PlayControlWidget->SetState(FrPlayControlWidget::Normal);
}

void FrGraphSetWidget::OnPreviousClicked(){
    m_PlayingTimePoint--;
    if(m_PlayingTimePoint < 0) 
        m_PlayingTimePoint = 0;

    QString info = QString("Current Time Point: %1").arg(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);

    // set marker position for all graphs
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        (*it)->GetQwtPlotWidget()->SetMarkerPosition(m_PlayingTimePoint);
    }   

    emit PreviousTimePoint();
}

void FrGraphSetWidget::OnNextClicked(){
    m_PlayingTimePoint++;
    if(m_PlayingTimePoint > this->GetMaxTimePoint()) 
        m_PlayingTimePoint = this->GetMaxTimePoint();

    QString info = QString("Current Time Point: %1").arg(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);

    // set marker position for all graphs
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        (*it)->GetQwtPlotWidget()->SetMarkerPosition(m_PlayingTimePoint);
    }   

    emit NextTimePoint();
}

int FrGraphSetWidget::GetMaxTimePoint(){
    int maxTP = 0;
    GraphsCollection::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        int tp = (*it)->GetQwtPlotWidget()->GetMaxTimePoint();
        if (tp > maxTP)
            maxTP = tp;
    }   
    
    return maxTP;
}