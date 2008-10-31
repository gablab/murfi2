#include "FrActionSignalManager.h"
#include "FrActionManager.h"
#include "FrMainWindow.h"
#include "FrMainController.h"
#include "FrBookmarkWidget.h"
#include "FrLayerListWidget.h"
#include "FrSettings.h"

// Qt
#include "Qt/qstring.h"
#include "Qt/qaction.h"
#include "Qt/qapplication.h"
#include "Qt/qmessagebox.h"
#include <QtGui/QFileDialog>


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
    CONNECT_ACTION_TRIGGERED(am->GetSaveTabsAction(), OnSaveTabsAction());
    CONNECT_ACTION_TRIGGERED(am->GetLoadTabsAction(), OnLoadTabsAction());
    CONNECT_ACTION_TRIGGERED(am->GetExitAction(), OnExitAction());
     
    //Edit 
    CONNECT_ACTION_TRIGGERED(am->GetSaveToTabAction(), OnSaveToTabAction());
	CONNECT_ACTION_TRIGGERED(am->GetNewLayerAction(),  OnNewLayerAction());
	CONNECT_ACTION_TRIGGERED(am->GetDeleteLayerAction(), OnDeleteLayerAction());
    CONNECT_ACTION_TRIGGERED(am->GetChangeLayerAction(), OnChangeLayerAction());
    
    //View
    CONNECT_ACTION_TRIGGERED(am->GetViewSliceAction(), OnViewSliceAction());
    CONNECT_ACTION_TRIGGERED(am->GetViewMosaicAction(), OnViewMosaicAction());
    CONNECT_ACTION_TRIGGERED(am->GetViewOrthoAction(), OnViewOrthoAction());
    CONNECT_ACTION_TRIGGERED(am->GetResetImageAction(), OnResetImageAction());

    // Tools 
    CONNECT_ACTION_TRIGGERED(am->GetManipulatorToolAction(), OnManipulatorToolAction());
    CONNECT_ACTION_TRIGGERED(am->GetVoxelToolAction(), OnVoxelToolAction());
    CONNECT_ACTION_TRIGGERED(am->GetRoiToolAction(), OnRoiToolAction());
    
    // Help
    CONNECT_ACTION_TRIGGERED(am->GetShowHelpAction(), OnShowHelpAction());
    CONNECT_ACTION_TRIGGERED(am->GetShowAboutAction(), OnShowAboutAction());

    // Connect some signals of BookmarkWidget 
    // to MainWindow object
    connect( m_mainWindow->m_BookmarkWidget, SIGNAL(CurrentChanged(int)),
             m_mainWindow, SLOT(OnBookmarkChanged(int)) );
    connect( m_mainWindow->m_BookmarkWidget, SIGNAL(DeleteTab(int)),
             m_mainWindow, SLOT(OnBookmarkDelete(int)) );

    // Connect signals to layer list widget
    connect( m_mainWindow->m_LayerListWidget, SIGNAL(LayerSelected(int)),
             m_mainWindow, SLOT(OnLayerSelected(int)) );

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
    DISCONNECT_ACTION_TRIGGERED(am->GetSaveTabsAction(), OnSaveTabsAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetLoadTabsAction(), OnLoadTabsAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetExitAction(), OnExitAction());
     
    //Edit 
    DISCONNECT_ACTION_TRIGGERED(am->GetSaveToTabAction(), OnSaveToTabAction());
	DISCONNECT_ACTION_TRIGGERED(am->GetNewLayerAction(),  OnNewLayerAction());
	DISCONNECT_ACTION_TRIGGERED(am->GetDeleteLayerAction(), OnDeleteLayerAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetChangeLayerAction(), OnChangeLayerAction());
    
    //View
    DISCONNECT_ACTION_TRIGGERED(am->GetViewSliceAction(), OnViewSliceAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetViewMosaicAction(), OnViewMosaicAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetViewOrthoAction(), OnViewOrthoAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetResetImageAction(), OnResetImageAction());

    // Tools 
    DISCONNECT_ACTION_TRIGGERED(am->GetManipulatorToolAction(), OnManipulatorToolAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetVoxelToolAction(), OnVoxelToolAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetRoiToolAction(), OnRoiToolAction());
    
    // Help
    DISCONNECT_ACTION_TRIGGERED(am->GetShowHelpAction(), OnShowHelpAction());
    DISCONNECT_ACTION_TRIGGERED(am->GetShowAboutAction(), OnShowAboutAction());

    // Connect some signals of BookmarkWidget 
    // to MainWindow object
    disconnect( m_mainWindow->m_BookmarkWidget, SIGNAL(CurrentChanged(int)),
             m_mainWindow, SLOT(OnBookmarkChanged(int)) );
    disconnect( m_mainWindow->m_BookmarkWidget, SIGNAL(DeleteTab(int)),
             m_mainWindow, SLOT(OnBookmarkDelete(int)) );

    // Connect signals to layer list widget
    disconnect( m_mainWindow->m_LayerListWidget, SIGNAL(LayerSelected(int)),
             m_mainWindow, SLOT(OnLayerSelected(int)) );

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

    // Connect test action
    DISCONNECT_ACTION_TRIGGERED(am->GetTestAction(), OnTestAction());
    m_isInit = false;
}

 // File 
void FrActionSignalManager::OnOpenImageAction(){
    QString fileName = QFileDialog::getOpenFileName(
        m_mainWindow, tr("Open MRI Data"), tr(""), 
        tr("NIfTI Image (*.nii)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->GetMainController()->LoadImageFromFile(fileName);
    }
}

void FrActionSignalManager::OnSaveTabsAction(){
    QString fileName = QFileDialog::getSaveFileName(
        m_mainWindow, tr("Save Tabs Data"), tr(""), 
        tr("Tabs Settings (*.tbs)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->GetMainController()->IoTabSettings(fileName, false);
    }
}

void FrActionSignalManager::OnLoadTabsAction(){
    QString fileName = QFileDialog::getOpenFileName(
        m_mainWindow, tr("Load Tabs Data"), tr(""), tr("Tabs Settings (*.tbs)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_mainWindow->GetMainController()->IoTabSettings(fileName, true);
    }
}

void FrActionSignalManager::OnExitAction(){
    QApplication::quit();
}

//Edit 
void FrActionSignalManager::OnSaveToTabAction(){
    m_mainWindow->GetMainController()->SaveCurrentViewToTab();
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
    m_mainWindow->GetMainController()->SetCurrentTool(0);
}

void FrActionSignalManager::OnVoxelToolAction(){
    m_mainWindow->GetMainController()->SetCurrentTool(1);
}

void FrActionSignalManager::OnRoiToolAction(){
    m_mainWindow->GetMainController()->SetCurrentTool(2);
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