#include "FrGraphPaneWidget.h"
#include "FrQwtPlotWidget.h"
#include "FrGraphListWidget.h"
#include "FrPlayControlWidget.h"
#include "FrGraphContextMenu.h"
#include "FrGraphBookmarkWidget.h"

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

// Implementation of FrGraphPaneWidget
FrGraphPaneWidget::FrGraphPaneWidget(QWidget* parent, FrMainDocument* doc) 
: QWidget(parent), m_Document(doc){

    // test, create popup menu
    m_GraphContextMenu = new FrGraphContextMenu(this);
    
    // Create left layout
    //m_GraphListWidget = new FrGraphListWidget(this);
    //QPushButton* btnAddGraph = new QPushButton(QString("Add Graph..."), this);
    //QVBoxLayout* leftLayout = new QVBoxLayout();
    //leftLayout->addWidget(m_GraphListWidget);
    //leftLayout->addWidget(btnAddGraph);

    // Create main layout
    m_GraphBookmarkWidget = new FrGraphBookmarkWidget(this);
    m_QwtPlotWidget = new FrQwtPlotWidget(this);
    m_PlayControlWidget = new FrPlayControlWidget(this);
    
    QVBoxLayout* rightLayout = new QVBoxLayout(this);
    rightLayout->addWidget(m_GraphBookmarkWidget);
    rightLayout->addWidget(m_QwtPlotWidget);
    rightLayout->addWidget(m_PlayControlWidget);

    //// Setup main layout
    //QHBoxLayout* mainLayout = new QHBoxLayout(this);
    //mainLayout->addLayout(leftLayout);
    //mainLayout->addLayout(rightLayout);

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
    connect(m_PlayControlWidget, SIGNAL(Previous()), this, SLOT(OnPreviousClicked()));
    connect(m_PlayControlWidget, SIGNAL(Next()), this, SLOT(OnNextClicked()));

    connect(this, SIGNAL(UpdateSignal()), this, SLOT(OnUpdate()));

    this->setMinimumHeight(350);//this->sizeHint().height());
    //this->setFixedWidth(this->sizeHint().width());
}

FrGraphPaneWidget::~FrGraphPaneWidget(){
    if (m_QwtPlotWidget) delete m_QwtPlotWidget;
}

void FrGraphPaneWidget::SetDocument(FrMainDocument* doc){

    if(m_Document != doc){
        m_Document = doc;
        this->Update();
    }
}

// HACK: signal will be emited to main thread
void FrGraphPaneWidget::Update(){
    emit UpdateSignal();
}

void FrGraphPaneWidget::OnUpdate(){
    // Clear all
    //m_GraphListWidget->RemoveAll();
    m_QwtPlotWidget->RemoveAll();
    
    if (m_Document == 0) return;
    this->UpdateLiveMode();
    
    // Get data from document
    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;

    int numberOfTimePoints = -1;

    // TODO: we need to get timepoints only from current timeseria
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

    mutex.lock();

    itEnd = graphs.end();
    int id = 0;
    for(it = graphs.begin(); it != itEnd; ++it){
        FrGraphDocObj* graphDO = (FrGraphDocObj*)(*it); 

        //int graphID = id++;
        QString& name = graphDO->GetSettings()->Name;
        QColor& color = graphDO->GetSettings()->Color;
        bool visibility = graphDO->GetSettings()->Visibility;

        //m_GraphListWidget->AddGraphWidget(graphID, name, color, visibility);
        m_QwtPlotWidget->AddGraph(graphDO->GetID(), name, color);
        
        // TODO: get and set graph data here
        FrGraphSettings::GraphTypes type = graphDO->GetSettings()->GetType();

        switch(type){
            case FrGraphSettings::GT_Intencity:
                SetIntencityData(graphDO);            
                break;
            case FrGraphSettings::GT_Movements:
                // not supported yet
                return; 
                break;
            case FrGraphSettings::GT_RoiMean:
                // not supported yet
                return; 
                break;
            case FrGraphSettings::GT_RoiStd:
                // not supported yet
                return; 
                break;
            case FrGraphSettings::GT_Stimulus:
                // not supported yet
                return; 
                break;
        }

        //m_QwtPlotWidget->SetVisibility(graphID, visibility);
    }

    m_QwtPlotWidget->replot();
    // NOTE: marker position will not be changed before replot
    m_QwtPlotWidget->SetMarkerPosition(viewDO->GetTimePoint());

    QString info = QString("Current Time Point: %1").arg(viewDO->GetTimePoint());
    m_PlayControlWidget->SetAdditionalInfo(info);

    mutex.unlock();
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
    QString info = QString("Current Time Point: %1").arg(position);
    m_PlayControlWidget->SetAdditionalInfo(info);

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
        return;
    }
    m_QwtPlotWidget->SetMarkerPosition(m_PlayingTimePoint);

    QString info = QString("Current Time Point: %1").arg(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);
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
    
    QString info = QString("Current Time Point: %1").arg(m_PlayingTimePoint);
    m_PlayControlWidget->SetAdditionalInfo(info);
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

void FrGraphPaneWidget::OnPreviousClicked(){
    emit PreviousTimePoint();
}

void FrGraphPaneWidget::OnNextClicked(){
    emit NextTimePoint();
}

void FrGraphPaneWidget::contextMenuEvent(QContextMenuEvent *event){
    // TODO: build context menu here
    // remove all items and find all added timeseries and timeseries that we can add
    
    // remove all items 
    m_GraphContextMenu->clear();

    // get all images (each image = timeseria)
    std::vector<FrDocumentObj*> images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
    
    // get all timeseries, check if every timeseria has graphs or no
    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){
        FrImageDocObj* img = (FrImageDocObj*)(*it); 
        
        FrAction* action = new FrAction("mri timeseria", this);
        action->SetID(img->GetSeriesNumber());
        // TODO: check if we have graph with this timeseria ID
        FrGraphDocObj* graph = m_Document->GetGraphDocObjByID(img->GetSeriesNumber());
        
        if (graph)
            action->setChecked(true);
        else
            action->setChecked(false);

        m_GraphContextMenu->addAction(action);
        connect(action, SIGNAL(actionChecked(int, bool)), this, SLOT(itemChecked(int, bool)));
    }

    // check if current layer is roi, if yes add items : ROI Mean, ROI STD
    // Get selected layer ID 
    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;

    int layerID = viewDO->GetActiveLayerID();
    FrLayerDocObj* currentLayer = m_Document->GetLayerDocObjByID(layerID);
    if (currentLayer == 0) return;

    if (currentLayer->IsRoi()){
        m_GraphContextMenu->addSeparator(); 
        QAction* test1 = new QAction("ROI Mean", this);
        m_GraphContextMenu->addAction(test1);
        QAction* test2 = new QAction("ROI STD", this);
        m_GraphContextMenu->addAction(test2);

        // TODO: check if we have this graphs
        test1->setCheckable(true);  
        test1->setChecked(false);
        test2->setCheckable(true);
        test2->setChecked(false);

        connect(test1, SIGNAL(toggled(bool)), this, SLOT(ShowRoiMean(bool)));
        connect(test2, SIGNAL(toggled(bool)), this, SLOT(ShowRoiSTD(bool)));
    }

    m_GraphContextMenu->exec(event->globalPos());
}

void FrGraphPaneWidget::itemChecked(int id, bool checked){
    emit GraphChanged(id, checked);
}

void FrGraphPaneWidget::ShowRoiMean(bool checked){
    // TODO: set unique ID for RoiMean
    emit GraphChanged(1, checked);
}

void FrGraphPaneWidget::ShowRoiSTD(bool checked){
    // TODO: set unique ID for RoiSTD
    emit GraphChanged(2, checked);
}

void FrGraphPaneWidget::SetIntencityData(FrGraphDocObj* graphDO){
    FrIntencityGraphSettings* gs = (FrIntencityGraphSettings*)graphDO->GetSettings();

    // get data
    double* idata;
    int size = 0;
    
    // go through all images from timeseria (later timeseria with specified id) and get values
    FrDataStore* dataStore = m_Document->GetDataStore();
    RtDataStore* dataStore2 = dataStore->GetStore();
    std::vector<RtDataID> dataIDs;
    dataStore->GetStuff(dataIDs);

    size = dataIDs.size();
    idata = new double[size];
    int i = 0;

    short* pImageData = 0;
    std::vector<RtDataID>::iterator it, itEnd(dataIDs.end());
    // TODO: do it for all points in PointsDocObj
    for(it = dataIDs.begin(); it != itEnd; ++it){
        RtDataID& dataID = (*it);
        
        RtData* data = dataStore2->getData(dataID);
        string type = dataID.getModuleID();
        if (type == "mri"){
            RtMRIImage* img = dynamic_cast<RtMRIImage*>(data);
            pImageData = img->getDataCopy();
            
            // get specified i,j,k value
            int width = img->getDim(0);
            int height = img->getDim(1);
            int index = gs->K * width*height +  gs->J * width + gs->I;
            idata[i] = (double)pImageData[index]; 
            i++;
        }
        // NOTE: another types can be supported
    }    
    m_QwtPlotWidget->SetData(graphDO->GetID(), idata, i);

    delete[] pImageData;
    delete idata;
}