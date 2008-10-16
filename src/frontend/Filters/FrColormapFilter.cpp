#include "FrColormapFilter.h"

// VTK stuff
#include "vtkMath.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkObjectFactory.h"

#define DEF_MIN_VALUE 0
#define DEF_MAX_VALUE 255
#define DEF_THRESHOLD 128

#define HUE_RED    0
#define HUE_YELLOW 60
#define HUE_LBLUE  180
#define HUE_BLUE   240
#define HUE_NORM   360
 

FrColormapFilter* FrColormapFilter::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrColormapFilter");
  
  if(result){
      return (FrColormapFilter*)result;
  }

  return new FrColormapFilter();
}

FrColormapFilter::FrColormapFilter()
: m_Type(FrColormapFilter::SingleColor),
  m_PxMin(DEF_MIN_VALUE), m_PxMax(DEF_MAX_VALUE),
  m_Threshold(DEF_THRESHOLD),
  m_IsModified(true){
    // use blue color as default
    m_Color = QColor(0, 0, 255, 255); 
}

// Modifiers
void FrColormapFilter::SetInput(vtkImageData *data){
    if(data != FrBaseFilter::GetInput()){
        FrBaseFilter::SetInput(data);
	    this->GetInput()->GetDimensions(m_Dims);
        this->Modified();
        m_IsModified = true;
    }
}

void FrColormapFilter::SetThreshold(int value){
    if(m_Threshold != value){
        m_Threshold = value;
        // Property valide for 
        if(m_Type == FrColormapFilter::MultiColor){
            this->Modified();
            m_IsModified = true;
        }
    }
}

void FrColormapFilter::SetPxMin(int value){
    if(m_PxMin != value){
        m_PxMin = value;
        this->Modified();
        m_IsModified = true;
    }
}

void FrColormapFilter::SetPxMax(int value){
    if(m_PxMax != value){
        m_PxMax = value;
        this->Modified();
        m_IsModified = true;
    }
}

void FrColormapFilter::SetType(int value){
    if(m_Type != value){
        m_Type = value;
        this->Modified();
        m_IsModified = true;
    }
}

void FrColormapFilter::SetColor(QColor value){
    if(m_Color != value){
        m_Color = value;

        // Property valide for single color maps
        if(m_Type == FrColormapFilter::SingleColor){
            this->Modified();
            m_IsModified = true;
        }
    }
}

// Use this to update output params
void FrColormapFilter::ExecuteInformation() {
    FrBaseFilter::ExecuteInformation();

    vtkImageData *input=this->GetInput();
    vtkImageData *output=this->GetOutput();

    if (input==NULL) return;
    
	output->SetScalarTypeToUnsignedChar();
	output->SetNumberOfScalarComponents(4);				// ARGB
	output->SetDimensions(input->GetDimensions());
	output->SetSpacing(input->GetSpacing());
}

void FrColormapFilter::SimpleExecute(vtkImageData* input, vtkImageData* output){
    // TODO: OPTIMIZE THIS!!!
	switch(m_Type){
        case FrColormapFilter::MultiColor:
			ProcessMultiColormap(input, output);
			break;
        case FrColormapFilter::SingleColor:
			ProcessSingleColormap(input, output);
			break;
	}
    m_IsModified = false;
}

int FrColormapFilter::GetId(int x, int y){
	return x*m_Dims[1]+y;	
}

void FrColormapFilter::ProcessSingleColormap(vtkImageData* input, vtkImageData* output){
    unsigned char* srcPtr = (unsigned char*)input->GetPointData()->GetScalars()->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->GetScalars()->GetVoidPointer(0);

    // For now just mark with single color
	int r1, g1, b1;
	m_Color.getRgb(&r1, &g1, &b1);

	//// HSV
	//double h, s, v; 
	//vtkMath::RGBToHSV(r1, b1, g1, &h, &s, &v);
	for (int x = 0; x < m_Dims[0]; x++){
		for (int y = 0; y < m_Dims[1]; y++){
			int id = GetId(x, y);
			
			// old grayscale value
			unsigned char k = *srcPtr; //[id];
            if(m_PxMin <= k && k <= m_PxMax){
                *dstPtr = r1;  ++dstPtr;
                *dstPtr = g1;  ++dstPtr;
                *dstPtr = b1;  ++dstPtr;
                *dstPtr = DEF_MAX_VALUE; 
                ++dstPtr;
            }
            else {
                // otherwise clear color
                *dstPtr = DEF_MIN_VALUE;  ++dstPtr;
                *dstPtr = DEF_MIN_VALUE;  ++dstPtr;
                *dstPtr = DEF_MIN_VALUE;  ++dstPtr;
                *dstPtr = DEF_MIN_VALUE; ++dstPtr;
            }
            ++srcPtr;
		}
	}
}

void FrColormapFilter::ProcessMultiColormap(vtkImageData* input, vtkImageData* output){
    unsigned char* srcPtr = (unsigned char*)input->GetPointData()->GetScalars()->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->GetScalars()->GetVoidPointer(0);
		
	for (int x = 0; x < m_Dims[0]; x++){
		for (int y = 0; y < m_Dims[1]; y++){
			int id = GetId(x, y);
						
			// old grayscale value
			unsigned char value = srcPtr[id];
            if(m_PxMin <= value && value <= m_PxMax){
                double min  = (value < m_Threshold) ? HUE_LBLUE : HUE_RED;
                double max  = (value < m_Threshold) ? HUE_BLUE  : HUE_YELLOW;
                double norm = (value < m_Threshold) ? 
                    (m_Threshold - m_PxMin) : (m_Threshold - m_PxMax);
                 
                double mult = abs((value - m_Threshold) / norm);
                double delta = (max - min) * mult;
                double hue = (value < m_Threshold) ? 
                    (min + delta) / HUE_NORM : 
                    (max - delta) / HUE_NORM;
                                                
                // calculate final color
                double r, g, b;
		        vtkMath::HSVToRGB(hue, 1.0, 255.0, &r, &g, &b);	

		        *dstPtr = r;   ++dstPtr;
		        *dstPtr = g;   ++dstPtr;
		        *dstPtr = b;   ++dstPtr;
                *dstPtr = DEF_MAX_VALUE; 
                ++dstPtr;
            }
            else {
                // otherwise clear color
                *dstPtr = DEF_MIN_VALUE; ++dstPtr;
                *dstPtr = DEF_MIN_VALUE; ++dstPtr;
                *dstPtr = DEF_MIN_VALUE; ++dstPtr;
                *dstPtr = DEF_MIN_VALUE; ++dstPtr;
            }
		}
	}
}
