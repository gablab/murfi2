#include "FrMosaicFilter.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(FrMosaicFilter);

FrMosaicFilter::FrMosaicFilter(){
}

void FrMosaicFilter::SetInput(vtkImageData *data){
    FrBaseFilter::SetInput(data);
	this->GetInput()->GetDimensions(m_oldDims);
}

void FrMosaicFilter::SetOutputDimensions(int w, int h, int num){
	m_newDims[0] = w;
	m_newDims[1] = h;
	m_newDims[2] = num;
}

// Use this to update output params
void FrMosaicFilter::ExecuteInformation() {
    FrBaseFilter::ExecuteInformation();

    vtkImageData *input=this->GetInput();
    vtkImageData *output=this->GetOutput();

    if (input==NULL) return;
    
    // Setup extent
    output->SetDimensions(m_newDims);
    output->SetWholeExtent( output->GetExtent() );
    output->SetUpdateExtent( output->GetExtent() );
        
    // Setup dimentions and other params
    output->SetSpacing( input->GetSpacing() );
    output->SetNumberOfScalarComponents(1);
    output->SetScalarTypeToUnsignedChar();
}

void FrMosaicFilter::SimpleExecute(vtkImageData* input, vtkImageData* output){
        
    unsigned char* srcPtr = (unsigned char*)input->GetPointData()->GetScalars()->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->GetScalars()->GetVoidPointer(0);
		
	for (int slice = 0; slice < m_newDims[2]; slice++)
	for (int x = 0; x < m_newDims[0]; x++)
		for (int y = 0; y < m_newDims[1]; y++){
			int newId = GetNewId(slice, x, y);
			*dstPtr = srcPtr[newId];
			dstPtr++;
		}
}

int FrMosaicFilter::GetNewId(int slice, int x, int y){
	int newId = x*m_oldDims[0] + slice*m_newDims[0] + y +
          (int)(slice/6)*m_newDims[0]*m_oldDims[0];
	return newId;
}

