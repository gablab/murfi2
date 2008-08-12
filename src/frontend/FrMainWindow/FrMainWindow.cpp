#include "FrMainWindow.h"

FrMainWindow::FrMainWindow(QWidget *parent): QMainWindow(parent){
	setupUi(this);
	
	myQVTKWidget = new QVTKWidget(this);
	myQVTKWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//----------------------------- test area --------------------
	vtkPNGReader* reader = vtkPNGReader::New();
	reader->SetDataScalarTypeToUnsignedChar();
	reader->SetFileName("test.png");

	vtkImageViewer2* image_view = vtkImageViewer2::New();
	image_view->SetSize(640, 480);
	myQVTKWidget->SetRenderWindow(image_view->GetRenderWindow());
	image_view->SetupInteractor(myQVTKWidget->GetRenderWindow()->GetInteractor());

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

//	scrollAreaOfToolsPanel = new QScrollArea;
//	scrollAreaOfToolsPanel->setWidget(myToolsPanel);
//	scrollAreaOfToolsPanel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	scrollAreaOfToolsPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	myToolsPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	myBookmarkWidget = new FrBookmarkWidget(this);
	myBookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

//	QVBoxLayout *verticalLayout = new QVBoxLayout;
//	verticalLayout->addWidget(myBookmarkWidget);

	// horizontal layout
	QHBoxLayout *layout = new QHBoxLayout;
//	layout->addWidget(scrollAreaOfToolsPanel);
	layout->addWidget(myToolsPanel);
	layout->addWidget(myQVTKWidget);
	layout->addWidget(myBookmarkWidget);

//	verticalLayout->addLayout(layout);

	// central widget
	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(layout);

	setCentralWidget(centralWidget);



	// actions of File-menu
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	// actions of Help-menu
	connect(actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));



}