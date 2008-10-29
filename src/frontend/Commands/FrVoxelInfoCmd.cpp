#include "FrVoxelInfoCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"
#include "FrTabSettingsDocObj.h"
#include "Fr2DSliceActor.h"
#include "FrUtils.h"
#include "FrVoxelInfoWidget.h"
#include "FrLayeredImage.h"


#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


//Some defines
#define INVALIDE_RENDERER_NUM -1


FrVoxelInfoCmd::FrVoxelInfoCmd()
: m_isMouseXY(false), m_Action(FrVoxelInfoCmd::Undefined), m_PointPicker(0) {
    
}

void FrVoxelInfoCmd::SetMouseXY(int x, int y){
    m_mouseX = x; m_mouseY = y;
    m_isMouseXY = true;
}

bool FrVoxelInfoCmd::Execute(){
    if(!this->GetMainController() ||
        m_Action == FrVoxelInfoCmd::Undefined ||
        !m_isMouseXY) return false;

    bool result = false;
    switch(m_Action){
        case Update:
            result = UpdateVoxelInfo();
            break;
        case Reset:
            result = ResetVoxelInfo();
            break;
        default:
            // Do nothing
            break;
    }
    return result;
}

bool FrVoxelInfoCmd::UpdateVoxelInfo(){
	// get renderer    
	FrMainWindow* mv = this->GetMainController()->GetMainView();
	FrMainDocument* md = this->GetMainController()->GetMainDocument();
	FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();
	
	std::vector<vtkRenderer*> renCollection;
	int imgNumber = -1;
	FrLayeredImage* lim;
    std::vector<FrLayerSettings*> layers;
	
	FrTabSettingsDocObj::View view = ts->GetActiveView();
	switch(view){
		case FrTabSettingsDocObj::View::SliceView:
			mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
			lim = mv->GetSliceView()->GetImage();
		    GetLayerSettings(ts->GetSliceViewSettings(), layers);
			break;
		case FrTabSettingsDocObj::View::MosaicView:
			mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
			lim = mv->GetMosaicView()->GetImage();
		    GetLayerSettings(ts->GetMosaicViewSettings(), layers);
			break;
		case FrTabSettingsDocObj::View::OrthoView:
			{
				FrOrthoView* ov =  mv->GetOrthoView();

				// Find Image where click's occured
				for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
					if (ov->GetImage(i)->GetRenderer()->IsInViewport(m_mouseX, m_mouseY)){
						imgNumber = i; 
						break;
					}
				}
				if (imgNumber != -1){
					ov->GetImage(imgNumber)->GetRenderers(renCollection);
					lim = ov->GetImage(imgNumber);
					GetLayerSettings(ts->GetOrthoViewSettings(), layers, imgNumber);
				}
				else{
					ResetVoxelInfo();
					return false;
				}
			}
			break;	
	}

	// we should get any layer with visible actor
	int visibleLayerNum = GetVisibleLayer(layers);
	vtkRenderer* renderer = renCollection[visibleLayerNum];	
	
	if (!m_PointPicker->Pick(m_mouseX, m_mouseY, 0, renderer)) {
        ResetVoxelInfo();
		return false;
    }

    // get image data
    vtkImageData* pImageData = lim->GetLayerByID(0)->GetInput();

    // Get the mapped position of the mouse using the picker.
    double ptMapped[3];
    m_PointPicker->GetMapperPosition(ptMapped);
    ptMapped[2] = 0;
    
    double dSpacing[3];	
	pImageData->GetSpacing(dSpacing);		

    // Get the volume index within the entire volume now.
    int nPointIdx = pImageData->FindPoint(ptMapped);
    
    // We have to handle different number of scalar components.
    unsigned short usPix;
    unsigned char usPixR;
    unsigned char usPixG;
    unsigned char usPixB;

	VoxelData vd;
	vd.name = "test";			// get layer (image?) name
	vd.timepoint = 44;			// get timepoint

    // TODO: fix for side values
	switch(view){
		case FrTabSettingsDocObj::View::SliceView:
			{
                // get slice number
				int slice = md->GetCurrentTabSettings()->
                    GetSliceViewSettings()->SliceNumber;	
	
                // set current indices of point
				vd.Index[0] = int((ptMapped[0]+1) / dSpacing[0]);
				vd.Index[1] = int((ptMapped[1]+1) / dSpacing[1]);
				vd.Index[2] = slice;							
                // and positions
				vd.Position[0] = vd.Index[0] * dSpacing[0];
				vd.Position[1] = vd.Index[1] * dSpacing[1];
				vd.Position[2] = vd.Index[3] * dSpacing[2];							
			}
			break;
		case FrTabSettingsDocObj::View::MosaicView:
			{
				vd.Index[0] = int((ptMapped[0]+1) / dSpacing[0]);
				vd.Index[1] = int((ptMapped[1]+1) / dSpacing[1]);
				vd.Index[2] = 0;

				vd.Position[0] = vd.Index[0] * dSpacing[0];
				vd.Position[1] = vd.Index[1] * dSpacing[1];
				vd.Position[2] = 0;							
			}
			break;
		case FrTabSettingsDocObj::View::OrthoView:
			{
				switch(imgNumber){
					case 0:			// coronal 
						{
                            // get slice number
							int slice = md->GetCurrentTabSettings()->
                                GetOrthoViewSettings()->CoronalSlice;	
			
							vd.Index[0] = int((ptMapped[0]+1) / dSpacing[0]);
							vd.Index[1] = slice;
							vd.Index[2] = int((ptMapped[1]+1) / dSpacing[2]);

							vd.Position[0] = vd.Index[0] * dSpacing[0];	
							vd.Position[1] = vd.Index[1] * dSpacing[1];
							vd.Position[2] = vd.Index[2]* dSpacing[2];
						}
						break;
					case 1:			// sagital
						{
							int slice = md->GetCurrentTabSettings()->
                                GetOrthoViewSettings()->SagitalSlice;	// get slice number
			
							vd.Index[0] = slice;			
							vd.Index[1] = int( (ptMapped[0]+1) / dSpacing[1]);
							vd.Index[2] = int( (ptMapped[1]+1) / dSpacing[2]);

							vd.Position[0] = vd.Index[0] * dSpacing[0];
							vd.Position[1] = vd.Index[1] * dSpacing[1];	
							vd.Position[2] = vd.Index[2] * dSpacing[2];					
						}
						break;
					case 2:			// axial
						{
							int slice = md->GetCurrentTabSettings()->
                                GetOrthoViewSettings()->AxialSlice;	// get slice number
			
							vd.Index[0] = int((ptMapped[0]+1) / dSpacing[0]);
							vd.Index[1] = int((ptMapped[1]+1) / dSpacing[1]);
							vd.Index[2] = slice;

							vd.Position[0] = vd.Index[0] * dSpacing[0];
							vd.Position[1] = vd.Index[1] * dSpacing[1];
							vd.Position[2] = vd.Index[2] * dSpacing[2];							
						}
						break;
				}
			}
			break;	
	}

	// Get voxel index, position
	// this should be done for all layers
//    GetLayerSettings(md->GetCurrentTabSettings()->GetSliceViewSettings(), layers);
	
    std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
    for (it = layers.begin(); it != itEnd; ++it){
        // get image data
    //    pImageData = mv->GetSliceView()->GetImage()->
      //      GetLayerByID( (*it)->ID )->GetInput();		
		pImageData = lim->GetLayerByID((*it)->ID)->GetInput();

		switch (pImageData->GetNumberOfScalarComponents()) {
			case 1:
				{
					unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
					usPix = pPix[nPointIdx];
				}
				break;
	        
			case 3:
				{
					unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
					usPixR = pPix[nPointIdx * 3 + 0];
					usPixG = pPix[nPointIdx * 3 + 1];
					usPixB = pPix[nPointIdx * 3 + 2];

					usPix = (usPixR + usPixG + usPixB)/3;
				}
				break;

			default:				
				break;
		}
		
		// Set table values
		Voxel v;
        v.name = (*it)->Name;
        
        // get voxel value from current layer
		v.value = usPix;
		vd.voxels.push_back(v);
	}

	mv->GetVoxelInfoWidget()->SetVoxelData(vd);
	
    return false;
}

bool FrVoxelInfoCmd::ResetVoxelInfo(){
	// reset main properties
	FrMainWindow* mv = this->GetMainController()->GetMainView();
	mv->GetVoxelInfoWidget()->Clear();	// clear table of voxel values

    return false;
}

void FrVoxelInfoCmd::GetVoxelInfo(){
}

int FrVoxelInfoCmd::GetVisibleLayer(std::vector<FrLayerSettings*> layers){
    std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
    for (it = layers.begin(); it != itEnd; ++it){
		if ((*it)->Visibility)
			return (*it)->ID;
	}
	
	return 0;		// there are no visible actors atm
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrVoxelInfoCmd::CanUndo(){
    return false;
}

bool FrVoxelInfoCmd::Undo(){
    return false;
}

bool FrVoxelInfoCmd::Redo(){
    return false;
}