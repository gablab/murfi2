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
                case FrMaskEditor::ZY:
                    this->UpdateSliceXY(img, m_Data);
                    break;
                case FrMaskEditor::XZ:
                    this->UpdateSliceXY(img, m_Data);
                    break;
            }
        }   
    }
    else if(m_View == FrMaskEditor::Mosaic){
        // Assume data data in our 'RtMaksImage* img' always unmosaic
        RtMaskImage* img = m_DocumentObj->GetMaskImage();

        // Little check
        // NOTE mosaiced image may have more pix then 
        int dataSize = m_Data->GetPointData()->GetScalars()->GetSize();
        int imgSize = img->getNumPix();        
        if(dataSize >= imgSize){
            // init some params
            int* dataDims = m_Data->GetDimensions();            
            int sliceWidth = img->getDim(DEF_XDIM);
            int sliceHeight = img->getDim(DEF_YDIM);
            int sliceSize = sliceWidth * sliceHeight;
            int numOfSlices = img->getDim(DEF_ZDIM);
            
            short* dstPtr = 0;
            unsigned char* srcPtr = (unsigned char*)m_Data->GetScalarPointer();
            for(int i=0; i < dataSize; ++i){
                // calc slice
                int slice = (i % dataDims[DEF_XDIM]) / sliceWidth + // slice column
                    ( i / (dataDims[DEF_XDIM] * sliceWidth) ) *  // slice row
                    (dataDims[DEF_XDIM] / sliceWidth);   // num of slices in row

                if(slice > numOfSlices) {
                    continue;
                }

                // calc row & column
                int row = (i / dataDims[DEF_XDIM]) % sliceHeight;
                int col = (i / dataDims[DEF_XDIM]) % sliceWidth;

                dstPtr = img->data + (slice * sliceSize) + (row * sliceWidth) + col;
                *dstPtr = *srcPtr;
                ++srcPtr;
            }
        }
    }
}

void FrMaskEditor::UpdateSliceXY(RtMaskImage* img, vtkImageData* data){
    // XY - update whole slice
    int sliceSize = img->getDim(DEF_XDIM) * img->getDim(DEF_YDIM);

    // NOTE: assume img is always unmosaic image
    short* dstPtr = img->data + (sliceSize * m_SliceNumber);    
    unsigned char* srcPtr = (unsigned char*)m_Data->GetScalarPointer();
        
    for(int i=0; i < sliceSize; ++i){
        *dstPtr = *srcPtr;
        ++dstPtr; ++srcPtr;
    }
}

void FrMaskEditor::UpdateSliceZY(RtMaskImage* img, vtkImageData* data){
    // ZY update vertical line for each slice
    int srcPixInRow = data->GetDimensions()[DEF_XDIM];
    int dstPixInRow = img->getDim(DEF_XDIM);
    int pixInColumn = img->getDim(DEF_YDIM);
    int sliceSize = img->getDim(DEF_XDIM) * img->getDim(DEF_YDIM);

    int sliceCount = img->getDim(DEF_ZDIM);
    for(int i=0; i < sliceCount; ++i){
        // get data pointers
        short* dstPtr = img->data + (i * sliceSize) + m_SliceNumber;
        unsigned char* srcPtr = ((unsigned char*)m_Data->GetScalarPointer()) + i;

        for(int j=0; j < pixInColumn; ++j){
            *dstPtr = *srcPtr;
            dstPtr += dstPixInRow;
            srcPtr += srcPixInRow;
        }
    }
}

void FrMaskEditor::UpdateSliceZX(RtMaskImage* img, vtkImageData* data){
    // ZX update horizontal line for each slice
    int srcPixInRow = data->GetDimensions()[DEF_XDIM];
    int pixInRow = img->getDim(DEF_XDIM);
    int sliceSize = img->getDim(DEF_XDIM) * img->getDim(DEF_YDIM);

    int sliceCount = img->getDim(DEF_ZDIM);
    for(int i=0; i < sliceCount; ++i){
        // get data pointers
        int offset1 = m_SliceNumber * pixInRow;
        short* dstPtr = img->data + (i * sliceSize) + offset1;

        int offset2 = i * pixInRow;
        unsigned char* srcPtr = ((unsigned char*)m_Data->GetScalarPointer()) + offset2;

        for(int j=0; j < pixInRow; ++j){
            *dstPtr = *srcPtr;
            ++dstPtr; ++srcPtr;
        }
    }
}

bool FrMaskEditor::IsDataValid(RtMaskImage* img, vtkImageData* data){
    // Check slice number
    bool goodSlice = ( (0 <= m_SliceNumber) && 
        (m_SliceNumber < img->getDim((int)m_Orientation)) );
    
    // Check dimentions
    bool goodDims = false;
    int* dims = data->GetDimensions();
    switch(m_Orientation){
        case FrMaskEditor::XY:
            goodDims = (img->getDim(DEF_XDIM) == dims[DEF_XDIM]) &&
                (img->getDim(DEF_YDIM) == dims[DEF_YDIM]);
            break;
        case FrMaskEditor::ZY:
            goodDims = ((img->getDim(DEF_ZDIM) == dims[DEF_XDIM]) &&
                (img->getDim(DEF_YDIM) == dims[DEF_YDIM]));
            break;
        case FrMaskEditor::XZ:
            goodDims = ((img->getDim(DEF_XDIM) == dims[DEF_XDIM]) &&
                (img->getDim(DEF_ZDIM) == dims[DEF_YDIM]));
            break;
    }

    return (goodSlice && goodDims);
}