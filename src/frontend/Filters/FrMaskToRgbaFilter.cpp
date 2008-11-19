#include "FrMaskToRgbaFilter.h"
#include "FrUtils.h"

// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

// Some defines
#define R_INDEX 0
#define G_INDEX 1
#define B_INDEX 2
#define A_INDEX 3
#define COMPONENT_NUM 4

#define DEF_MIN_VALUE 0
#define DEF_MAX_VALUE 255


vtkStandardNewMacro(FrMaskToRgbaFilter);


FrMaskToRgbaFilter::FrMaskToRgbaFilter(){
}

void FrMaskToRgbaFilter::ExecuteInformation() {
    FrBaseFilter::ExecuteInformation();
    if(!this->GetInput()) return;

    vtkImageData *input=this->GetInput();
    vtkImageData *output=this->GetOutput();

    output->SetScalarTypeToUnsignedChar();
    output->SetNumberOfScalarComponents(4); // ARGB
    output->SetDimensions(input->GetDimensions());
    output->SetSpacing(input->GetSpacing());
}

// The execute method created by the subclass.
void FrMaskToRgbaFilter::SimpleExecute(vtkImageData *inData, 
                                       vtkImageData *outData){
    // Check for valid data type
    if(!inData) return;
    vtkDataArray* inArray = inData->GetPointData()->GetScalars();
    vtkDataArray* outArray = outData->GetPointData()->GetScalars();

    // Apply transformation

    // NOTE: assume that input is a single component grayscale data
    unsigned char* srcPtr = (unsigned char*)inArray->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)outArray->GetVoidPointer(0);
    for(int i=0; i < inArray->GetSize(); ++i){
        dstPtr[R_INDEX] = dstPtr[G_INDEX] = dstPtr[B_INDEX] = DEF_MIN_VALUE;
        dstPtr[A_INDEX] = ( (*srcPtr) > 0 ) ? DEF_MIN_VALUE : DEF_MAX_VALUE;
        dstPtr += COMPONENT_NUM;
        ++srcPtr;
    }
}
