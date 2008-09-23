#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrToolsPanel.h"
#include "FrLowPanel.h"
#include "FrBookmarkWidget.h"
#include "QVTKWidget.h"
#include "FrTabInfoDialog.h"

#include "Qt/QMessageBox.h"
#include "Qt/QTabWidget.h"

#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"


FrMainWindow::FrMainWindow()
    :  QMainWindow(0), m_MainDocument(0), m_MainController(0), 
    m_SliceView(0), m_MosaicView(0), m_OrthoView(0), m_CurrentView(0){
	
    setupUi(this);
    InitializeWidgets();
    InitializeSignals();
}

FrMainWindow::~FrMainWindow(){
    if(m_SliceView) delete m_SliceView;
    if(m_MosaicView) delete m_MosaicView;
    if(m_OrthoView) delete m_OrthoView;
}

void FrMainWindow::InitializeWidgets(){
    QGroupBox* groupBox = new QGroupBox(this);

    // Create 3D View
    m_qtView = new QVTKWidget(groupBox);
    m_qtView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_SliceView  = new FrSliceView(this);
    m_MosaicView = new FrMosaicView(this);
    m_OrthoView  = new FrOrthoView(this);;
    
    m_BookmarkWidget = new FrBookmarkWidget(groupBox);
	m_BookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            
    QHBoxLayout *groupBoxLayout = new QHBoxLayout(groupBox);
	groupBoxLayout->setContentsMargins(0, 0, 0, 0);
	groupBoxLayout->setSpacing(0);
    groupBoxLayout->addWidget(m_qtView);
    groupBoxLayout->addWidget(m_BookmarkWidget);

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
    connect(m_lowPanel->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessValueChanged()));
	connect(m_lowPanel->contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(contrastValueChanged()));
	connect(m_lowPanel->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(thresholdValueChanged()));

	// actions of Bookmark Widget
	connect(m_BookmarkWidget, SIGNAL(CurrentChanged(int)), this, SLOT(OnBookmarkChanged(int)));
    connect(m_BookmarkWidget, SIGNAL(DeleteTab(int)), this, SLOT(OnBookmarkDelete(int)));
}

void FrMainWindow::Initialize(){
    m_OrthoView->Initialize();
    m_OrthoView->RemoveRenderers();

    m_MosaicView->Initialize();
    m_MosaicView->RemoveRenderers();
    
    m_SliceView->Initialize();
    //m_SliceView->UpdatePipeline(0);

    // make it as current
    m_CurrentView = m_SliceView;
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
	this->GetMainController()->ChangeView(0);    
}

void FrMainWindow::mode2Clicked(){
    this->GetMainController()->ChangeView(1);    
}

void FrMainWindow::mode3Clicked(){
    this->GetMainController()->ChangeView(2);
}

void FrMainWindow::OnBookmarkChanged(int id){
    this->GetMainController()->ChangeBookmark(id);
}

void FrMainWindow::OnBookmarkDelete(int id){
    this->GetMainController()->DeleteBookmark(id);
}

// this slot indicates to what tab user switched
void FrMainWindow::saveToTab(){
    this->GetMainController()->SaveCurrentViewToTab();
}

#include <Qt/QString.h>
#include <QtGUI/QFileDialog.h>
void FrMainWindow::openImage(){
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, 
        QString("Open MRI data"), QString(), 
        QString("NIfTI Image (*.nii)"));

    if(!fileName.isNull() && !fileName.isEmpty()){
        m_MainController->LoadImage(fileName);
    }
}

void FrMainWindow::UpdateTBCValues(double contrast, double brightness){
	m_lowPanel->SetContrastValue(contrast);	
	m_lowPanel->SetBrightnessValue(brightness);
}