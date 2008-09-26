#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrNotify.h"
#include "Fr2DSliceActor.h"
#include "FrSliceExtractor.h"
#include "FrTabSettingsDocObj.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkTextActor.h"
#include "vtkRenderWindow.h"
#include "vtkCoordinate.h"
#include "vtkTextProperty.h"


FrSliceExtractor::Orientation g_Orientation[RENDERER_COUNT-1];

// Default constructor
FrOrthoView::FrOrthoView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
       
    m_docReader = FrDocumentReader::New();

    // Init orientations
    g_Orientation[CORONAL_RENDERER] = FrSliceExtractor::YZ;
    g_Orientation[SAGITAL_RENDERER] = FrSliceExtractor::XZ;
    g_Orientation[AXIAL_RENDERER]   = FrSliceExtractor::XY;

    // create renderers
	for(int i = 0; i < RENDERER_COUNT; i++){
        m_renderer[i] = vtkRenderer::New();
		m_actor[i] = Fr2DSliceActor::New();
		m_tactor[i] = vtkTextActor::New();
		m_SliceExtractor[i] = FrSliceExtractor::New();
		m_tbcFilter[i] = FrTBCFilter::New();	
    }
}

FrOrthoView::~FrOrthoView(){
    if(m_docReader) m_docReader->Delete();

    for(int i = 0; i < RENDERER_COUNT; i++){
        if(m_renderer[i]) m_renderer[i]->Delete();
		if(m_actor[i]) m_actor[i]->Delete();
		if(m_tactor[i]) m_tactor[i]->Delete();
		if(m_SliceExtractor[i]) m_SliceExtractor[i]->Delete();
		if(m_tbcFilter[i]) m_tbcFilter[i]->Delete();	
    }    
}

void FrOrthoView::Initialize()
{
    // create renderers
	for(int i = 0; i < RENDERER_COUNT; i++){        
        m_renderer[i]->GetActiveCamera()->ParallelProjectionOn();
        m_renderer[i]->SetBackground( 0.0, 0.0, 0.0 );
    }

    SetupRenderers();
}

void FrOrthoView::SetupRenderers(){
    
    RemoveRenderers();
	GetRenderWindow()->Render();
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[CORONAL_RENDERER]);
    m_renderer[CORONAL_RENDERER]->SetViewport(0.0, 0.5, 0.5, 1.0);
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[SAGITAL_RENDERER]);
	m_renderer[SAGITAL_RENDERER]->SetViewport(0.5, 0.5, 1.0, 1.0);
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[AXIAL_RENDERER]);
	m_renderer[AXIAL_RENDERER]->SetViewport(0.0, 0.0, 0.5, 0.5);

    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[DUMMY_RENDERER]);
	m_renderer[DUMMY_RENDERER]->SetViewport(0.5, 0.0, 1.0, 0.5);
}

void FrOrthoView::RemoveRenderers(){

    // Remove all renderers
    for (int i = 0; i < RENDERER_COUNT; i++) {      
        QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer[i]);
    }
    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrOrthoView::UpdatePipeline(int point){

    // TODO: implement!!!
    bool isCleared = false;
    vtkCamera* cam = 0L;
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    OViewSettings& viewSettings = document->GetCurrentTabSettings()->GetOrthoViewSettings();
    	
	int clickedRenderer = -1;
	double* local;
	double localx, localy, localz;
	double* bounds;
	double maxX, maxY;
	int coor[3];

	char text[50] = "";
	char tmp[10];

    switch(point){
    case FRP_FULL:
        {
            // Clear scene
            for(int i = 0; i < RENDERER_COUNT; i++){
                m_renderer[i]->RemoveAllViewProps();
                m_renderer[i]->ResetCamera();
            }
			GetRenderWindow()->Render();
			
            isCleared = true;
        }
    case FRP_READIMAGE:
        {
            m_docReader->SetDocument(document);
            m_docReader->SetUnMosaicOn(true);
            m_docReader->Update();

            const char* tactorText[RENDERER_COUNT-1];
            tactorText[CORONAL_RENDERER] = "Coronal";
            tactorText[SAGITAL_RENDERER] = "Sagital";
            tactorText[AXIAL_RENDERER]   = "Axial";
			for(int i = 0; i < RENDERER_COUNT-1; i++){
				m_SliceExtractor[i]->SetInput(m_docReader->GetOutput());
				m_SliceExtractor[i]->SetOrientation(g_Orientation[i]);

				// set text actors                
				m_tactor[i]->GetTextProperty()->SetColor(100, 100, 0);
				m_tactor[i]->GetTextProperty()->SetBold(5);
				m_tactor[i]->GetTextProperty()->SetFontSize(20);
				m_tactor[i]->SetPosition(20, 20);
                m_tactor[i]->SetInput(tactorText[i]);
				m_renderer[i]->AddActor(m_tactor[i]);	
			}
        }
    case FRP_SLICE:
        {
            int* sliceNumber[RENDERER_COUNT-1];
            sliceNumber[CORONAL_RENDERER] = &viewSettings.CoronalSlice;
            sliceNumber[SAGITAL_RENDERER] = &viewSettings.SagitalSlice;
            sliceNumber[AXIAL_RENDERER]   = &viewSettings.AxialSlice;

            // Need this to allow working process
            for(int i=0; i < RENDERER_COUNT-1; ++i){
                // Setup slice                 
                if(m_SliceExtractor[i]->GetInput() != 0L){
                    m_SliceExtractor[i]->SetSlice( (*sliceNumber[i]) );
                    m_SliceExtractor[i]->Update();
				    m_tbcFilter[i]->SetInput(m_SliceExtractor[i]->GetOutput());
                }
            }
		}   
	case FRP_TBC:
        {
            for(int i = 0; i < RENDERER_COUNT-1; i++){
				// Setup brightness contrast
                m_tbcFilter[i]->SetThreshold(viewSettings.TbcSettings[i].Threshold);
                m_tbcFilter[i]->SetBrightness(viewSettings.TbcSettings[i].Brightness);
                m_tbcFilter[i]->SetContrast(viewSettings.TbcSettings[i].Contrast);
                viewSettings.TbcSettings[i].Threshold = m_tbcFilter[i]->GetThreshold();
                viewSettings.TbcSettings[i].Brightness = m_tbcFilter[i]->GetBrightness();
                viewSettings.TbcSettings[i].Contrast = m_tbcFilter[i]->GetContrast();

				if(m_tbcFilter[i]->GetInput()){                
					m_tbcFilter[i]->Update();
					m_actor[i]->SetInput(m_tbcFilter[i]->GetOutput());

                    // Add actors if they were removed
                    if(isCleared){
                        m_renderer[i]->AddActor(m_actor[i]);
					}
				}
            }
        }
    case  FRP_SETCAM:
        {   
            for(int i=0; i < RENDERER_COUNT; ++i){
                cam = m_renderer[i]->GetActiveCamera();
                cam->ParallelProjectionOn();
                cam->SetParallelScale(viewSettings.CamSettings[i].Scale);
                cam->SetFocalPoint(viewSettings.CamSettings[i].FocalPoint);
                cam->SetViewUp(viewSettings.CamSettings[i].ViewUp);
                cam->SetPosition(viewSettings.CamSettings[i].Position);
            }
        }
    default:
        // do nothing
        break;
    }
    GetRenderWindow()->Render();
}

// Commented
//// get absolute mouse position
//x = document->GetXCoord();
//y = document->GetYCoord();
//
//// search for renderer that was activated
//for (int i = 0; i < RENDERER_COUNT-1; i++){
//	if (m_renderer[i]->IsInViewport(x, y))
//		clickedRenderer = i;
//}
//
//// get local coordinates for found renderer
//if (clickedRenderer != -1){
//	local = GetLocalRendererPoint(m_renderer[clickedRenderer], x, y);
//	localx = local[0];
//	localy = local[1];
//
//	// output some slice info in 4th renderer
//	//strcat(text, "X = ");
//	//itoa(localx, tmp, 10);
//	//strcat(text, tmp);
//	//strcat(text, " , Y = ");
//	//itoa(localy, tmp, 10);
//	//strcat(text, tmp);
//	//strcat(text, " , Z = ");
//	//itoa(coor[2], tmp, 10);
//	//strcat(text, tmp);
//	//
//	//m_tactor[3]->SetInput(text);
//
//	for(int i = 0; i < RENDERER_COUNT-1; ++i){
//		if(i != clickedRenderer){
//			switch(i){
//				case CORONAL_RENDERER: 
//					m_SliceExtractor[i]->SetSlice(GetCoronalSlice(clickedRenderer, localx, localy));
//					break;
//				case SAGITAL_RENDERER:
//					m_SliceExtractor[i]->SetSlice(GetSagitalSlice(clickedRenderer, localx, localy));
//					break;
//				case AXIAL_RENDERER:
//					m_SliceExtractor[i]->SetSlice(GetAxialSlice(clickedRenderer, localx, localy));
//					break;
//			}
//		}
//	}
//}

double* FrOrthoView::GetLocalRendererPoint(vtkRenderer *ren, int x, int y){
	vtkCoordinate* coord = vtkCoordinate::New();
	coord->SetCoordinateSystemToDisplay();
	coord->SetValue(x, y, 0.0);
	double* out = coord->GetComputedWorldValue(ren);

	coord->Delete();

	return out;
}

//int FrOrthoView::GetCoronalSlice(int ren, int dx, int dy){
//	double* bounds = m_actor[ren]->GetBounds();	
//	int max, slice;			
//
//	switch(ren)
//	{
//		case AXIAL_RENDERER:
//			max = bounds[3];
//			slice = dy;
//			break;
//		case SAGITAL_RENDERER:
//			max = bounds[1];
//			slice = dx;
//			break;
//	}
//
//	if (slice > max)
//		slice = max;
//	if (slice < 0)
//		slice = 0;
//
//	return slice;
//}

int FrOrthoView::GetSagitalSlice(int ren, int dx, int dy){
	double* bounds = m_actor[ren]->GetBounds();	
	int max, slice;			

	switch(ren)
	{
		case AXIAL_RENDERER:
			max = bounds[1];
			slice = dx;
			break;
		case CORONAL_RENDERER:
			max = bounds[1];
			slice = dx;
			break;
	}

	if (slice > max)
		slice = max;
	if (slice < 0)
		slice = 0;

	return slice;
}

int FrOrthoView::GetAxialSlice(int ren, int dx, int dy){
	double* bounds = m_actor[ren]->GetBounds();	
	int max, slice;			

	max = bounds[3];
	slice = dy;		

	if (slice > max)
		slice = max;
	if (slice < 0)
		slice = 0;

	return slice;
}