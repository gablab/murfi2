#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrViewDocObj.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrBookmarkWidget.h"
#include "FrLayerListWidget.h"
#include "FrVoxelInfoWidget.h"
#include "FrROIInfoWidget.h"
#include "QVTKWidget.h"
#include "FrToolBar.h"
#include "FrMainMenu.h"
#include "FrActionManager.h"
#include "FrActionSignalManager.h"
#include "FrGraphPaneWidget.h"
#include "FrImageSettingsWidget.h"

#include "Qt/qgroupbox.h"
#include "Qt/qboxlayout.h"
#include "Qt/qapplication.h"
#include "Qt/qmessagebox.h"
#include "Qt/qtabwidget.h"
#include "Qt/qtextedit.h"
#include "QtGui/qstatusbar.h"
#include "Qt/qdockwidget.h"


FrMainWindow::FrMainWindow()
: QMainWindow(0), m_MainDocument(0), m_MainController(0), 
  m_SliceView(0), m_MosaicView(0), m_OrthoView(0), m_CurrentView(0),
  m_SignalManager(0), m_ActionManager(0) {

    SetupUi(this);
    InitializeWidgets();

    // Initialize signals
    m_SignalManager = new FrActionSignalManager(this);
}

FrMainWindow::~FrMainWindow(){
    if(m_SignalManager) delete m_SignalManager;
    if(m_ActionManager) delete m_ActionManager;
    if(m_SliceView) delete m_SliceView;
    if(m_MosaicView) delete m_MosaicView;
    if(m_OrthoView) delete m_OrthoView;
}

void FrMainWindow::SetMainDocument(FrMainDocument* document){
    if(document != m_MainDocument){
        m_MainDocument = document;
        m_LayerListWidget->SetDocument(document);
        m_LayerListWidget->Update();

        m_GraphPaneWidget->SetDocument(document);
        m_GraphPaneWidget->Update();

        m_ImageSettingsWidget->SetDocument(document);
        m_ImageSettingsWidget->Update();
    }
}
void FrMainWindow::SetupUi(QMainWindow* mainWindow){
    // Main window
    mainWindow->setWindowState(Qt::WindowMaximized);
    if (mainWindow->objectName().isEmpty())
    mainWindow->setObjectName(QString::fromUtf8("MainWindow"));

    // Action manager
    m_ActionManager = new FrActionManager(mainWindow);

    // Main menu
    m_mainMenu = new FrMainMenu(mainWindow, m_ActionManager);
    m_mainMenu->setObjectName(QString::fromUtf8("m_mainMenu"));
    mainWindow->setMenuBar(m_mainMenu);

    // Tool bar
    m_toolBar = new FrToolBar(mainWindow, m_ActionManager);
    m_toolBar->setObjectName(QString::fromUtf8("m_toolBar"));
    mainWindow->addToolBar(Qt::TopToolBarArea, m_toolBar);

    // Status bar        
    m_statusBar = new QStatusBar(mainWindow);
    m_statusBar->setObjectName(QString::fromUtf8("m_statusBar"));
    mainWindow->setStatusBar(m_statusBar);

    RetranslateUi(mainWindow);
    QMetaObject::connectSlotsByName(mainWindow);
}


void FrMainWindow::RetranslateUi(QMainWindow* mainWindow){
    // Performe retranslation
    mainWindow->setWindowTitle(
        QApplication::translate("MainWindow", "festr", 0, 
        QApplication::UnicodeUTF8));

    m_ActionManager->Retranslate();
    m_mainMenu->Retranslate();
    m_toolBar->Retranslate();
}

void FrMainWindow::InitializeWidgets(){
    QWidget* topPane = new QWidget(this);
    topPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Create 3D view and logical views 
    m_QVTKWidget = new QVTKWidget(topPane);
    m_QVTKWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    m_QVTKWidget->setMinimumHeight(400);

    m_SliceView  = new FrSliceView(this);
    m_MosaicView = new FrMosaicView(this);
    m_OrthoView  = new FrOrthoView(this);

    // Create bookmark widget
    m_BookmarkWidget = new FrBookmarkWidget(topPane);
    m_BookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QHBoxLayout* viewLayout = new QHBoxLayout();
#if QT_VERSION >= 0x040300
    viewLayout->setContentsMargins(0, 0, 0, 0);
#else
    viewLayout->setMargin(0);
#endif
    viewLayout->setSpacing(0);
    viewLayout->addWidget(m_QVTKWidget);
    viewLayout->addWidget(m_BookmarkWidget);

    topPane->setLayout(viewLayout);
    this->setCentralWidget(topPane);
    //topPane->setGeometry(QRect(0,0,600,400));

    m_mainMenu->GetMenuView()->addSeparator();

    QDockWidget* dock = new QDockWidget(tr("Graph View"), this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | 
                          Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_GraphPaneWidget = new FrGraphPaneWidget(dock);
    m_GraphPaneWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    dock->setWidget(m_GraphPaneWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	

    dock = new QDockWidget(tr("Layer List"), this);
    m_LayerListWidget = new FrLayerListWidget(dock, m_MainDocument);

    dock->setWidget(m_LayerListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	

    dock = new QDockWidget(tr("Voxel Info"), this);
    m_VoxelInfoWidget = new FrVoxelInfoWidget(dock);

    dock->setWidget(m_VoxelInfoWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setVisible(false);
    m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	

    dock = new QDockWidget(tr("Image Settings"), this);
    m_ImageSettingsWidget = new FrImageSettingsWidget(dock, m_MainDocument);

    dock->setWidget(m_ImageSettingsWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setVisible(false);
    m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	

    //dock = new QDockWidget(tr("ROI Info"), this);

    //m_ROIInfoWidget = new FrROIInfoWidget(dock);

    //dock->setWidget(m_ROIInfoWidget);
    //addDockWidget(Qt::LeftDockWidgetArea, dock);

    //dock = new QDockWidget(tr("Drawing Panel"), this);

    //m_DrawingPanelWidget = new FrDrawingPanelWidget(dock);

    //dock->setWidget(m_DrawingPanelWidget);
    //addDockWidget(Qt::LeftDockWidgetArea, dock);
    //m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	
}

void FrMainWindow::Initialize(){
    m_MosaicView->Initialize();
    m_MosaicView->RemoveRenderers();

    m_SliceView->Initialize();
    m_SliceView->RemoveRenderers();

    m_OrthoView->Initialize();

    // make it as current
    m_CurrentView = m_OrthoView;

    FrViewDocObj* viewDO = new FrViewDocObj();
    this->GetMainController()->GetMainDocument()->Add(viewDO);
}

void FrMainWindow::OnBookmarkChanged(unsigned long id){
    this->GetMainController()->ChangeBookmark(id);
}

void FrMainWindow::OnBookmarkDelete(unsigned long id){
    this->GetMainController()->DeleteBookmark(id);
}

void FrMainWindow::OnGraphBookmarkChanged(unsigned long id){
    this->GetMainController()->ChangeGraphBookmark(id);
}

void FrMainWindow::OnGraphBookmarkDelete(unsigned long id){
    this->GetMainController()->DeleteGraphBookmark(id);
}

void FrMainWindow::OnLayerSelected(unsigned long id){
    this->GetMainController()->SelectLayer(id);
}

void FrMainWindow::DisconnectActions(){
    m_SignalManager->Deinitialize();
}
