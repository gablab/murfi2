#include "FrVoxelTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

// Qt stuff
#include "Qt/QString.h"
#include "Qt/QMessageBox.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkPointPicker.h"

// Some defines 
#define DEF_TOLERANCE 0.0
#define BAD_POINT_INDEX -1


FrVoxelTool::FrVoxelTool()
: m_pointPicker(0) {
    m_pointPicker = vtkPointPicker::New();
    m_pointPicker->SetTolerance(DEF_TOLERANCE);
}

FrVoxelTool::~FrVoxelTool(){
    if(m_pointPicker) m_pointPicker->Delete();
}

void FrVoxelTool::Start(){
    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrVoxelTool::Stop(){
    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrVoxelTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
//    QMessageBox::information(0, QString("Tool info"), QString("Voxel tool is working"));
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
        cmd->SetAction(FrVoxelInfoCmd::Update);
        cmd->SetPointPicker(m_pointPicker);
        cmd->SetMouseXY(params.X, params.Y);
        cmd->Execute();
        delete cmd;
    }
	return false;
}

bool FrVoxelTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
        cmd->SetAction(FrVoxelInfoCmd::Update);
        cmd->SetPointPicker(m_pointPicker);
        cmd->SetMouseXY(params.X, params.Y);
        cmd->Execute();
        delete cmd;
    }
    return false;
}

int FrVoxelTool::GetPickedPointIndex(int x, int y, vtkRenderer* renderer){
    if (!m_pointPicker->Pick(x, y, 0, renderer)) {
        return BAD_POINT_INDEX;
    }
    
    int id = m_pointPicker->GetPointId();
    return id;
  
	//// Get the mapped position of the mouse using the picker.
 //   double ptMapped[3];
 //   m_pointPicker->GetMapperPosition(ptMapped);

 //   // We have to manually set the physical z-coordinate which requires
 //   // us to get the volume spacing.
 //   double dSpacing[3];
 //   m_pImageViewer->GetInput()->GetSpacing(dSpacing);			// get image data
 //   ptMapped[2] = m_pImageViewer->GetSlice() * dSpacing[2];		// get slice number

 //   // Get a shortcut to the pixel data.
 //   vtkImageData* pImageData = m_pImageViewer->GetInput();		// get image data

 //   // Get the volume index within the entire volume now.
 //   int nVolIdx = pImageData->FindPoint(ptMapped);

 //   // We have to handle different number of scalar components.
 //   switch (pImageData->GetNumberOfScalarComponents()) {
 //       case 1:
 //           {
 //               unsigned short* pPix = (unsigned short*)pImageData->GetScalarPointer();
 //               unsigned short usPix = pPix[nVolIdx];
	//	       // FmtStr(m_strDetails, "Pixel val = [" << usPix << "]   at index [" << nVolIdx << "],  coordinates(" << ptMapped[0] << "," << ptMapped[1] << ")");
 //           }
 //           break;
 //       
 //       case 3:
 //           {
 //               unsigned char* pPix = (unsigned char*)pImageData->GetScalarPointer();
 //               unsigned char usPixR = pPix[nVolIdx * 3 + 0];
 //               unsigned char usPixG = pPix[nVolIdx * 3 + 1];
 //               unsigned char usPixB = pPix[nVolIdx * 3 + 2];
 //               //FmtStr(m_strDetails, "Pixel val = [" << (int)usPixR << "," << (int)usPixG << "," << (int)usPixB << "]   at index [" << nVolIdx << "],  coordinates(" << ptMapped[0] << "," << ptMapped[1] << ")");
 //           }
 //           break;

 //       default:
 //           //FmtStr(m_strDetails, "Unsupported number of scalar components [" << pImageData->GetNumberOfScalarComponents() << "] for pixel picker.");
 //           break;
 //   }

 //   return nVolIdx;
}