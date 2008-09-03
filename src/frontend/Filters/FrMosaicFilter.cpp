#include "FrMosaicFilter.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"

FrMosaicFilter* FrMosaicFilter::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrMosaicFilter");
  
  if(result){
      return (FrMosaicFilter*)result;
  }

  return new FrMosaicFilter();
}

FrMosaicFilter::FrMosaicFilter(){
	m_data = vtkImageData::New();
}

void FrMosaicFilter::SetInput(vtkImageData *data){
	m_data = data;
	m_data->GetDimensions(m_oldDims);
}

void FrMosaicFilter::SetOutputDimensions(int w, int h, int num){
	m_newDims[0] = w;
	m_newDims[1] = h;
	m_newDims[2] = num;
}

vtkImageData* FrMosaicFilter::GetOutput(){
	vtkImageData* m_copy = vtkImageData::New();
	m_copy->SetDimensions(m_newDims);
	m_copy->SetSpacing(m_data->GetSpacing());
	m_copy->SetNumberOfScalarComponents(1);
	m_copy->SetScalarTypeToUnsignedChar();
	m_copy->AllocateScalars();

	unsigned char* dstPtr = (unsigned char*)m_copy->GetPointData()->GetScalars()->GetVoidPointer(0);
	unsigned char* dataPtr = (unsigned char*)m_data->GetPointData()->GetScalars()->GetVoidPointer(0);
	
	for (int slice = 0; slice<m_newDims[2]; slice++)
	for (int x = 0; x<m_newDims[0]; x++)
		for (int y = 0; y<m_newDims[1]; y++){
			int newId = GetNewId(slice, x, y);
			*dstPtr = dataPtr[newId];
			dstPtr++;
		}

	return m_copy;
}

int FrMosaicFilter::GetNewId(int slice, int x, int y){
	int newId = x*m_oldDims[0] + slice*m_newDims[0] + y + (int)(slice/6)*m_newDims[0]*m_oldDims[0];
	return newId;
}

