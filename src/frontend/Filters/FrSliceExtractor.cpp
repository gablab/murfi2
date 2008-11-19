#include "FrSliceExtractor.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"
#include "vtkImageReslice.h"
#include "vtkMatrix4x4.h"

#include <vector>

// Defines
#define DEF_FRAME 0
#define DEF_PORT_NUM 0


vtkStandardNewMacro(FrSliceExtractor);


FrSliceExtractor::FrSliceExtractor(){
    // Init
    m_Inputs.clear();
    m_Outputs.clear();

	m_Slice = DEF_FRAME;    
    m_Orientation = FrSliceExtractor::XY;
}

FrSliceExtractor::~FrSliceExtractor(){
    // Release inputs
    DataPorts::iterator it, itEnd(m_Inputs.end());
    for(it = m_Inputs.begin(); it != itEnd; ++it){
        if(it->second){
            it->second->UnRegister(this);
            it->second = 0L;
        }
    }
    m_Inputs.clear();

    // Release outputs
    this->ClearOutput();
}

void FrSliceExtractor::SetInput(int port, vtkImageData* input){
    // check params
    if(input){
        if(input->GetNumberOfScalarComponents() != 1 ||
            input->GetScalarType() != VTK_UNSIGNED_CHAR){
                vtkErrorMacro( << "FrSliceExtractor: bad input format."
                    << "\tScalarComponents: " << input->GetNumberOfScalarComponents()
                    << "\tScalarType:" << input->GetScalarTypeAsString() );
                return;
        }
    }

    // Performe check and ...
    DataPorts::iterator it = m_Inputs.find(port);
    if(it != m_Inputs.end()){
        // ...update existing port 
        if(it->second != input){
            // Update
            if(it->second) it->second->UnRegister(this);
            if(input) input->Register(this);
            it->second = input;

            this->SetModified(port);
        }
    }
    else {
        // ...or just create new
        if(input) input->Register(this);
        m_Inputs[port] = input;
    }
}

void FrSliceExtractor::SetInput(vtkImageData* input){
    this->SetInput(DEF_PORT_NUM, input);
}

vtkImageData* FrSliceExtractor::GetInput(int port){
    vtkImageData* result = 0L;

    DataPorts::iterator it = m_Inputs.find(port);
    if(it != m_Inputs.end()){
        result = it->second;
    }
    return result;
}

vtkImageData* FrSliceExtractor::GetInput(){
    return this->GetInput(DEF_PORT_NUM);
}

vtkImageData* FrSliceExtractor::GetOutput(int port){
    vtkImageData* result = 0L;

    DataPorts::iterator it = m_Outputs.find(port);
    if(it != m_Outputs.end()){
        result = it->second;
    }
    return result;
}

vtkImageData* FrSliceExtractor::GetOutput(){
    return this->GetOutput(DEF_PORT_NUM);
}

void FrSliceExtractor::SetOrientation(FrSliceExtractor::Orientation orient){
    if(m_Orientation != orient){
        m_Orientation = orient;
        this->ClearOutput();
    }
}

void FrSliceExtractor::SetSlice(int slice){
    if(m_Slice != slice){
        m_Slice = slice;
        this->ClearOutput();
    }
}

void FrSliceExtractor::Update(){
    DataPorts::iterator it, itEnd(m_Inputs.end());
    for(it = m_Inputs.begin(); it != itEnd; ++it){
        // Update only modified inputs
        if(this->IsModified(it->first)){
            // create data object
            vtkImageData* output = vtkImageData::New();
            this->SetImageInfo(it->second, output);
            output->AllocateScalars();

            bool result = false;
            switch (m_Orientation)
            {
            case FrSliceExtractor::XY:
                result = this->ExtractXY(it->second, output);
                break;
            case FrSliceExtractor::XZ:
                result = this->ExtractXZ(it->second, output);
                break;
            case FrSliceExtractor::YZ:
                result = this->ExtractYZ(it->second, output);
                break;
            }

            if(result){
                this->SetOutput(it->first, output);
            }
            output->Delete();
        }
    }
}

int FrSliceExtractor::GetMaxSliceNumber(){
    int result = -1;

    // Calc using default input (if any)
    vtkImageData* img = this->GetInput();
    if(img){
        int dimentions[3];
        img->GetDimensions( dimentions);	

        switch (m_Orientation)
        {
        case FrSliceExtractor::XY:
                result = dimentions[2]-1; 
                break;
        case FrSliceExtractor::XZ:
                result = dimentions[1]-1;
                break;
        case FrSliceExtractor::YZ:
                result = dimentions[0]-1;
                break;
        }
    }
	return result;
}

void FrSliceExtractor::RemoveAdditionalPorts(){
    std::vector<int> portsToDelete;

    // Get all port# to be deleted
    DataPorts::iterator it, itEnd(m_Inputs.end());
    for(it = m_Inputs.begin(); it != itEnd; ++it){
        if(it->first != DEF_PORT_NUM) {
            portsToDelete.push_back(it->first);
            it->second->UnRegister(this);
        }
    }

    std::vector<int>::iterator itr, itrEnd(portsToDelete.end());
    for(itr = portsToDelete.begin(); itr != itrEnd; ++itr){
        m_Inputs.erase( (*itr) );
        this->SetModified( (*itr) );
    }
}

void FrSliceExtractor::ClearOutput(){
    // unregister all output image data
    DataPorts::iterator it, itEnd(m_Outputs.end());
    for(it = m_Outputs.begin(); it != itEnd; ++it){
        if(it->second){
            it->second->UnRegister(this);
        }
    }
    m_Outputs.clear();
}

void FrSliceExtractor::SetOutput(int port, vtkImageData* data){
    DataPorts::iterator it = m_Outputs.find(port);
    if (it != m_Outputs.end()){
        if(data) data->Register(this);
        if(it->second) it->second->UnRegister(this);
        it->second = data;
    }
    else {
        if(data) data->Register(this);
        m_Outputs[port] = data;
    }
}

bool FrSliceExtractor::IsModified(int port){
    // Port is modified if there is no output value for it
    DataPorts::iterator it = m_Outputs.find(port);
    return (it == m_Outputs.end());
}

void FrSliceExtractor::SetModified(int port){
    // Port is modified if there is no output value for it
    DataPorts::iterator it = m_Outputs.find(port);
    if(it != m_Outputs.end()){
        if(it->second){
            it->second->UnRegister(this);
        }
        m_Outputs.erase(it);
    }
}

void FrSliceExtractor::SetImageInfo(vtkImageData* in, vtkImageData* out){
    if(!in || !out) return;

    int* dims = in->GetDimensions();
    double* spacing = in->GetSpacing();

    switch (m_Orientation)
    {
    case FrSliceExtractor::XY:
        out->SetDimensions(dims[0], dims[1], 1);
        out->SetSpacing(spacing[0], spacing[1], 1.0);
        break;
    case FrSliceExtractor::XZ:
        out->SetDimensions(dims[0], dims[2], 1);
        out->SetSpacing(spacing[0], spacing[2], 1.0);
        break;
    case FrSliceExtractor::YZ:
        out->SetDimensions(dims[1], dims[2], 1);
        out->SetSpacing(spacing[1], spacing[2], 1.0);
        break;
    }

    out->SetOrigin(0.0, 0.0, 0.0);
    out->SetNumberOfScalarComponents(1);
    out->SetScalarTypeToUnsignedChar();
}

bool FrSliceExtractor::ExtractYZ(vtkImageData* in, vtkImageData* out){
    if(!in || !out) return false;

    bool result = false;
    int* dataDims = in->GetDimensions();
    if(0 <= m_Slice && m_Slice < dataDims[0]){
        // Init data
        unsigned char* srcPtr = ((unsigned char*)in->GetScalarPointer()) + m_Slice;
        unsigned char* dstPtr = (unsigned char*)out->GetScalarPointer();

        int stride = dataDims[0];
        int dataSize = dataDims[2] * dataDims[1];
        for(int i=0; i < dataSize; ++i){
            // copy vertical line from each slice
            (*dstPtr) = (*srcPtr);
            srcPtr += stride;
            ++dstPtr;
        }
        result = true;
    }
    return result;
}

bool FrSliceExtractor::ExtractXZ(vtkImageData* in, vtkImageData* out){
    if(!in || !out) return false;

    bool result = false;
    int* dataDims = in->GetDimensions();
    if(0 <= m_Slice && m_Slice < dataDims[1]){
        // Init data
        int lineSize = dataDims[0];
        unsigned char* srcPtr = ((unsigned char*)in->GetScalarPointer()) + (m_Slice * lineSize);
        unsigned char* dstPtr = (unsigned char*)out->GetScalarPointer();

        int lineCnt = dataDims[2];
        int lineStride = dataDims[0] * dataDims[1] - lineSize;
        for(int i=0; i < lineCnt; ++i){
            // copy horizontal line from each slice
            for(int j=0; j < lineSize; ++j){
                (*dstPtr) = (*srcPtr);
                ++dstPtr; ++srcPtr;
            }
            srcPtr += lineStride;
        }
        result = true;
    }
    return result;
}

bool FrSliceExtractor::ExtractXY(vtkImageData* in, vtkImageData* out){
    if(!in || !out) return false;

    bool result = false;
    int* dataDims = in->GetDimensions();
    if(0 <= m_Slice && m_Slice < dataDims[2]){
        // Init data
        int dataSize = dataDims[0] * dataDims[1];
        unsigned char* srcPtr = ((unsigned char*)in->GetScalarPointer()) + (m_Slice * dataSize);
        unsigned char* dstPtr = (unsigned char*)out->GetScalarPointer();

        memcpy(dstPtr, srcPtr, dataSize);
        result = true;
    }

    return result;
}
