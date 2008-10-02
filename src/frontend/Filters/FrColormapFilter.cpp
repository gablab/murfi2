#include "FrColormapFilter.h"

// VTK stuff
#include "vtkMath.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkObjectFactory.h"


FrColormapFilter* FrColormapFilter::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrColormapFilter");
  
  if(result){
      return (FrColormapFilter*)result;
  }

  return new FrColormapFilter();
}

FrColormapFilter::FrColormapFilter(){
}

void FrColormapFilter::SetInput(vtkImageData *data){
    FrBaseFilter::SetInput(data);
	this->GetInput()->GetDimensions(m_Dims);
}

// Use this to update output params
void FrColormapFilter::ExecuteInformation() {
    FrBaseFilter::ExecuteInformation();

    vtkImageData *input=this->GetInput();
    vtkImageData *output=this->GetOutput();

    if (input==NULL) return;
    
	output->SetScalarTypeToUnsignedChar();
	output->SetNumberOfScalarComponents(3);				// RGB
	output->SetDimensions(input->GetDimensions());
	output->SetSpacing(input->GetSpacing());
}

void FrColormapFilter::SimpleExecute(vtkImageData* input, vtkImageData* output){
    // TODO: not finished

    unsigned char* srcPtr = (unsigned char*)input->GetPointData()->GetScalars()->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->GetScalars()->GetVoidPointer(0);
		
	for (int x = 0; x < m_Dims[0]; x++){
		for (int y = 0; y < m_Dims[1]; y++){
			int id = GetId(x, y);
			
			// new values for RGB
			double r, g, b;
			
			// old grayscale value
			unsigned char k = srcPtr[id];
			
			// calculate new Hue value
			double hue;
			int min, max;

			if (k>=0 && k<128){		// (min, threshold) range
				// map to 240..180 range
				min = 180;
				max = 240;
			}
			else if (k>=128 && k<=255){		// (threshold, max) range
				// map to 60..0 range
				min = 0;
				max = 60;
			}

			float mult = float(max - min) / 255.0f;
			hue = k*mult + min;
			double m = 1/360.0;
			
			vtkMath::HSVToRGB(hue*m, 1.0, 255.0, &r, &g, &b);	// saturation, value is always the same

			*dstPtr = r;	dstPtr++;
			*dstPtr = g;	dstPtr++;
			*dstPtr = b;	dstPtr++;
		}
	}

}

int FrColormapFilter::GetId(int x, int y){
	return x*m_Dims[1]+y;	
}
