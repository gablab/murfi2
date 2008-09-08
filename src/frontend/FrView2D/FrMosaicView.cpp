#include "FrMosaicView.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrTBCFilter.h"
#include "FrMosaicFilter.h"
#include "FrInteractorStyle.h"

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
#include "vtkImageData.h"
#include "vtkImageReslice.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"
#include "vtkBMPWriter.h"
#include "vtkImageWriter.h"
#include "vtkDataWriter.h"
#include "vtkImagePadFilter.h"
#include "vtkPointData.h"



// Default constructor
FrMosaicView::FrMosaicView(FrMainWindow* mainWindow, QWidget* parent)
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
	m_renderer = vtkRenderer::New();//m_imageViewer->GetRenderer();
	//m_imageViewer->GetRenderWindow()->AddRenderer(m_renderer);
	m_imageViewer->SetRenderer(m_renderer);
	m_renderer->SetBackground( 0.1, 0.2, 0.4 );

    // Create filter
    m_tbcFilter = FrTBCFilter::New();
    m_actor = vtkImageActor::New();

	m_slice = 0;
}

FrMosaicView::~FrMosaicView(){

    if(m_renderer) m_renderer->Delete();
    if(m_imageViewer) m_imageViewer->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
    if(m_actor) m_actor->Delete();
}

QWidget* FrMosaicView::GetWidget(){
    return m_qtView;
}

void FrMosaicView::SetInteractorStyle(vtkInteractorStyle* style){

    if(style && m_qtView->GetInteractor()){
        m_qtView->GetInteractor()->SetInteractorStyle(style);
    }
}

void FrMosaicView::UpdateScene(){
	
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
            vtkImageData* data = img->GetImageData();
			matrixSize = img->GetMatrixSize();
			int oldDims[3];
			data->GetDimensions(oldDims);
			numSlices = (oldDims[0]/matrixSize)*(oldDims[1]/matrixSize);

			FrMosaicFilter* m_MosaicFilter = FrMosaicFilter::New();
			m_MosaicFilter->SetInput(data);
			m_MosaicFilter->SetOutputDimensions(matrixSize, matrixSize, numSlices);
				
			//vtkImageWriter* writer = vtkImageWriter::New();
			//writer->SetFileName("test.img");
			//writer->SetFileDimensionality(3);
			//writer->SetInput(m_MosaicFilter->GetOutput());
			//writer->Write();

			m_tbcFilter->SetInput(m_MosaicFilter->GetOutput());
            m_tbcFilter->Update();

            m_actor->SetInput(m_tbcFilter->GetOutput());
			m_tbcFilter->GetOutput()->GetDimensions(m_dims);
			m_actor->SetDisplayExtent(0, m_dims[0]-1, 0, m_dims[1]-1, m_slice, m_slice);
 
			m_renderer->AddActor(m_actor);
			//m_imageViewer->GetRenderWindow()->AddRenderer(m_renderer);
			//m_imageViewer->GetRenderer()->AddActor(m_actor);

			// set image in the center of screen
            double* center = m_actor->GetCenter();
            m_actor->AddPosition(-center[0], -center[1], 0);
			m_renderer->GetActiveCamera()->ParallelProjectionOn();
			m_renderer->GetActiveCamera()->SetParallelScale(120);
			
			//FrInteractorStyle* is = (FrInteractorStyle*)m_qtView->GetInteractor()->GetInteractorStyle();
			//is->CurrentRenderer->GetActiveCamera()->SetParallelScale(100);
			//m_imageViewer->GetInteractorStyle()->GetCurrentRenderer()->GetActiveCamera()->SetParallelScale(100);
			//m_imageViewer->GetRenderer()->GetActiveCamera()->SetParallelScale(100);
        } 
        // redraw scene
        m_imageViewer->GetRenderWindow()->Render();
    }
}

void FrMosaicView::UpdateTBC(){
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

void FrMosaicView::UpdateSlice(){
    FrMainDocument* document = m_mainWindow->GetDocument();
    if(document){
		// check input data
		int slice = document->GetSlice();
		m_slice += slice;
		if (m_slice>numSlices)
			m_slice = numSlices;
		if (m_slice<0)
			m_slice = 0;
		
		
		m_actor->SetDisplayExtent(0, m_dims[0]-1, 0, m_dims[1]-1, m_slice, m_slice);
		//m_renderer->ResetCameraClippingRange();
		double scale = m_renderer->GetActiveCamera()->GetParallelScale();
		m_renderer->ResetCamera();
		m_renderer->GetActiveCamera()->SetParallelScale(scale);
		m_imageViewer->GetRenderWindow()->Render();
	}	
}