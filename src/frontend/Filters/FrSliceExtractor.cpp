#include "FrSliceExtractor.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"
#include "vtkImageReslice.h"

// Defines
#define DEF_SPACING 1.0
#define DEF_FRAME 0


FrSliceExtractor* FrSliceExtractor::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrSliceExtractor");
  
  if(result)
      return (FrSliceExtractor*)result;

  return new FrSliceExtractor();
}

FrSliceExtractor::FrSliceExtractor(){
    // Create reslicer
	m_Reslicer = vtkImageReslice::New();
    
	m_Slice = DEF_FRAME;    
    m_Orientation = FrSliceExtractor::XY;
    m_Spacing[0] = m_Spacing[1] = m_Spacing[2] = DEF_SPACING;
}

FrSliceExtractor::~FrSliceExtractor(){
    if(m_Reslicer) m_Reslicer->Delete();
}

void FrSliceExtractor::SetInput(vtkImageData *input){
    if(input == m_Reslicer->GetInput()) return;
    m_Reslicer->SetInput(input);
    this->Modified();
    
    if(input != 0L){
        input->GetSpacing(m_Spacing);
    }
    else {
        m_Spacing[0] = m_Spacing[1] = m_Spacing[2] = DEF_SPACING;
    }
}

vtkImageData* FrSliceExtractor::GetInput(){
    return (vtkImageData*)m_Reslicer->GetInput();
}

vtkImageData* FrSliceExtractor::GetOutput(){    
    return m_Reslicer->GetOutput();
}

void FrSliceExtractor::SetOrientation(FrSliceExtractor::Orientation orient){
    if(m_Orientation != orient){
        m_Orientation = orient;
        this->Modified();
    }
}

void FrSliceExtractor::SetSlice(int slice){
    if(m_Slice != slice){
        m_Slice = slice;
	    this->Modified();
    }
}

void FrSliceExtractor::Update(){
    // For safty
    if(!m_Reslicer->GetInput()) return;

	switch (m_Orientation)
	{
    case FrSliceExtractor::XY:		// XY
			m_Reslicer->SetResliceAxesOrigin(0, 0, m_Slice);
			m_Reslicer->SetResliceAxesDirectionCosines(1,0,0, 0,1,0, 0,0,1);
			break;

    case FrSliceExtractor::XZ:		// XZ
			m_Reslicer->SetResliceAxesOrigin(m_Slice, 0, 0);
			m_Reslicer->SetResliceAxesDirectionCosines(0,1,0, 0,0,1, 1,0,0);
			break;

    case FrSliceExtractor::YZ:		// YZ
			m_Reslicer->SetResliceAxesOrigin(0, m_Slice, 0);
			m_Reslicer->SetResliceAxesDirectionCosines(1,0,0, 0,0,1, 0,-1,0);
			break;

		default:
			// unrecognized orientation
			break;
	}
	
    m_Reslicer->InterpolateOn();
	m_Reslicer->SetOutputDimensionality(2);
	m_Reslicer->SetOutputSpacing(m_Spacing);
	m_Reslicer->Update();
}

int FrSliceExtractor::GetMaxSliceNumber(){
    int result = -1;

    if(m_Reslicer->GetInput() != 0L){
        int dimentions[3];
        vtkImageData* img = (vtkImageData*)m_Reslicer->GetInput();
        img->GetDimensions( dimentions);	
	
	    switch (m_Orientation)
	    {
	    case FrSliceExtractor::XY:		// XY
		    result = dimentions[2]; 
		    break;
	    case FrSliceExtractor::XZ:		// XZ	
		    result = dimentions[1];
		    break;
	    case FrSliceExtractor::YZ:		// YZ
		    result = dimentions[0];
		    break;
	    }
    }
	return result;
}