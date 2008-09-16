#include "FrSliceExtractor.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"
#include "vtkImageReslice.h"

FrSliceExtractor* FrSliceExtractor::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrSliceExtractor");
  
  if(result){
      return (FrSliceExtractor*)result;
  }

  return new FrSliceExtractor();
}

FrSliceExtractor::FrSliceExtractor(){
//	this->ImageVOI = NULL;
	this->ImageVOI = vtkExtractVOI::New();
	this->ImageReslice = vtkImageReslice::New();

	m_axis = 2; //	default orientation - 2 XY
	m_frame = 0;
}

void FrSliceExtractor::SetInput(vtkImageData *data){
    FrBaseFilter::SetInput(data);
	this->GetInput()->GetDimensions(m_Dims);

	this->ImageVOI->SetInput(data);
	this->ImageVOI->Update();
}

//void FrSliceExtractor::SetOutputDimensions(int w, int h, int num){
//	m_newDims[0] = w;
//	m_newDims[1] = h;
//	m_newDims[2] = num;
//}

void FrSliceExtractor::SetOutputOrientation(int axis){
	m_axis = axis;
	this->Modified();
}

void FrSliceExtractor::SetCurrentFrame(int frame){
	m_frame = frame;
	this->Modified();
}

// Use this to update output params
void FrSliceExtractor::ExecuteInformation() {
    FrBaseFilter::ExecuteInformation();

    vtkImageData *input = this->GetInput();
    vtkImageData *output = this->GetOutput();

    if (input==NULL) return;
    
    // Setup extent
//    output->SetDimensions(m_Dims[0], m_Dims[1], 1);
//    output->SetWholeExtent(output->GetExtent());
//    output->SetUpdateExtent(output->GetExtent());
        
    // Setup dimentions and other params
//    output->SetSpacing(input->GetSpacing());
//    output->SetNumberOfScalarComponents(1);
//    output->SetScalarTypeToUnsignedChar();
}

void FrSliceExtractor::SimpleExecute(vtkImageData* input, vtkImageData* output){
	
	output2 = vtkImageData::New();
	output2->SetDimensions(m_Dims[0], m_Dims[1], 1);
	
	switch (m_axis)
	{
		case 2:		// XY
			//this->ImageVOI->SetVOI(0, m_Dims[0]-1, 0, m_Dims[1]-1, m_frame, m_frame);
			ImageReslice->SetResliceAxesOrigin(0, 0, m_frame);
			ImageReslice->SetResliceAxesDirectionCosines(1,0,0, 0,1,0, 0,0,1);
			break;

		case 1:		// XZ
			//this->ImageVOI->SetVOI(0, m_Dims[0]-1, m_frame, m_frame, 0, m_Dims[2]-1);
			ImageReslice->SetResliceAxesOrigin(m_frame, 0, 0);
			ImageReslice->SetResliceAxesDirectionCosines(0,1,0, 0,0,1, 1,0,0);
			break;

		case 0:		// YZ
			//this->ImageVOI->SetVOI(m_frame, m_frame, 0, m_Dims[1]-1, 0, m_Dims[2]-1);
			ImageReslice->SetResliceAxesOrigin(0, m_frame, 0);
			ImageReslice->SetResliceAxesDirectionCosines(1,0,0, 0,0,1, 0,-1,0);
			break;

		default:
			// unrecognized orientation
			break;
	}
	
//	ImageVOI->Modified();
//	ImageVOI->Update();

//	vtkImageData* tmp = input;//ImageVOI->GetOutput();
	
	ImageReslice->SetInput(input);
	ImageReslice->SetOutputDimensionality(2);
	ImageReslice->InterpolateOn();
	ImageReslice->SetOutputSpacing(input->GetSpacing());
	//reslice->SetOutputExtent(0, m_Dims[0]-1, 0, m_Dims[1]-1, 0, m_Dims[2]-1);
	ImageReslice->Update();

	output2 = ImageReslice->GetOutput();
	//output2 = ImageVOI->GetOutput();
}

vtkImageData* FrSliceExtractor::GetOutput(){
	return output2;	
}

int FrSliceExtractor::GetMaxSliceNumber(){
	int maxSlice = m_Dims[2];
	
	switch (m_axis)
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