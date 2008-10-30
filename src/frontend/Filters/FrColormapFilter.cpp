#include "FrColormapFilter.h"


// VTK stuff
#include "vtkMath.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkObjectFactory.h"

#define R_INDEX 0
#define G_INDEX 1
#define B_INDEX 2
#define A_INDEX 3
#define COMPONENT_NUM 4

#define H_INDEX 0
#define S_INDEX 1
#define V_INDEX 2

#define DEF_MIN_VALUE 0
#define DEF_MAX_VALUE 255
#define DEF_MID_VALUE 50
#define DEF_THRESHOLD 4

#define HUE_RED    0
#define HUE_YELLOW 60
#define HUE_LBLUE  180
#define HUE_BLUE   240
#define HUE_NORM   360
 
vtkStandardNewMacro(FrColormapFilter);


FrColormapFilter::FrColormapFilter()
: m_Type(FrColormapFilter::SingleColor),
  m_PxMin(DEF_MIN_VALUE), m_PxMax(DEF_MAX_VALUE),
  m_PxMid(DEF_MID_VALUE),
  m_Threshold(DEF_THRESHOLD),
  m_IsModified(true){
    // use blue color as default
    m_Color = QColor(0, 0, 255, 255); 
}

// Modifiers
void FrColormapFilter::SetInput(vtkImageData *data){
    if(data != FrBaseFilter::GetInput()){
        FrBaseFilter::SetInput(data);

        if(this->GetInput()){
	        this->GetInput()->GetDimensions(m_Dims);
        }
        else {
            m_Dims[0] = m_Dims[1] = m_Dims[2] = 0;
        }
        this->Modified();
        m_IsModified = true;
    }
}

void FrColormapFilter::SetPxMid(int value){
    if(m_PxMid != value){
        m_PxMid = value;
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

void FrColormapFilter::SetThreshold(int value){
    if(m_Threshold != value){
        m_Threshold = value;
        this->Modified();
        m_IsModified = true;
    }
}

// Use this to update output params
void FrColormapFilter::ExecuteInformation() {
    FrBaseFilter::ExecuteInformation();
    if(!this->GetInput()) return;

    vtkImageData *input=this->GetInput();
    vtkImageData *output=this->GetOutput();
    
	output->SetScalarTypeToUnsignedChar();
	output->SetNumberOfScalarComponents(4);				// ARGB
	output->SetDimensions(input->GetDimensions());
	output->SetSpacing(input->GetSpacing());
}

void FrColormapFilter::SimpleExecute(vtkImageData* input, vtkImageData* output){
    if(!input) return;
    vtkDataArray* inArray = input->GetPointData()->GetScalars();
    vtkDataArray* outArray = output->GetPointData()->GetScalars();

    unsigned char luTable[256][3];
    bool isValideType = true;
    
	switch(m_Type){
        case FrColormapFilter::SingleColor:
			InitSingleLookupTable(luTable);
			break;
        case FrColormapFilter::MultiColor:
			InitMultiLookupTable(luTable);
			break;
        default:
            vtkErrorMacro(<<"FrColormapFilter: Filter type is undefined...");
            isValideType = false;
            break;
	}

    // Apply transformation
    if(isValideType){
        // NOTE: assume that input is a single component grayscale data
        unsigned char value;
        unsigned char* srcPtr = (unsigned char*)inArray->GetVoidPointer(0);
        unsigned char* dstPtr = (unsigned char*)outArray->GetVoidPointer(0);
        for(int i=0; i < inArray->GetSize(); ++i){
            value = (*srcPtr);
            if(m_PxMin <= value && value <= m_PxMax){
                dstPtr[R_INDEX] = luTable[value][R_INDEX];
                dstPtr[G_INDEX] = luTable[value][G_INDEX];
                dstPtr[B_INDEX] = luTable[value][B_INDEX];
                dstPtr[A_INDEX] = DEF_MAX_VALUE;
                dstPtr += COMPONENT_NUM;
            }
            else {
                // otherwise clear color
                dstPtr[R_INDEX] = DEF_MIN_VALUE;
                dstPtr[G_INDEX] = DEF_MIN_VALUE;
                dstPtr[B_INDEX] = DEF_MIN_VALUE;
                dstPtr[A_INDEX] = DEF_MIN_VALUE;
                dstPtr += COMPONENT_NUM;
            }
            ++srcPtr;
        }
    }
    m_IsModified = false;
}

void FrColormapFilter::InitSingleLookupTable(unsigned char luTable[][3]){
    double rgb[3];
    rgb[R_INDEX] = m_Color.red();
    rgb[G_INDEX] = m_Color.green();
    rgb[B_INDEX] = m_Color.blue();

    double hsv[3];
    vtkMath::RGBToHSV(rgb, hsv);

    for(int i=0; i < 256; ++i){
        if(m_PxMin <= i && i <= m_PxMax){
            hsv[V_INDEX] = double(i);
            vtkMath::HSVToRGB(hsv, rgb);

            luTable[i][R_INDEX] = rgb[R_INDEX];
            luTable[i][G_INDEX] = rgb[G_INDEX];
            luTable[i][B_INDEX] = rgb[B_INDEX];
        }
    }
}

void FrColormapFilter::InitMultiLookupTable(unsigned char luTable[][3]){
    // params for initialization
    double min, max;
    double iPos, delta, hue;
    double rgb[3];
    double hsv[3];
    
    // set defaults
    hsv[S_INDEX] = 1.0;
    hsv[V_INDEX] = 255.0;

    // Calc threshold
    int threshold = m_PxMin + ((m_PxMax - m_PxMin) * m_PxMid) / 100;
    for(int i=0; i < 256; ++i){
        // NOTE: range consists from two parts: left and right
        // [m_PxMin, m_Threshold] && [m_Threshold, m_PxMax]
        if(m_PxMin <= i && i <= m_PxMax){
            // get relative pos of i from threshold
            if(i < threshold){
                // ...in left part
                min = HUE_LBLUE; max = HUE_BLUE;
                iPos = double(i - threshold) / double(m_PxMin - threshold);
            }
            else {
                // ...in right part
                min = HUE_YELLOW; max = HUE_RED;
                iPos = double(i - threshold) / double(m_PxMax - threshold);
            }
                
            // calc delta for hue 
            // (NOTE: in left part delta is positive in right is negative)
            delta = (max - min) * iPos;
            hsv[H_INDEX] = (min + delta) / HUE_NORM;

            // calculate final color
	        vtkMath::HSVToRGB(hsv, rgb);	

	        luTable[i][R_INDEX] = (unsigned char)rgb[R_INDEX];
            luTable[i][G_INDEX] = (unsigned char)rgb[G_INDEX];
            luTable[i][B_INDEX] = (unsigned char)rgb[B_INDEX];
        }
    }

	// special area with non significant pixels around threshold
    int iMin = threshold - (m_Threshold / 2);
    int iMax = iMin + m_Threshold;
	if (iMin<0)
		iMin = 0;
	if (iMax>256)
		iMax = 256;
    for(int i = iMin; i < iMax; ++i){
		luTable[i][R_INDEX] = 
        luTable[i][G_INDEX] = 
        luTable[i][B_INDEX] = 0;
	}
}