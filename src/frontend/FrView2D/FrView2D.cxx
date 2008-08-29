#include "FrView2D.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrTBCFilter.h"

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
	m_imageViewer->SetupInteractor(rwi);

    // create renderer
    m_renderer = m_imageViewer->GetRenderer();
	m_renderer->SetBackground( 0.1, 0.2, 0.4 );

    // Create filter
    m_tbcFilter = FrTBCFilter::New();
    m_actor = vtkImageActor::New();
}

FrView2D::~FrView2D(){

    if(m_renderer) m_renderer->Delete();
    if(m_imageViewer) m_imageViewer->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
    if(m_actor) m_actor->Delete();
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
	
    // Clear scene
    m_renderer->RemoveAllViewProps();
    m_renderer->ResetCamera();
    m_imageViewer->GetRenderWindow()->Render();
    
    FrMainDocument* document = m_mainWindow->GetDocument();
    if(document){
        typedef std::vector<FrDocumentObj*> ImageVector;
        ImageVector images;
        document->GetAllImages(images); 
        // TODO: use DocumentReader here instead!!!
        
        ImageVector::iterator it(images.begin()), itEnd(images.end());
        if(it != itEnd){
            // Update if needed
            FrImageDocObj* img = reinterpret_cast<FrImageDocObj*>(*it);
            if(img->IsUpdateNeeded()){
                img->UpdateObject();
            }

            // Threshold/brightness/contrast filter
            m_tbcFilter->SetThreshold(document->GetThreshold());
            m_tbcFilter->SetBrightness(document->GetBrightness());
            m_tbcFilter->SetContrast(document->GetContrast());
            m_tbcFilter->SetInput(img->GetImageData());
            m_tbcFilter->Update();

            
            m_actor->SetInput(m_tbcFilter->GetOutput());
            m_renderer->AddActor(m_actor);

            // set image in the center of screen
            double* center = m_actor->GetCenter();
            m_actor->AddPosition(-center[0], -center[1], 0);
            m_renderer->GetActiveCamera()->SetParallelScale(100);
        } 
        // redraw scene
        m_imageViewer->GetRenderWindow()->Render();
    }
}

void FrView2D::UpdateTBC(){
    // Just update Threshold/brightness/contrast
    FrMainDocument* document = m_mainWindow->GetDocument();
    if(document){
        m_tbcFilter->SetThreshold(document->GetThreshold());
        m_tbcFilter->SetBrightness(document->GetBrightness());
        m_tbcFilter->SetContrast(document->GetContrast());

        // Do update only if has valid input
        if(m_tbcFilter->GetInput()){
            m_tbcFilter->Update();

            // set new input and redraw scene
            m_actor->SetInput(m_tbcFilter->GetOutput());
            m_imageViewer->GetRenderWindow()->Render();
        }
    }
}

void FrView2D::SetThreshold(double value){
    m_tbcFilter->SetThreshold(value);
}

void FrView2D::SetBrightness(double value){
    m_tbcFilter->SetBrightness(value);
}

void FrView2D::SetContrast(double value){
    m_tbcFilter->SetContrast(value);
}