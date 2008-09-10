#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrView2D.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrToolsPanel.h"
#include "FrLowPanel.h"
#include "FrBookmarkWidget.h"
#include "QVTKWidget.h"

#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"


FrMainWindow::FrMainWindow()
    :  QMainWindow(0), m_MainDocument(0), m_MainController(0), 
    m_view2D(0), m_viewMosaic(0), m_viewOrtho(0){
	
    setupUi(this);
    InitializeWidgets();
    InitializeSignals();
}

FrMainWindow::~FrMainWindow(){
    if(m_view2D) delete m_view2D;
    if(m_viewMosaic) delete m_viewMosaic;
    if(m_viewOrtho) delete m_viewOrtho;
}

void FrMainWindow::InitializeWidgets(){
    QGroupBox* groupBox = new QGroupBox(this);

    // Create 3D View
    m_qtView = new QVTKWidget(groupBox);
    m_qtView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_view2D = new FrView2D(this);
    m_viewMosaic = new FrMosaicView(this);
    m_viewOrtho = new FrOrthoView(this);;
    
    m_bookmarkWidget = new FrBookmarkWidget(groupBox);
	m_bookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QHBoxLayout *groupBoxLayout = new QHBoxLayout(groupBox);
	groupBoxLayout->setContentsMargins(0, 0, 0, 0);
	groupBoxLayout->setSpacing(0);
    groupBoxLayout->addWidget(m_qtView);
    groupBoxLayout->addWidget(m_bookmarkWidget);

    m_toolsPanel = new FrToolsPanel(this);
	m_toolsPanel->setFixedWidth(m_toolsPanel->width());
	m_toolsPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    m_lowPanel = new FrLowPanel(this);
	m_lowPanel->setFixedHeight(m_lowPanel->height());

	QVBoxLayout *verticalLayout = new QVBoxLayout;
	verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->addWidget(groupBox);
	verticalLayout->addWidget(m_lowPanel);

	// horizontal layout
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(m_toolsPanel);
	layout->addLayout(verticalLayout);
	
    m_graphWidget = new QWidget;
    m_slice2DWidget = new QWidget;
    m_tabWidget = new QTabWidget(this);

	m_slice2DWidget->setLayout(layout);
	m_tabWidget->addTab(m_slice2DWidget, "2D Slice View");
	m_tabWidget->addTab(m_graphWidget, "Graphs and calculations");
	setCentralWidget(m_tabWidget);
}

void FrMainWindow::InitializeSignals(){
    // actions of File-menu
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionLoadImage, SIGNAL(triggered()), this, SLOT(openImage())); 

	// actions of Help-menu
	connect(actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	// toolbar actions
	connect(actionTool1, SIGNAL(triggered()), this, SLOT(tool1Triggered()));
	connect(actionTool2, SIGNAL(triggered()), this, SLOT(tool2Triggered()));
	connect(actionTool3, SIGNAL(triggered()), this, SLOT(tool3Triggered()));
	connect(actionSaveToTab, SIGNAL(triggered()), this, SLOT(saveToTab()));

	// actions of tools panel
	connect(m_toolsPanel->mode1Button, SIGNAL(clicked()), this, SLOT(mode1Clicked()));
	connect(m_toolsPanel->mode2Button, SIGNAL(clicked()), this, SLOT(mode2Clicked()));
	connect(m_toolsPanel->mode3Button, SIGNAL(clicked()), this, SLOT(mode3Clicked()));

	// actions of Low panel
	//connect(m_lowPanel->brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(brightnessValueChanged()));
	//connect(m_lowPanel->contrastSlider, SIGNAL(sliderReleased()), this, SLOT(contrastValueChanged()));
	//connect(m_lowPanel->thresholdSlider, SIGNAL(sliderReleased()), this, SLOT(thresholdValueChanged()));
    connect(m_lowPanel->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessValueChanged()));
	connect(m_lowPanel->contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(contrastValueChanged()));
	connect(m_lowPanel->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(thresholdValueChanged()));

	// actions of Tab widget
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	// actions of Bookmark Widget
	connect(m_bookmarkWidget, SIGNAL(currentChanged(int)), this, SLOT(bookmarkChanged(int)));
}

void FrMainWindow::Initialize(){
    m_viewOrtho->Initialize();
    m_viewOrtho->RemoveRenderers();

    m_viewMosaic->Initialize();
    m_viewMosaic->RemoveRenderers();

    m_view2D->Initialize();
    m_view2D->SetupPipeline();
}

// change brightness of the scene
void FrMainWindow::brightnessValueChanged(){
    // map into range [-1..+1]
    double value = m_lowPanel->GetBrightnessValue();
    value /= 100.0;

    if(m_MainController){
        m_MainController->SetValueTBC(FrMainController::Brightness, value);
    }
}

// change contrast of the scene
void FrMainWindow::contrastValueChanged(){    
    // map into range [-1..+1]
    double value = m_lowPanel->GetContrastValue();
    value /= 100.0;
    
    if(m_MainController){
        m_MainController->SetValueTBC(FrMainController::Contrast, value);
    }
}

// change threshold of the scene
void FrMainWindow::thresholdValueChanged(){
    // map into range [0..+1]
    double value = m_lowPanel->GetThresholdValue();
    value /= 100.0;

    if(m_MainController){
        m_MainController->SetValueTBC(FrMainController::Threshold, value);
    }
}

void FrMainWindow::tool1Triggered(){
	QMessageBox::information(this, "Info", "tool 1 triggered");

}

void FrMainWindow::tool2Triggered(){
	QMessageBox::information(this, "Info", "tool 2 triggered");

}

void FrMainWindow::tool3Triggered(){
	QMessageBox::information(this, "Info", "tool 3 clicked");

}

void FrMainWindow::mode1Clicked(){
	QMessageBox::information(this, "Info", "mode 1 clicked");

}

void FrMainWindow::mode2Clicked(){
	QMessageBox::information(this, "Info", "mode 2 clicked");

}

void FrMainWindow::mode3Clicked(){
	QMessageBox::information(this, "Info", "mode 3 clicked");

}

void FrMainWindow::tabChanged(int index){
	// disable some tools at toolbar
	switch (index)
	{
			case 0:		// 2D image view
				this->actionTool1->setDisabled(false);
				this->actionTool2->setDisabled(false);
				this->actionTool3->setDisabled(false);
				break;

			case 1:		// Graphics/calculations view
				this->actionTool1->setDisabled(true);
				this->actionTool2->setDisabled(true);
				this->actionTool3->setDisabled(true);
				break;
	}
}

// this slot indicates to what tab user switched
void FrMainWindow::bookmarkChanged(int index){
	
}

void FrMainWindow::saveToTab(){
    //QWidget* tab = new QWidget();
    //tab->setObjectName(QString::fromUtf8("tab"));
    //myBookmarkWidget->addTab(tab, "test");
	m_bookmarkWidget->addBookmark();
}

#include <Qt/QString.h>
#include <QtGUI/QFileDialog.h>
void FrMainWindow::openImage(){
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, 
        QString("Open MRI data"), QString(), 
        QString("NIfTI Image (*.nii)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        //fileName = "test.png";
        m_MainController->LoadImage(fileName);
    }
}