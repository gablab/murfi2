#include "FrMainWindow.h"

#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"

FrMainWindow::FrMainWindow(QWidget *parent): QMainWindow(parent){
	setupUi(this);
	
	myTabWidget = new QTabWidget(this);
	slice2DWidget = new QWidget;
	graphWidget = new QWidget;
	QGroupBox* groupBox = new QGroupBox(this);
//	QWidget* groupBoxWidget = new QWidget(groupBox);
//	groupBoxWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QHBoxLayout *groupBoxLayout = new QHBoxLayout(groupBox);
	//groupBoxLayout->setContentsMargins(0, 0, 0, 0);
	groupBoxLayout->setSpacing(0);

	myQVTKWidget = new QVTKWidget(groupBox);
	myQVTKWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	myBookmarkWidget = new FrBookmarkWidget(groupBox);
	myBookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

//----------------------------- test area --------------------
	vtkPNGReader* reader = vtkPNGReader::New();
	reader->SetDataScalarTypeToUnsignedChar();
	reader->SetFileName("test.png");

	vtkImageViewer2* image_view = vtkImageViewer2::New();
	image_view->SetSize(640, 480);
	myQVTKWidget->SetRenderWindow(image_view->GetRenderWindow());

    vtkRenderWindowInteractor* rwi = myQVTKWidget->GetRenderWindow()->GetInteractor();
    vtkInteractorStyleImage* style = vtkInteractorStyleImage::New();
    rwi->SetInteractorStyle(style);
	image_view->SetupInteractor(rwi);

	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground( 0.1, 0.2, 0.4 );
	vtkImageActor *image = vtkImageActor::New();

	image->SetInput(reader->GetOutput());
	image_view->GetRenderWindow()->AddRenderer(ren1);

	ren1->AddActor(image);

	image_view->SetColorLevel(138.5);
	image_view->SetColorWindow(233);
//----------------------------- test area --------------------


	myToolsPanel = new FrToolsPanel(this);
	myToolsPanel->setFixedWidth(myToolsPanel->width());
	myToolsPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

//	scrollAreaOfToolsPanel = new QScrollArea;
//	scrollAreaOfToolsPanel->setWidget(myToolsPanel);
//	scrollAreaOfToolsPanel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	scrollAreaOfToolsPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	myLowPanel = new FrLowPanel(this);
	myLowPanel->setFixedHeight(myLowPanel->height());

	QVBoxLayout *verticalLayout = new QVBoxLayout;
	verticalLayout->setContentsMargins(0, 0, 0, 0);

//	verticalLayout->addWidget(myBookmarkWidget);
//	verticalLayout->addWidget(myQVTKWidget);
	
	groupBoxLayout->addWidget(myQVTKWidget);

//	groupBoxLayout->addWidget(tabWidget);
	groupBoxLayout->addWidget(myBookmarkWidget);

	verticalLayout->addWidget(groupBox);
//	verticalLayout->addLayout(groupBoxLayout);
	verticalLayout->addWidget(myLowPanel);

	// horizontal layout
	QHBoxLayout *layout = new QHBoxLayout;
//	layout->addWidget(scrollAreaOfToolsPanel);
	layout->addWidget(myToolsPanel);
	//layout->addWidget(myQVTKWidget);
	layout->addLayout(verticalLayout);
//	layout->addWidget(myBookmarkWidget);

//	verticalLayout->addLayout(layout);
	
//	QHBoxLayout *bottomLayout = new QHBoxLayout;
//    QSpacerItem* spacerItem = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
//	bottomLayout->setContentsMargins(0, 0, 0, 0);
//	bottomLayout->addWidget(myLowPanel);
////	bottomLayout->addItem(spacerItem);
//
//	verticalLayout->addLayout(bottomLayout);

	// central widget
//	QWidget *centralWidget = new QWidget;
//	centralWidget->setLayout(layout);

//	setCentralWidget(centralWidget);
	
	slice2DWidget->setLayout(layout);
	myTabWidget->addTab(slice2DWidget, "2D Slice View");
	myTabWidget->addTab(graphWidget, "Graphs and calculations");
	setCentralWidget(myTabWidget);

	// actions of File-menu
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	// actions of Help-menu
	connect(actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	// toolbar actions
	connect(actionTool1, SIGNAL(triggered()), this, SLOT(tool1Triggered()));
	connect(actionTool2, SIGNAL(triggered()), this, SLOT(tool2Triggered()));
	connect(actionTool3, SIGNAL(triggered()), this, SLOT(tool3Triggered()));
	connect(actionSaveToTab, SIGNAL(triggered()), this, SLOT(saveToTab()));

	// actions of tools panel
	connect(myToolsPanel->mode1Button, SIGNAL(clicked()), this, SLOT(mode1Clicked()));
	connect(myToolsPanel->mode2Button, SIGNAL(clicked()), this, SLOT(mode2Clicked()));
	connect(myToolsPanel->mode3Button, SIGNAL(clicked()), this, SLOT(mode3Clicked()));

	// actions of Low panel
	connect(myLowPanel->brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(brightnessValueChanged()));
	connect(myLowPanel->contrastSlider, SIGNAL(sliderReleased()), this, SLOT(contrastValueChanged()));
	connect(myLowPanel->thresholdSlider, SIGNAL(sliderReleased()), this, SLOT(thresholdValueChanged()));

	// actions of Tab widget
	connect(myTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	// actions of Bookmark Widget
	connect(myBookmarkWidget, SIGNAL(currentChanged(int)), this, SLOT(bookmarkChanged(int)));

}

// change brightness of the scene
void FrMainWindow::brightnessValueChanged(){
}

// change contrast of the scene
void FrMainWindow::contrastValueChanged(){
}

// change threshold of the scene
void FrMainWindow::thresholdValueChanged(){
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
	myBookmarkWidget->addBookmark();

}