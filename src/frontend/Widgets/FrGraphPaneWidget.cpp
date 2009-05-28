#include "FrGraphPaneWidget.h"
#include "FrQwtPlotWidget.h"
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
#include "FrGraphPaneDocObj.h"
#include "FrDataStore.h"

// backend
#include "RtDataStore.h"
#include "RtDataID.h"
#include "RtData.h"

// Qt stuff
#include "Qt/qstring.h"
#include "Qt/qlayout.h"
#include "Qt/qpushbutton.h" 
#include "Qt/qtoolbutton.h"
#include "Qt/qthread.h"
#include "Qt/qevent.h"
#include "Qt/qlabel.h"


// Implementation of FrGraphPaneWidget
FrGraphPaneWidget::FrGraphPaneWidget(unsigned long id, QWidget* parent, FrMainDocument* doc) 
: QFrame(parent), m_Document(doc), m_Selected(false), m_ID(id) {

    //m_ID = (unsigned long)(void*)(this);
    
    // create popup menu
    m_GraphContextMenu = new FrGraphContextMenu(this);

    // create top layout
    deleteBtn = new QToolButton();
    deleteBtn->setText("x");
    
    m_QwtPlotWidget = new FrQwtPlotWidget(this);
    m_QwtPlotWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Create main layout
    QVBoxLayout* rightLayout = new QVBoxLayout(this);
    rightLayout->addWidget(deleteBtn);
    rightLayout->addWidget(m_QwtPlotWidget);

    // Connect signals
    connect(m_QwtPlotWidget, SIGNAL(markerPositionChange(int)),
        this, SLOT(OnGraphMarkerPositionChanged(int)));

    connect(this, SIGNAL(UpdateSignal()), this, SLOT(OnUpdate()));
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(OnDeleteBtnClicked()));

 //   this->setMinimumHeight(this->sizeHint().height());
    this->setMinimumHeight(150);
    //this->setFixedHeight(200);
    
//    this->setFixedWidth(this->sizeHint().width());
    //this->setFrameShape(QFrame::Box);
    this->setFrameShadow(QFrame::Plain);

//    mutex = new QMutex(QMutex::NonRecursive);
}

FrGraphPaneWidget::~FrGraphPaneWidget(){
    if (m_QwtPlotWidget) delete m_QwtPlotWidget;
//    if (mutex) delete mutex;

    disconnect(this, SIGNAL(UpdateSignal()), this, SLOT(OnUpdate()));
    disconnect(deleteBtn, SIGNAL(clicked()), this, SLOT(OnDeleteBtnClicked()));
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
    m_QwtPlotWidget->RemoveAll();
    
    if (m_Document == 0) return;
    //this->UpdateLiveMode();

    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;

    // get max number of timepoints in available timeserias
    int numberOfTimePoints = -1;
    
    // TODO: find max tp number in timeserias
    // TODO: we need to get timepoints only from current timeseria
    std::vector<FrDocumentObj*> layers;
    m_Document->GetObjectsByType(layers, FrDocumentObj::LayerObject);
    FrDataStore* dataStore = m_Document->GetDataStore();

    std::vector<FrDocumentObj*>::iterator it, itEnd(layers.end());
    for(it = layers.begin(); it != itEnd; ++it){
        FrLayerDocObj* layer = (FrLayerDocObj*)(*it);    
        RtDataID id = layer->GetSettings()->DataID;
        
        // find image
        FrImageDocObj* img = m_Document->GetImageDocObjByID(id);
        if (img){
            int num = img->GetNumberOfTimePoints();
            if (num > numberOfTimePoints)
                numberOfTimePoints = num;                
        }
    }

    if(numberOfTimePoints < 0) return;

    m_QwtPlotWidget->SetNumberOfTimePoints(
        numberOfTimePoints);
 
    // get list of graphs
    FrGraphPaneDocObj* graphSet = m_Document->GetGraphSetDocObjByID(m_ID);
    FrGraphPaneDocObj::GraphsCollection graphs;
    graphs = graphSet->GetGraphs();

    std::vector<FrGraphDocObj*>::iterator itr, itrEnd(graphs.end());
    for(itr = graphs.begin(); itr != itrEnd; ++itr){
        // add graphs to widget
        FrGraphDocObj* graphDO = (FrGraphDocObj*)(*itr); 

        QString& name = graphDO->GetSettings()->Name;
        
	    /* ohinds 2009-02-07
	     * temporary hack
	    QColor& color = graphDO->GetSettings()->Color;
	    */
	    QColor color(Qt::red);

        m_QwtPlotWidget->AddGraph(graphDO->GetID(), name, color);
        
        // TODO: get and set graph data here
        this->SetGraphData(graphDO);
    }

    m_QwtPlotWidget->replot();
    // NOTE: marker position will not be changed before replot
    m_QwtPlotWidget->SetMarkerPosition(viewDO->GetTimePoint());
    //m_QwtPlotWidget->replot();

    //QString info = QString("Current Time Point: %1").arg(viewDO->GetTimePoint());
    //m_PlayControlWidget->SetAdditionalInfo(info);

    //mutex->unlock();
}

void FrGraphPaneWidget::mousePressEvent(QMouseEvent* event){
    emit Clicked(m_ID);
}

void FrGraphPaneWidget::contextMenuEvent(QContextMenuEvent *event){
    // remove all items 
    m_GraphContextMenu->clear();

    // Get available timeserias from data store
    std::vector<FrDataStore::DataItem> data;
    FrDataStore* dataStore = m_Document->GetDataStore();
    dataStore->GetAvailableData(data);

    std::vector<FrDataStore::DataItem>::iterator it, itEnd(data.end());
    for(it = data.begin(); it != itEnd; ++it){
        // create a name for timeseria 
        char* name = (char*)(*it).ModuleID.c_str();
        char tmp[20];
        itoa((*it).TimeSeries, tmp, 10);
        strcat(name, " ");
        strcat(name, tmp);

        FrAction* action = new FrAction(QString(name), this);
        action->SetID((*it).TimeSeries);

        // check if we have graph with this timeseria ID
        FrGraphPaneDocObj* graphSet = m_Document->GetGraphSetDocObjByID(m_ID);
        FrGraphDocObj* graph = graphSet->GetGraphDocObjByTS((*it).TimeSeries);
        
        if (graph)
            action->setChecked(true);
        else
            action->setChecked(false);

        m_GraphContextMenu->addAction(action);
        connect(action, SIGNAL(actionChecked(unsigned long, bool)), this, SLOT(itemChecked(unsigned long, bool)));
    }

    m_GraphContextMenu->exec(event->globalPos());
}

void FrGraphPaneWidget::itemChecked(unsigned long id, bool checked){
    emit GraphChanged(m_ID, id, checked);
}

void FrGraphPaneWidget::SetSelected(bool selected){
    m_Selected = selected;
    this->setFrameShape(selected ? QFrame::Box : QFrame::NoFrame);
}

void FrGraphPaneWidget::OnDeleteBtnClicked(){
    emit DeleteWidget(m_ID);
}

void FrGraphPaneWidget::OnGraphMarkerPositionChanged(int position){
    emit TimePointChanged(position);
}

void FrGraphPaneWidget::SetGraphData(FrGraphDocObj* graphDO){
    // get data
    double* idata;
    int size = 0;
    
    // go through all images from specified timeseria and get values
    FrDataStore* dataStore = m_Document->GetDataStore();
    RtDataStore* dataStore2 = dataStore->GetStore();
    std::vector<RtDataID> dataIDs;
    dataStore->GetStuff(dataIDs);

    size = dataIDs.size();
    idata = new double[size];
    int i = 0;

    std::vector<RtDataID>::iterator it, itEnd(dataIDs.end());
    for(it = dataIDs.begin(); it != itEnd; ++it){
        RtDataID& dataID = (*it);
        if (dataID.getSeriesNum() == graphDO->GetTimeSeria()){
            RtData* data = dataStore2->getData(dataID);
            string type = dataID.getModuleID();

            RtDataImage<short>* img = dynamic_cast<RtDataImage<short>*>(data);

            FrGraphSettings::GraphTypes gType = graphDO->GetSettings()->GetType();
            double value = 0.0f;

            switch(gType){
                case FrGraphSettings::GT_Intencity:
                    value = this->GetIntensityValue(img, graphDO->GetSettings());
                    break;
                case FrGraphSettings::GT_Movements:
                    // not supported yet
                    break;
                case FrGraphSettings::GT_RoiMean:
                    // not supported yet
                    break;
                case FrGraphSettings::GT_RoiStd:
                    // not supported yet
                    break;
                case FrGraphSettings::GT_Stimulus:
                    // not supported yet
                    break;
            }

            idata[i] = value;
            i++;
        }
    }    
    m_QwtPlotWidget->SetData(graphDO->GetID(), idata, i);

    delete idata;
}

double FrGraphPaneWidget::GetIntensityValue(RtDataImage<short>* img, FrGraphSettings *gs){
    double value = 0.0f;

    // since we don't have voxel selection tool we can't select more than 1 point, 
    // so get point from PointsDocObj
    FrPointsDocObj* pointsDO = 0L;
    FrDocument::DocObjCollection pointObjects;
    m_Document->GetObjectsByType(pointObjects, FrDocumentObj::PointsObject);    

    if(pointObjects.size() > 0){
        pointsDO = (FrPointsDocObj*)pointObjects[0];    
        int point[3];
        pointsDO->GetPoint(point);
        if (point[0] < 0)   // incorrect point
            return 0.0f;

        //FrIntencityGraphSettings* igs = (FrIntencityGraphSettings*)gs;
        short* pImageData = img->getDataCopy();

        // get specified i,j,k value
        int width = img->getDim(0);
        int height = img->getDim(1);
        //int index = igs->K * width*height +  igs->J * width + igs->I;
        int index = point[2] * width*height +  point[1] * width + point[0];

        value = (double)pImageData[index]; 
        
        delete[] pImageData;
    }

    return value;
}

