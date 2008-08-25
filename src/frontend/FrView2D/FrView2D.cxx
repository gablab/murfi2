#include "FrView2D.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"

// Qt
#include "Qt/QWidget.h"
#include "QtGUI/QSizePolicy.h"

// VTK
#include "vtkImageViewer2.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkPNGReader.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"
#include "vtkCollectionIterator.h"


// Default constructor
FrView2D::FrView2D(FrMainWindow* mainWindow, QWidget* parent)
: m_mainWindow(mainWindow) {
   
    // Create qt widget to render
    m_qtView = new QVTKWidget(parent);
    m_qtView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // create image viewer
    m_imageViewer = vtkImageViewer2::New();
	m_imageViewer->SetSize(640, 480);
    m_imageViewer->SetColorLevel(138.5);
	m_imageViewer->SetColorWindow(233);
	m_qtView->SetRenderWindow(m_imageViewer->GetRenderWindow());

    // set interactor
    vtkRenderWindowInteractor* rwi = m_qtView->GetRenderWindow()->GetInteractor();
    //vtkInteractorStyleImage* style = vtkInteractorStyleImage::New();
    //rwi->SetInteractorStyle(style);
    //style->Delete();
	m_imageViewer->SetupInteractor(rwi);

    // create renderer
    m_renderer = m_imageViewer->GetRenderer();
	m_renderer->SetBackground( 0.1, 0.2, 0.4 );
}

FrView2D::~FrView2D(){
    if(m_renderer) m_renderer->Delete();
    if(m_imageViewer) m_imageViewer->Delete();
}

QWidget* FrView2D::GetWidget(){
    return m_qtView;
}

void FrView2D::SetInteractorStyle(vtkInteractorStyle* style){

    if(style && m_qtView->GetInteractor()){
        m_qtView->GetInteractor()->SetInteractorStyle(style);
    }
}

void FrView2D::UpdateScene(){
	
    // Read from file
    vtkPNGReader* reader = vtkPNGReader::New();
	reader->SetDataScalarTypeToUnsignedChar();
	reader->SetFileName("test.png");
	
	vtkImageActor *image = vtkImageActor::New();
	image->SetInput(reader->GetOutput());

	m_renderer->AddActor(image);
    m_imageViewer->GetRenderWindow()->Render();

	reader->Delete();
    image->Delete();
}