#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrBookmarkWidget.h"
#include "FrLayerListWidget.h"
#include "QVTKWidget.h"
#include "FrToolBar.h"
#include "FrMainMenu.h"
#include "FrActionManager.h"
#include "FrActionSignalManager.h"

#include "Qt/QGroupBox.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QApplication.h"
#include "Qt/QMessageBox.h"
#include "Qt/QTabWidget.h"
#include "Qt/QTextEdit.h"
#include "QtGui/QStatusBar.h"
#include "Qt/QDockWidget.h"


FrMainWindow::FrMainWindow()
    :  QMainWindow(0), m_MainDocument(0), m_MainController(0), 
    m_SliceView(0), m_MosaicView(0), m_OrthoView(0), m_CurrentView(0){
	
    SetupUi(this);
    InitializeWidgets();
    
    // Initialize signals
    m_SignalManager = new FrActionSignalManager(this);
}

FrMainWindow::~FrMainWindow(){
    if(m_SliceView) delete m_SliceView;
    if(m_MosaicView) delete m_MosaicView;
    if(m_OrthoView) delete m_OrthoView;
    if(m_SignalManager) delete m_SignalManager;
}

void FrMainWindow::SetupUi(QMainWindow* mainWindow){
    // Main window
    mainWindow->resize(600, 800);
    if (mainWindow->objectName().isEmpty())
        mainWindow->setObjectName(QString::fromUtf8("MainWindow"));

    // Central widget
//    m_centralWidget = new QWidget(mainWindow);
//    m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
//    mainWindow->setCentralWidget(m_centralWidget);

    // Action manager
    m_ActionManager = new FrActionManager(mainWindow);

    // Main menu
    m_mainMenu = new FrMainMenu(mainWindow, m_ActionManager);
    m_mainMenu->setObjectName(QString::fromUtf8("m_mainMenu"));
    m_mainMenu->setGeometry(QRect(0, 0, 800, 21));
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
        QApplication::translate("MainWindow", "MRI", 0, 
        QApplication::UnicodeUTF8));
    
    m_ActionManager->Retranslate();
    m_mainMenu->Retranslate();
    m_toolBar->Retranslate();
}

void FrMainWindow::InitializeWidgets(){
    
//    QWidget* centralWidget = this->centralWidget();
    
//	QDockWidget *dock = new QDockWidget(tr("3D View"), this);
//	dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* topPane = new QWidget(this);
    topPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Create 3D view and logical views 
    m_QVTKWidget = new QVTKWidget(topPane);
	m_QVTKWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    
    m_SliceView  = new FrSliceView(this);
    m_MosaicView = new FrMosaicView(this);
    m_OrthoView  = new FrOrthoView(this);

    // Create bookmark widget
    m_BookmarkWidget = new FrBookmarkWidget(topPane);
	m_BookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QHBoxLayout* viewLayout = new QHBoxLayout();
	viewLayout->setContentsMargins(0, 0, 0, 0);
	viewLayout->setSpacing(0);
    viewLayout->addWidget(m_QVTKWidget);
    viewLayout->addWidget(m_BookmarkWidget);

    topPane->setLayout(viewLayout);
	this->setCentralWidget(topPane);		// test

//	dock->setWidget(topPane);
//	addDockWidget(Qt::LeftDockWidgetArea, dock);
	m_mainMenu->GetMenuView()->addSeparator();
//	m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	// ?

	QDockWidget* dock = new QDockWidget(tr("Info Tab"), this);
	dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QTabWidget* viewInfoControl = new QTabWidget(dock);
    viewInfoControl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewInfoControl->addTab(new QWidget(viewInfoControl), QString("Info1"));
    viewInfoControl->addTab(new QWidget(viewInfoControl), QString("Info2"));

	dock->setWidget(viewInfoControl);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	// ?
		
//    QHBoxLayout* vcLayout = new QHBoxLayout();
//    vcLayout->addLayout(viewLayout);
//    vcLayout->addWidget(viewInfoControl);


	dock = new QDockWidget(tr("Graph View"), this);
//	dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	
    QWidget* btmPane = new QWidget(dock);
    btmPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* graphLayout = new QHBoxLayout();
    graphLayout->addWidget(new QGroupBox(QString("Graph View"), btmPane));
    btmPane->setLayout(graphLayout);

	dock->setWidget(btmPane);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	// ?

	dock = new QDockWidget(tr("Layer List"), this);

	m_LayerListWidget = new FrLayerListWidget(dock);

	dock->setWidget(m_LayerListWidget);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	m_mainMenu->GetMenuView()->addAction(dock->toggleViewAction());	// ?

    // Setup layout to  central widget
    //QVBoxLayout* vLayout = new  QVBoxLayout();
    //vLayout->addWidget(topPane);
    //vLayout->addWidget(btmPane);

    //centralWidget->setLayout(vLayout);
}

void FrMainWindow::Initialize(){
    m_OrthoView->Initialize();
    m_OrthoView->RemoveRenderers();

    m_MosaicView->Initialize();
    m_MosaicView->RemoveRenderers();

    m_SliceView->Initialize();

    // make it as current
    m_CurrentView = m_SliceView;
}

void FrMainWindow::OnBookmarkChanged(int id){
    this->GetMainController()->ChangeBookmark(id);
}

void FrMainWindow::OnBookmarkDelete(int id){
    this->GetMainController()->DeleteBookmark(id);
}

void FrMainWindow::OnLayerEdit(int id){
    this->GetMainController()->EditLayer(id);
}