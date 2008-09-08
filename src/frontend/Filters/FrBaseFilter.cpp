#include "FrBaseFilter.h"
#include "vtkImageData.h"

#include "vtkInformation.h"
#include "vtkStreamingDemandDrivenPipeline.h"


FrBaseFilter::FrBaseFilter(){
    this->NumberOfRequiredInputs = 1;
    this->SetNumberOfInputPorts(1);
}

FrBaseFilter::~FrBaseFilter(){
}

void FrBaseFilter::SetInput(vtkImageData *input){
    this->vtkProcessObject::SetNthInput(0, input);
}

vtkImageData* FrBaseFilter::GetInput(){
    if (this->NumberOfInputs < 1) {
        return NULL;
    }
    return (vtkImageData *)(this->Inputs[0]);
}

void FrBaseFilter::ExecuteInformation() {
    vtkImageData *input = this->GetInput();
    vtkImageData *output = this->GetOutput();
    
    // Make sure the Input has been set.
    if ( input == NULL){
      vtkErrorMacro(<< "ExecuteInformation: Input is not set.");
      return;
    }

    // Start with some defaults.
    output->CopyTypeSpecificInformation( input );
}


void FrBaseFilter::ComputeInputUpdateExtent(int inExt[6], 
                                            int vtkNotUsed(outExt)[6] ){
    vtkImageData *input = this->GetInput();

    // Make sure the Input has been set.
    if ( input == NULL) {
        vtkErrorMacro(<< "ComputeInputUpdateExtent: Input is not set.");
        return;
    }

    // Use whole extent in base class
    int* wholeExtent = input->GetWholeExtent();
    memcpy(inExt,wholeExtent,sizeof(int)*6);
}

void FrBaseFilter::ExecuteData(vtkDataObject *vtkNotUsed(out)){

    vtkDebugMacro("Executing.");
    vtkImageData* output = this->GetOutput();
    vtkImageData* input = this->GetInput();

    if (!input) {
        vtkErrorMacro("No input is specified!");
        return;
    }

    // Too many filters have floating point exceptions to execute
    // with empty input/ no request.
    if (this->UpdateExtentIsEmpty(output)) return;

    // Set the extent of the output and allocate memory.
    output->SetExtent(output->GetUpdateExtent());
    output->AllocateScalars();

    this->SimpleExecute(input, output);
}

int FrBaseFilter::FillOutputPortInformation(int port, vtkInformation* info){
    if(!this->Superclass::FillInputPortInformation(port, info)) {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
    return 1;
}
