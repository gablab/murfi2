#include "FrActionSignalManager.h"
#include "FrActionManager.h"
#include "FrMainWindow.h"
#include "FrMainController.h"
#include "FrBookmarkWidget.h"
#include "FrGraphBookmarkWidget.h"
#include "FrLayerListWidget.h"
#include "FrGraphPaneWidget.h"
#include "FrImageSettingsWidget.h"
#include "FrSettings.h"

// Qt
#include "Qt/qstring.h"
#include "Qt/qaction.h"
#include "Qt/qapplication.h"
#include "Qt/qmessagebox.h"
#include "Qt/qfiledialog.h"


#define CONNECT_ACTION_TRIGGERED(action, slot)\
    connect(action,SIGNAL(triggered()),this,SLOT(slot))
#define DISCONNECT_ACTION_TRIGGERED(action, slot)\
    disconnect(action,SIGNAL(triggered()),this,SLOT(slot)) 
    

FrActionSignalManager::FrActionSignalManager(FrMainWindow* mainWindow)
: QObject(mainWindow), m_mainWindow(mainWindow), m_isInit(false) {
    this->Initialize();
}

FrActionSignalManager::~FrActionSignalManager(){
    this->Deinitialize();
}

void FrActionSignalManager::Initialize(){
    if(m_isInit) return;

    // Connect signals
    FrActionManager* am = m_mainWindow->m_ActionManager;

    // File 
    CONNECT_ACTION_TRIGGERED(am->GetOpenImageAction(), OnOpenImageAction());
    CONNECT_ACTION_TRIGGERED(am->GetOpenConfigAction(), OnOpenConfigAction());
    CONNECT_ACTION_TRIGGERED(am->GetOpenDataStoreAction(), OnOpenDataStoreAction());
    CONNECT_ACTION_TRIGGERED(am->GetSaveTabsAction(), OnSaveTabsAction());
    CONNECT_ACTION_TRIGGERED(am->GetLoadTabsAction(), OnLoadTabsAction());
    CONNECT_ACTION_TRIGGERED(am->GetSaveGraphTabsAction(), OnSaveGraphTabsAction());
    CONNECT_ACTION_TRIGGERED(am->GetLoadGraphTabsAction(), OnLoadGraphTabsAction());
    CONNECT_ACTION_TRIGGERED(am->GetExitAction(), OnExitAction());
     
    //Edit 
    CONNECT_ACTION_TRIGGERED(am->GetSaveToTabAction(), OnSaveToTabAction());
    CONNECT_ACTION_TRIGGERED(am->GetSaveGraphToTabAction(), OnSaveGraphToTabAction());
	CONNECT_ACTION_TRIGGERED(am->GetNewLayerAction(),  OnNewLayerAction());
	CONNECT_ACTION_TRIGGERED(am->GetDeleteLayerAction(), OnDeleteLayerAction());
    CONNECT_ACTION_TRIGGERED(am->GetChangeLayerAction(), OnChangeLayerAction());
    CONNECT_ACTION_TRIGGERED(am->GetNewROIAction(), OnNewROIAction());
    
    //View
    CONNECT_ACTION_TRIGGERED(am->GetViewSliceAction(), OnViewSliceAction());
    CONNECT_ACTION_TRIGGERED(am->GetViewMosaicAction(), OnViewMosaicAction());
    CONNECT_ACTION_TRIGGERED(am->GetViewOrthoAction(), OnViewOrthoAction());
    CONNECT_ACTION_TRIGGERED(am->GetResetImageAction(), OnResetImageAction());

    // Tools 
    CONNECT_ACTION_TRIGGERED(am->GetManipulatorToolAction(), OnManipulatorToolAction());
    CONNECT_ACTION_TRIGGERED(am->GetVoxelToolAction(), OnVoxelToolAction());
    CONNECT_ACTION_TRIGGERED(am->GetRoiToolAction(), OnRoiToolAction());
    CONNECT_ACTION_TRIGGERED(am->GetVoxelSelectionToolAction(), OnVoxelSelectionToolAction());
    
    // Help
    CONNECT_ACTION_TRIGGERED(am->GetShowHelpAction(), OnShowHelpAction());
    CONNECT_ACTION_TRIGGERED(am->GetShowAboutAction(), OnShowAboutAction());

    // Connect some signals of BookmarkWidget 
    // to MainWindow object
    connect( m_mainWindow->m_BookmarkWidget, SIGNAL(CurrentChanged(unsigned long)),
             m_mainWindow, SLOT(OnBookmarkChanged(unsigned long)) );
    connect( m_mainWindow->m_BookmarkWidget, SIGNAL(DeleteTab(unsigned long)),
             m_mainWindow, SLOT(OnBookmarkDelete(unsigned long)) );

    // Connect signals to layer list widget
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(LayerSelected(unsigned long)),
             m_mainWindow, SLOT(OnLayerSelected(unsigned long)) );

    connect( m_mainWindow->m_LayerListWidget, SIGNAL(NewLayer()),
             this, SLOT(OnNewLayerAction()) );
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(DeleteLayer()),
             this, SLOT(OnDeleteLayerAction()) );
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayer()),
             this, SLOT(OnChangeLayerAction()) );
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayerParams()),
             this, SLOT(OnLayerParamsChanged()) );
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayerColormap()),
             this, SLOT(OnLayerColormapChanged()) );
    // new (for updating layers order)
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayerPosition(int)),
             this, SLOT(OnLayerPositionChanged(int)) );

    connect( m_mainWindow->m_LayerListWidget, SIGNAL(RoiToolChanged()),
             this, SLOT(OnRoiToolChanged()) );
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(ClearCurrentRoi()),
             this, SLOT(OnClearCurrentRoi()) );

    // Connect graph pane
    connect( m_mainWindow->m_GraphPaneWidget, SIGNAL(LiveModeChanged(bool)),
             this, SLOT(OnLiveModeChanged(bool)) );
    connect( m_mainWindow->m_GraphPaneWidget, SIGNAL(TimePointChanged(int)),
             this, SLOT(OnTimePointChanged(int)) );
    connect( m_mainWindow->m_GraphPaneWidget, SIGNAL(PreviousTimePoint()),
             this, SLOT(OnPreviousTimePointPressed()) );
    connect( m_mainWindow->m_GraphPaneWidget, SIGNAL(NextTimePoint()),
             this, SLOT(OnNextTimePointPressed()) );
    connect( m_mainWindow->m_GraphPaneWidget, SIGNAL(GraphChanged(unsigned long, bool)),
             this, SLOT(OnGraphChanged(unsigned long, bool)) );

    connect( m_mainWindow->m_GraphPaneWidget->GetGraphBookmarkWidget(), SIGNAL(CurrentChanged(unsigned long)),
             m_mainWindow, SLOT(OnGraphBookmarkChanged(unsigned long)) );
    connect( m_mainWindow->m_GraphPaneWidget->GetGraphBookmarkWidget(), SIGNAL(DeleteTab(unsigned long)),
             m_mainWindow, SLOT(OnGraphBookmarkDelete(unsigned long)) );


    // Connect image settings widget
    connect( m_mainWindow->m_ImageSettingsWidget, SIGNAL(ImageParamsChanged()),
        this, SLOT(OnImageParamsChanged()) );

    // Connect test action
    CONNECT_ACTION_TRIGGERED(am->GetTestAction(), OnTestAction());
    m_isInit = true;
}

void FrActionSignalManager::Deinitialize(){
    if(!m_isInit) return;

    // Connect signals
    FrActionManager* am = m_mainWindow->m_ActionManager;
    
    // File 
    DISCONNECT_ACTION_TRIGGERED(am->GetOpenImageAction(), OnOpenImageAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetOpenDataStoreAction(), OnOpenDataStoreAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetSaveTabsAction(), OnSaveTabsAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetLoadTabsAction(), OnLoadTabsAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetExitAction(), OnExitAction());
     
    //Edit 
    DISCONNECT_ACTION_TRIGGERED(am->GetSaveToTabAction(), OnSaveToTabAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetSaveGraphToTabAction(), OnSaveGraphToTabAction());
	DISCONNECT_ACTION_TRIGGERED(am->GetNewLayerAction(),  OnNewLayerAction());
	DISCONNECT_ACTION_TRIGGERED(am->GetDeleteLayerAction(), OnDeleteLayerAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetChangeLayerAction(), OnChangeLayerAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetNewROIAction(), OnNewROIAction());
    
    //View
    DISCONNECT_ACTION_TRIGGERED(am->GetViewSliceAction(), OnViewSliceAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetViewMosaicAction(), OnViewMosaicAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetViewOrthoAction(), OnViewOrthoAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetResetImageAction(), OnResetImageAction());

    // Tools 
    DISCONNECT_ACTION_TRIGGERED(am->GetManipulatorToolAction(), OnManipulatorToolAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetVoxelToolAction(), OnVoxelToolAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetRoiToolAction(), OnRoiToolAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetVoxelSelectionToolAction(), OnVoxelSelectionToolAction());
    
    // Help
    DISCONNECT_ACTION_TRIGGERED(am->GetShowHelpAction(), OnShowHelpAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetShowAboutAction(), OnShowAboutAction());

    // Connect some signals of BookmarkWidget 
    // to MainWindow object
    disconnect( m_mainWindow->m_BookmarkWidget, SIGNAL(CurrentChanged(unsigned long)),
             m_mainWindow, SLOT(OnBookmarkChanged(unsigned long)) );
    disconnect( m_mainWindow->m_BookmarkWidget, SIGNAL(DeleteTab(unsigned long)),
             m_mainWindow, SLOT(OnBookmarkDelete(unsigned long)) );

    // Connect signals to layer list widget
    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(LayerSelected(unsigned long)),
             m_mainWindow, SLOT(OnLayerSelected(unsigned long)) );

    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(NewLayer()),
             this, SLOT(OnNewLayerAction()) );
    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(DeleteLayer()),
             this, SLOT(OnDeleteLayerAction()) );
    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayer()),
             this, SLOT(OnChangeLayerAction()) );
    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayerParams()),
             this, SLOT(OnLayerParamsChanged()) );
    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(ChangeLayerColormap()),
             this, SLOT(OnLayerColormapChanged()) );

    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(RoiToolChanged()),
             this, SLOT(OnRoiToolChanged()) );

    // Graph pane
    disconnect( m_mainWindow->m_GraphPaneWidget, SIGNAL(LiveModeChanged(bool)),
             this, SLOT(OnLiveModeChanged(bool)) );

    disconnect( m_mainWindow->m_GraphPaneWidget, SIGNAL(TimePointChanged(int)),
             this, SLOT(OnTimePointChanged(int)) );

    disconnect( m_mainWindow->m_GraphPaneWidget, SIGNAL(PreviousTimePoint()),
             this, SLOT(OnPreviousTimePointPressed()) );
    disconnect( m_mainWindow->m_GraphPaneWidget, SIGNAL(NextTimePoint()),
             this, SLOT(OnNextTimePointPressed()) );
    disconnect( m_mainWindow->m_GraphPaneWidget, SIGNAL(NextTimePoint()),
             this, SLOT(OnNextTimePointPressed()) );

    disconnect( m_mainWindow->m_GraphPaneWidget->GetGraphBookmarkWidget(), SIGNAL(CurrentChanged(unsigned long)),
             m_mainWindow, SLOT(OnGraphBookmarkChanged(unsigned long)) );
    disconnect( m_mainWindow->m_GraphPaneWidget->GetGraphBookmarkWidget(), SIGNAL(DeleteTab(unsigned long)),
             m_mainWindow, SLOT(OnGraphBookmarkDelete(unsigned long)) );

    // image settings widget
    disconnect( m_mainWindow->m_ImageSettingsWidget, SIGNAL(ImageParamsChanged()),
        this, SLOT(OnImageParamsChanged()) );

    // Connect test action
    DISCONNECT_ACTION_TRIGGERED(am->GetTestAction(), OnTestAction());
    m_isInit = false;
}

 // File 
void FrActionSignalManager::OnOpenImageAction(){
    QStringList fileNames = QFileDialog::getOpenFileNames(
        m_mainWindow, tr("Open MRI Data"), 
        tr(""), tr("NIfTI Image (*.nii)"));
        
    std::vector<QString> fileNamesToLoad;
    fileNamesToLoad.reserve(fileNames.size());
    fileNamesToLoad.insert(fileNamesToLoad.begin(), 
        fileNames.begin(), fileNames.end());
    
    // .. load if any
    if(!fileNamesToLoad.empty()){
        m_mainWindow->
            GetMainController()->
            LoadImageFromFile(fileNamesToLoad);
    }
}

void FrActionSignalManager::OnOpenConfigAction(){
  QString fileName = QFileDialog::getOpenFileName(
        m_mainWindow, tr("Open Run Configuration"), 
        tr(""), tr("XML configuration (*.xml)"));
        
  RtConfigFmriRun runConfig;
    
  string st;
  // ohinds: had to change to compile
  //st = fileName.toLatin1();
  st = fileName.toStdString();

  if(!runConfig.parseConfigFile(st)) {
    cerr << "FrActionSignalManager::OnOpenConfig(): failed to parse config file " << st << endl;
  }

  executeRun(runConfig);
}

void FrActionSignalManager::OnOpenDataStoreAction(){
    m_mainWindow->GetMainController()->OpenDataStore();
}

void FrActionSignalManager::OnSaveTabsAction(){
    QString fileName = QFileDialog::getSaveFileName(
        m_mainWindow, tr("Save Tabs Data"), 
        tr(""), tr("Tabs Settings (*.tbs)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->
            GetMainController()->
            IoTabSettings(fileName, false);
    }
}

void FrActionSignalManager::OnLoadTabsAction(){
    QString fileName = QFileDialog::getOpenFileName(
        m_mainWindow, tr("Load Tabs Data"), 
        tr(""), tr("Tabs Settings (*.tbs)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->
            GetMainController()->
            IoTabSettings(fileName, true);
    }
}

void FrActionSignalManager::OnSaveGraphTabsAction(){
    QString fileName = QFileDialog::getSaveFileName(
        m_mainWindow, tr("Save Graph Tabs Data"), 
        tr(""), tr("Graph Tabs Settings (*.gtbs)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->
            GetMainController()->
            IoGraphTabSettings(fileName, false);
    }
}

void FrActionSignalManager::OnLoadGraphTabsAction(){
    QString fileName = QFileDialog::getOpenFileName(
        m_mainWindow, tr("Load Graph Tabs Data"), 
        tr(""), tr("Graph Tabs Settings (*.gtbs)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->
            GetMainController()->
            IoGraphTabSettings(fileName, true);
    }
}


void FrActionSignalManager::OnExitAction(){
    QApplication::quit();
}

//Edit 
void FrActionSignalManager::OnSaveToTabAction(){
    m_mainWindow->
        GetMainController()->
        SaveCurrentViewToTab();
}

void FrActionSignalManager::OnSaveGraphToTabAction(){
    m_mainWindow->
        GetMainController()->
        SaveCurrentGraphToTab();
}

void FrActionSignalManager::OnNewLayerAction(){
	m_mainWindow->GetMainController()->AddLayer();
}

void FrActionSignalManager::OnDeleteLayerAction(){
	m_mainWindow->GetMainController()->DeleteLayer();
}

void FrActionSignalManager::OnChangeLayerAction(){
    m_mainWindow->GetMainController()->ChangeLayer(0);
}

void FrActionSignalManager::OnNewROIAction(){
    m_mainWindow->GetMainController()->CreatNewROI();
}

//View
void FrActionSignalManager::OnViewSliceAction(){    
	m_mainWindow->GetMainController()->ChangeView(0);
}

void FrActionSignalManager::OnViewMosaicAction(){
    m_mainWindow->GetMainController()->ChangeView(1);
}

void FrActionSignalManager::OnViewOrthoAction(){
    m_mainWindow->GetMainController()->ChangeView(2);
}

void FrActionSignalManager::OnResetImageAction(){
    m_mainWindow->GetMainController()->ResetImage();
}

// Tools 
void FrActionSignalManager::OnManipulatorToolAction(){
    m_mainWindow->
        GetMainController()->
        SetCurrentTool(0);
}

void FrActionSignalManager::OnVoxelToolAction(){
    m_mainWindow->
        GetMainController()->
        SetCurrentTool(1);
}

void FrActionSignalManager::OnRoiToolAction(){
    m_mainWindow->
        GetMainController()->
        SetCurrentTool(2);
}

void FrActionSignalManager::OnVoxelSelectionToolAction(){
    m_mainWindow->
        GetMainController()->
        SetCurrentTool(3);
}

// Help
void FrActionSignalManager::OnShowHelpAction(){
    QMessageBox::information(m_mainWindow,
        tr("To be implemented"),
        tr("This shows help..."));
}

void FrActionSignalManager::OnShowAboutAction(){
    QApplication::aboutQt();
}

//Test
void FrActionSignalManager::OnTestAction(){
    m_mainWindow->GetMainController()->Test();
}

void FrActionSignalManager::OnLayerParamsChanged(){
    m_mainWindow->GetMainController()->ChangeLayer(1);
}

void FrActionSignalManager::OnLayerColormapChanged(){
    m_mainWindow->GetMainController()->ChangeLayer(2);
}

void FrActionSignalManager::OnImageParamsChanged(){
    m_mainWindow->GetMainController()->ChangeImageSettings();
}

void FrActionSignalManager::OnLayerPositionChanged(int inc){
    m_mainWindow->GetMainController()->ChangeLayerPosition(inc);
}

void FrActionSignalManager::OnRoiToolChanged(){
    m_mainWindow->
        GetMainController()->
        UpdateRoiTool();
}

void FrActionSignalManager::OnClearCurrentRoi(){
    m_mainWindow->
        GetMainController()->
        ClearCurrentRoi();
}

// Graph pane events
void FrActionSignalManager::OnLiveModeChanged(bool newValue){
    m_mainWindow->
        GetMainController()->
        SetLiveMode(newValue);
}

void FrActionSignalManager::OnTimePointChanged(int newValue){
    m_mainWindow->
        GetMainController()->
        SetCurrentTimePoint(newValue);
    cout << "changing timepoint to " << newValue << endl;
}

void FrActionSignalManager::OnPreviousTimePointPressed(){
    m_mainWindow->
        GetMainController()->
        SetPreviousTimePoint();
}

void FrActionSignalManager::OnNextTimePointPressed(){
    m_mainWindow->
        GetMainController()->
        SetNextTimePoint();
}

void FrActionSignalManager::OnGraphChanged(unsigned long id, bool add){
    m_mainWindow->
        GetMainController()->
        ChangeGraph(id, add);
}
