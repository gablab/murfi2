#include "FrMaskEditor.h"
#include "FrDocument.h"
#include "FrDocumentObj.h"
#include "FrRoiDocObj.h"

#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

// Backend includes
#include "RtMaskImage.h"

// Some Defines
#define DEF_ME_VIEW        FrMaskEditor::Unknown
#define DEF_ME_ORIENTATION FrMaskEditor::Undefined
#define DEF_ME_SLICE       0

#define DEF_XDIM    0
#define DEF_YDIM    1
#define DEF_ZDIM    2

vtkStandardNewMacro(FrMaskEditor);


FrMaskEditor::FrMaskEditor()
: m_Data(0), m_DocumentObj(0),
  m_View(DEF_ME_VIEW), 
  m_Orientation(DEF_ME_ORIENTATION), 
  m_SliceNumber(DEF_ME_SLICE) {
}

FrMaskEditor::~FrMaskEditor(){
}

void FrMaskEditor::SetInput(vtkImageData* in){
    m_Data = in;
}

void FrMaskEditor::SetView(Views view){
    switch(view){
        case SliceView: 
            m_View = FrMaskEditor::Slice;
            break;
        case MosaicView: 
            m_View = FrMaskEditor::Mosaic;
            break;
        case OrthoView: 
            m_View = FrMaskEditor::Ortho;
            break;
    }
}

void FrMaskEditor::Update(){
    // Check input data
    if(!m_Data || !m_DocumentObj) return;
    // Check params
    if((m_Orientation == FrMaskEditor::Undefined) || 
       (m_View == FrMaskEditor::Unknown)) return;

    // Performe action
    if(m_View == FrMaskEditor::Slice || 
        m_View == FrMaskEditor::Ortho){

        // Update single slice
        RtMaskImage* img = m_DocumentObj->GetMaskImage();
        if(this->IsDataValid(img, m_Data)){
            switch(m_Orientation){
                case FrMaskEditor::XY:
                    this->UpdateSliceXY(img, m_Data);
                    break;
                case FrMaskEditor::YZ:
                    this->UpdateSliceYZ(img, m_Data);
                    break;
                case FrMaskEditor::XZ:
                    this->UpdateSliceXZ(img, m_Data);
                    break;
            }
        }   
    }
    else if(m_View == FrMaskEditor::Mosaic){
        // Assume data data in our 'RtMaksImage* img' always unmosaic
        RtMaskImage* img = m_DocumentObj->GetMaskImage();
        this->UpdateMosaic(img, m_Data);
    }
}

void FrMaskEditor::UpdateSliceXY(RtMaskImage* img, vtkImageData* data){
    // XY - update whole slice
    int sliceSize = img->getDim(DEF_XDIM) * img->getDim(DEF_YDIM);

    // NOTE: assume img is always unmosaic image
    short* dstPtr = img->data + (sliceSize * m_SliceNumber);    
    unsigned char* srcPtr = (unsigned char*)m_Data->GetScalarPointer();
        
    for(int i=0; i < sliceSize; ++i){ 
        (*dstPtr) = (*srcPtr);
        ++dstPtr; ++srcPtr;
    }
}

void FrMaskEditor::UpdateSliceYZ(RtMaskImage* img, vtkImageData* data){
    // YZ update vertical line for each slice
    short* dstPtr = img->data + m_SliceNumber;
    unsigned char* srcPtr = (unsigned char*)data->GetScalarPointer();
     
    int stride = img->getDim(DEF_XDIM);
    int dataSize = img->getDim(DEF_ZDIM) * img->getDim(DEF_YDIM);
    for(int i=0; i < dataSize; ++i){
        // copy to vertical line for each slice
        (*dstPtr) = (*srcPtr);
        dstPtr += stride;
        ++srcPtr;
    }
}

void FrMaskEditor::UpdateSliceXZ(RtMaskImage* img, vtkImageData* data){
    // ZX update horizontal line for each slice
    int lineSize = img->getDim(DEF_XDIM);

    short* dstPtr = img->data + (m_SliceNumber * lineSize);
    unsigned char* srcPtr = (unsigned char*)m_Data->GetScalarPointer();

    int sliceCount = img->getDim(DEF_ZDIM);
    int lineStride = (img->getDim(DEF_XDIM) * img->getDim(DEF_YDIM)) - lineSize;
    for(int i=0; i < sliceCount; ++i){
        // Copy line data
        for(int j=0; j < lineSize; ++j){
            *dstPtr = *srcPtr;
            ++dstPtr; ++srcPtr;
        }
        dstPtr += lineStride;
    }
}

bool FrMaskEditor::IsDataValid(RtMaskImage* img, vtkImageData* data){
    // Check slice number
    bool goodDims = false;
    bool goodSlice = ( (0 <= m_SliceNumber) && 
        (m_SliceNumber <= img->getDim((int)m_Orientation)) );
    
    // Check dimentions
    if(goodSlice){
        int* dims = data->GetDimensions();
        switch(m_Orientation){
            case FrMaskEditor::XY:
                goodDims = (img->getDim(DEF_XDIM) == dims[DEF_XDIM]) &&
                    (img->getDim(DEF_YDIM) == dims[DEF_YDIM]);
                break;
            case FrMaskEditor::YZ:
                goodDims = ((img->getDim(DEF_YDIM) == dims[DEF_XDIM]) &&
                    (img->getDim(DEF_ZDIM) == dims[DEF_YDIM]));
                break;
            case FrMaskEditor::XZ:
                goodDims = ((img->getDim(DEF_XDIM) == dims[DEF_XDIM]) &&
                    (img->getDim(DEF_ZDIM) == dims[DEF_YDIM]));
                break;
        }
    }
    return (goodSlice && goodDims);
}

void FrMaskEditor::UpdateMosaic(RtMaskImage* img, vtkImageData* data){
    // NOTE: mosaiced source image may have 
    // more or equal pixs than destiny image
    int dataSize = data->GetPointData()->GetScalars()->GetSize();
    int imgSize = img->getNumPix();
    if(dataSize < imgSize) return;

    // Preset some params
    int* dataDims = m_Data->GetDimensions();
    unsigned char* basePtr = (unsigned char*)m_Data->GetScalarPointer();
        
    int sliceWidth = img->getDim(DEF_XDIM);
    int sliceHeight = img->getDim(DEF_YDIM);
    int sliceSize = sliceWidth * sliceHeight;
    int rowSize = dataDims[DEF_XDIM] * sliceHeight;

    // for each slice
    int cols = dataDims[DEF_XDIM] / sliceWidth;
    int stride = dataDims[DEF_XDIM] - sliceWidth;
    for(int i=0; i < img->getDim(DEF_ZDIM); ++i){
        // set pointers
        int row = i / cols;
        int col = i % cols;
        unsigned char* srcPtr = basePtr + (row * rowSize) + (col * sliceWidth);
        short* dstPtr = img->data + (i * sliceSize);
        
        // copy data from source to destiny
        for(int y = 0; y < sliceHeight; ++y){
            for(int x = 0; x < sliceWidth; ++x){
                (*dstPtr) = (*srcPtr);
                ++dstPtr; ++srcPtr;
            }
            srcPtr += stride;
        }
    } // end for slice
}
