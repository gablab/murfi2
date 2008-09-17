#include "FrSliceExtractor.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"
#include "vtkImageReslice.h"

FrSliceExtractor* FrSliceExtractor::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrSliceExtractor");
  
  if(result)
      return (FrSliceExtractor*)result;

  return new FrSliceExtractor();
}

FrSliceExtractor::FrSliceExtractor(){
//	this->ImageVOI = vtkExtractVOI::New();
	this->m_Reslice = vtkImageReslice::New();

	m_Axis = 2; //	default orientation - 2 XY
	m_Frame = 0;
}

FrSliceExtractor::~FrSliceExtractor(){
    if(m_Input) m_Input->Delete();
    if(m_Output) m_Output->Delete();
}

void FrSliceExtractor::SetInput(vtkImageData *input){
	m_Input = input;
	m_Input->GetDimensions(m_Dims);

//	this->ImageVOI->SetInput(data);
//	this->ImageVOI->Update();
}

//void FrSliceExtractor::SetOutputDimensions(int w, int h, int num){
//	m_newDims[0] = w;
//	m_newDims[1] = h;
//	m_newDims[2] = num;
//}

//void FrSliceExtractor::SetOutputOrientation(int axis){
//	m_axis = axis;
//	this->Modified();
//}
//
//void FrSliceExtractor::SetCurrentFrame(int frame){
//	m_frame = frame;
//	this->Modified();
//}

void FrSliceExtractor::Update(){
	
	m_Output = vtkImageData::New();
	m_Output->SetDimensions(m_Dims[0], m_Dims[1], 1);
	
	switch (m_Axis)
	{
		case 2:		// XY
			//this->ImageVOI->SetVOI(0, m_Dims[0]-1, 0, m_Dims[1]-1, m_frame, m_frame);
			m_Reslice->SetResliceAxesOrigin(0, 0, m_Frame);
			m_Reslice->SetResliceAxesDirectionCosines(1,0,0, 0,1,0, 0,0,1);
			break;

		case 1:		// XZ
			//this->ImageVOI->SetVOI(0, m_Dims[0]-1, m_frame, m_frame, 0, m_Dims[2]-1);
			m_Reslice->SetResliceAxesOrigin(m_Frame, 0, 0);
			m_Reslice->SetResliceAxesDirectionCosines(0,1,0, 0,0,1, 1,0,0);
			break;

		case 0:		// YZ
			//this->ImageVOI->SetVOI(m_frame, m_frame, 0, m_Dims[1]-1, 0, m_Dims[2]-1);
			m_Reslice->SetResliceAxesOrigin(0, m_Frame, 0);
			m_Reslice->SetResliceAxesDirectionCosines(1,0,0, 0,0,1, 0,-1,0);
			break;

		default:
			// unrecognized orientation
			break;
	}
	
//	ImageVOI->Modified();
//	ImageVOI->Update();

//	vtkImageData* tmp = input;//ImageVOI->GetOutput();
	
	m_Reslice->SetInput(m_Input);
	m_Reslice->SetOutputDimensionality(2);
	m_Reslice->InterpolateOn();
	m_Reslice->SetOutputSpacing(m_Input->GetSpacing());
	//reslice->SetOutputExtent(0, m_Dims[0]-1, 0, m_Dims[1]-1, 0, m_Dims[2]-1);
	m_Reslice->Update();

	m_Output = m_Reslice->GetOutput();
	//output2 = ImageVOI->GetOutput();
}

//vtkImageData* FrSliceExtractor::GetOutput(){
//	return output2;	
//}

int FrSliceExtractor::GetMaxSliceNumber(){
	int maxSlice = m_Dims[2];
	
	switch (m_Axis)
	{
	case 2:		// XY
		maxSlice = m_Dims[2]; 
		break;
	case 1:		// XZ	
		maxSlice = m_Dims[1];
		break;
	case 0:		// YZ
		maxSlice = m_Dims[0];
		break;
	}
	
	return maxSlice;
}