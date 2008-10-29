#include "FrRoiDocObj.h"
#include "FrDocument.h"

// Qt stuff
#include <Qt/QString.h>
#include <Qt/QFile.h>

// VTK stuff
#include "vtkImageData.h"
#include "vtkPointData.h"


FrRoiDocObj::FrRoiDocObj()
:  m_MaskImage(0) {
    
}

FrRoiDocObj::~FrRoiDocObj(){
    if(m_MaskImage) m_MaskImage->Delete();
}

void FrRoiDocObj::OnAdd(FrDocument* doc){
    // TODO : Implement update
}

void FrRoiDocObj::OnRemove(FrDocument* doc){
    // TODO : Implement update
}

FrDocumentObj::ObjType FrRoiDocObj::GetType(){
    return FrDocumentObj::RoiObject;
}

bool FrRoiDocObj::LoadFromFile(QString& fileName){
    bool result = false;
    if(QFile::exists(fileName)){
        // TODO: implement
        // need to get image params
        result = true;
    }
    return result;
}

void FrRoiDocObj::CreateMaskImage(int x, int y, int z, 
                                  int sx, int sy, int sz){
    if(m_MaskImage) m_MaskImage->Delete();
    
    m_MaskImage = vtkImageData::New();
    m_MaskImage->SetDimensions(x, y, z);
    m_MaskImage->SetSpacing(sx, sy, sz);
    m_MaskImage->SetScalarTypeToUnsignedChar();
    m_MaskImage->SetNumberOfScalarComponents(1);
    m_MaskImage->AllocateScalars();
    
    // All values have to be false (zero)
    int size = m_MaskImage->GetPointData()->GetScalars()->GetSize();
    unsigned char* ptr = (unsigned char*)m_MaskImage->GetPointData()->
                                         GetScalars()->GetVoidPointer(0);
    for(int i=0; i < size; ++i){
        (*ptr) = 0;  ++ptr;
    }
}