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
    // TODO: not finished, we can implement different methods for different types of colormap
	
	switch(m_CMType){
		case 1:
			ProcessMultiColormap(input, output);
			break;
		case 2:
			ProcessSingleColormap(input, output);
			break;
	}

}

int FrColormapFilter::GetId(int x, int y){
	return x*m_Dims[1]+y;	
}
void FrColormapFilter::ProcessSingleColormap(vtkImageData* input, vtkImageData* output){
    unsigned char* srcPtr = (unsigned char*)input->GetPointData()->GetScalars()->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->GetScalars()->GetVoidPointer(0);

	int r1, g1, b1;
	m_Color.getRgb(&r1, &g1, &b1);
	// HSV
	double h, s, v; 
	vtkMath::RGBToHSV(r1, b1, g1, &h, &s, &v);

	for (int x = 0; x < m_Dims[0]; x++){
		for (int y = 0; y < m_Dims[1]; y++){
			int id = GetId(x, y);
			
			// new values for RGB
			double r, g, b;
			
			// old grayscale value
			unsigned char k = srcPtr[id];

			vtkMath::HSVToRGB(h, s, k, &r, &g, &b);

			*dstPtr = r;	dstPtr++;
			*dstPtr = g;	dstPtr++;
			*dstPtr = b;	dstPtr++;
		}
	}
}

void FrColormapFilter::ProcessMultiColormap(vtkImageData* input, vtkImageData* output){
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

			if (k>=m_PxMin && k<m_Threshold){		// (min, threshold) range
				// map to 240..180 range
				min = 180;
				max = 240;
			}
			else if (k>=m_Threshold && k<=m_PxMax){		// (threshold, max) range
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
