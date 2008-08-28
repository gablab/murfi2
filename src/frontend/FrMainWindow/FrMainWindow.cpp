#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrView2D.h"
#include "FrToolsPanel.h"
#include "FrLowPanel.h"
#include "FrBookmarkWidget.h"

#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"


FrMainWindow::FrMainWindow()
    :  QMainWindow(0), m_document(0), m_controller(0), m_view2D(0){
	
    setupUi(this);
		
	QGroupBox* groupBox = new QGroupBox(this);	
    m_view2D = new FrView2D(this, groupBox);
    
    m_bookmarkWidget = new FrBookmarkWidget(groupBox);
	m_bookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QHBoxLayout *groupBoxLayout = new QHBoxLayout(groupBox);
	groupBoxLayout->setContentsMargins(0, 0, 0, 0);
	groupBoxLayout->setSpacing(0);
    groupBoxLayout->addWidget(m_view2D->GetWidget());
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
	connect(m_lowPanel->brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(brightnessValueChanged()));
	connect(m_lowPanel->contrastSlider, SIGNAL(sliderReleased()), this, SLOT(contrastValueChanged()));
	connect(m_lowPanel->thresholdSlider, SIGNAL(sliderReleased()), this, SLOT(thresholdValueChanged()));

	// actions of Tab widget
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	// actions of Bookmark Widget
	connect(m_bookmarkWidget, SIGNAL(currentChanged(int)), this, SLOT(bookmarkChanged(int)));
}

FrMainWindow::~FrMainWindow(){
}

// change brightness of the scene
void FrMainWindow::brightnessValueChanged(){
    // should be in the range [-1..+1]
    double value = m_lowPanel->GetBrightnessValue();
    value /= 100.0;

    //QString message("Brightness Value: ");
    //message += QString::number(value);
    //QMessageBox::information(this, "Info", message);

    m_view2D->SetBrightness(value);
    m_view2D->UpdateTBC();
}

// change contrast of the scene
void FrMainWindow::contrastValueChanged(){    
    // should be in the range [-1..+1]
    double value = m_lowPanel->GetContrastValue();
    value /= 100.0;

    //QString message("Contrast Value: ");
    //message += QString::number(value);
    //QMessageBox::information(this, "Info", message);

    m_view2D->SetContrast(value);
    m_view2D->UpdateTBC();
}

// change threshold of the scene
void FrMainWindow::thresholdValueChanged(){
    // should be in the range [0..+1]
    double value = m_lowPanel->GetThresholdValue();
    value /= 100.0;

    //QString message("Threshold Value: ");
    //message += QString::number(value);
    //QMessageBox::information(this, "Info", message);

    m_view2D->SetThreshold(value);
    m_view2D->UpdateTBC();
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
        m_controller->LoadImage(fileName);
    }
}