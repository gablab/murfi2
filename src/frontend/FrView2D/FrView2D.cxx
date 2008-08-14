#include "FrView2D.h"
#include "QVTKWidget.h"

// Qt
#include "Qt/QWidget.h"
#include "QtGUI/QSizePolicy.h"

// VTK
#include "vtkImageViewer2.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkPNGReader.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"


// Default constructor
FrView2D::FrView2D(QWidget* parent) {
    // take all available space
    m_view = new QVTKWidget(parent);    
    m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //----------------------------- test area --------------------
	vtkPNGReader* reader = vtkPNGReader::New();
	reader->SetDataScalarTypeToUnsignedChar();
	reader->SetFileName("test.png");

	vtkImageViewer2* imageView = vtkImageViewer2::New();
	imageView->SetSize(640, 480);
	m_view->SetRenderWindow(imageView->GetRenderWindow());
   
	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground( 0.1, 0.2, 0.4 );
	vtkImageActor *image = vtkImageActor::New();

	image->SetInput(reader->GetOutput());
	imageView->GetRenderWindow()->AddRenderer(ren1);

	ren1->AddActor(image);

	imageView->SetColorLevel(138.5);
	imageView->SetColorWindow(233);
    //----------------------------- test area --------------------
}

FrView2D::~FrView2D(){

}

QWidget* FrView2D::GetWidget(){
    return m_view;
}

void FrView2D::SetInteractorStyle(vtkInteractorStyle* style){
    if(style && m_view->GetInteractor()){
        m_view->GetInteractor()->SetInteractorStyle(style);
    }
}