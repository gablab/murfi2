#include "FrImageDocObj.h"
#include "FrDocument.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"
// Backend includes
#include "RtMRIImage.h"


FrImageDocObj::FrImageDocObj()
:m_origin(0), m_copy(0) {
}

FrImageDocObj::~FrImageDocObj(){
    if(m_origin) delete m_origin;
    if(m_copy) m_copy->Delete();
}

void FrImageDocObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrImageDocObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjType FrImageDocObj::GetType(){
    return FrDocumentObj::ImageObject;
}

void FrImageDocObj::UpdateObject(){    
    if(!m_origin) return;
    if(m_copy) m_copy->Delete();

    // Update m_copy object from m_origin
    m_copy = vtkImageData::New();

    double dimW = m_origin->getDim(0);
    double dimH = m_origin->getDim(1);
	m_copy->SetDimensions(dimW, dimH, 1.0);
//	m_copy->SetDimensions(64, 64, 36);

    double w = m_origin->getPixDim(0);
    double h = m_origin->getPixDim(1);
	m_copy->SetSpacing(w, h, 1.0);

//	m_copy->SetExtent(0, 63, 0, 63, 0, 35);
//	m_copy->SetWholeExtent(0, 63, 0, 63, 0, 35);
//	m_copy->SetUpdateExtentToWholeExtent();
  
	m_copy->SetNumberOfScalarComponents(1);
    m_copy->SetScalarTypeToUnsignedChar();
    m_copy->AllocateScalars();
    
    //m_copy->GetPointData()->GetScalars()->FillComponent(0, 0);
    short* dataPtr = m_origin->getDataCopy();
    unsigned char* dstPtr = (unsigned char*)m_copy->
        GetPointData()->GetScalars()->GetVoidPointer(0);

    short* srcPtr = dataPtr;
	int num = m_origin->getNumPix();
	//num = 64*64*36;
    for(int i=0; i < num; i++){
        *dstPtr = (unsigned char)(*srcPtr);
        dstPtr++;
        srcPtr++;
    }
    delete[] dataPtr;
    
    /*for(int i=0; i < dimW; ++i){
        for(int j=0; j < dimH; ++j){
            if(i == j || (i+j) == dimW){
                m_copy->SetScalarComponentFromDouble(i, j, 0, 0, 200); 
            }
        }
    }*/
    //void* dataSrc = m_origin->getDataCopy();
    //void* dataDst = m_copy->GetPointData()->GetScalars()->GetVoidPointer(0);    
    //memcpy(dataDst, dataSrc, m_origin->getNumPix() * m_origin->getBytesPerPix());
    //delete dataSrc;
}

#include <Qt/QString.h>
#include <Qt/QFile.h>
bool FrImageDocObj::LoadFromFile(QString& fileName){
    bool result = false;
    
    if(QFile::exists(fileName)){
        std::string stdFileName = fileName.toAscii();
        RtMRIImage* img = new RtMRIImage();
        
        if(img->readNifti(stdFileName)){
            if(m_origin){
                delete m_origin;
            }

            m_origin = img;
		
            SetUpdateNeeded(true);
            result = true;
        }
    }
    return result;
}