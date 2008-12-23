#include "FrPointsDocObj.h"
#include "FrDocument.h"
#include "FrGraphSettings.h"
#include "RtMRIImage.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>

FrPointsDocObj::FrPointsDocObj(RtMRIImage* img){
    m_Points.clear();
 
    if (img){
        m_dimensions[0] = img->getDim(0);   
        m_dimensions[1] = img->getDim(1);   
        m_dimensions[2] = img->getDim(2);
        m_spacing[0] = img->getPixDim(0);
        m_spacing[1] = img->getPixDim(1);
        m_spacing[2] = img->getPixDim(2);
    }
}

FrPointsDocObj::~FrPointsDocObj(){
    this->ClearAll();
//    if (m_PointsData) m_PointsData->Delete();
}

void FrPointsDocObj::OnAdd(FrDocument* doc){
}

void FrPointsDocObj::OnRemove(FrDocument* doc){
}

FrDocumentObj::ObjTypes FrPointsDocObj::GetType(){
    return FrDocumentObj::PointsObject;
}

void FrPointsDocObj::ClearAll(){
    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_Points.clear();
}

void FrPointsDocObj::AddPoint(FrPoint* point){
    if (point){
        m_Points.push_back(point);
    }
}
 
void FrPointsDocObj::RemovePoint(FrPoint *point){
    // TODO: implement 
    if (point){
    }
}

vtkImageData* FrPointsDocObj::GetPointsXY(int z){
    vtkImageData* data = vtkImageData::New();
	data->SetScalarTypeToUnsignedChar();
	data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(m_dimensions[0], m_dimensions[1], 1);
    data->SetExtent(0, m_dimensions[0]-1, 0, m_dimensions[1]-1, 0, 0);
    data->AllocateScalars(); 

    //unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();
    vtkDataArray* inArray = data->GetPointData()->GetScalars();
    unsigned char* pointsPtr = (unsigned char*)inArray->GetVoidPointer(0);

    // init data with transparent values
    for(int i=0; i < inArray->GetSize()/4; ++i){
        pointsPtr[0] = 255;
        pointsPtr[1] = 255;
        pointsPtr[2] = 255; 
        pointsPtr[3] = 0;
        pointsPtr += 4;
    }
  
    unsigned char* dataPtr = (unsigned char*)inArray->GetVoidPointer(0);
    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
        if ((*it)->z == z){
            double pos[3];
            pos[0] = (*it)->x; pos[1] = (*it)->y; pos[2] = 0;        

            int id = data->FindPoint(pos); 

            if (id >= 0){      // point found
                dataPtr[id*4] = (unsigned char)(*it)->color.red();
                dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
                dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
                dataPtr[id*4+3] = 255;                  // alpha
            }
        }
    }

    return data;
}

vtkImageData* FrPointsDocObj::GetPointsXZ(int y){
    vtkImageData* data = vtkImageData::New();
	data->SetScalarTypeToUnsignedChar();
	data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(m_dimensions[0], 1, m_dimensions[2]);
	data->SetSpacing(m_spacing);

    unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();

    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
        if ((*it)->y == y){
            int pos[3];
            pos[0] = (*it)->x; pos[1] = 0; pos[2] = (*it)->z;        

            int id = data->ComputePointId(pos); 

            if (id >= 0){      // point found
                pointsPtr[id] = (unsigned char)(*it)->color.red();
                pointsPtr[id+1] = (unsigned char)(*it)->color.green();
                pointsPtr[id+2] = (unsigned char)(*it)->color.blue();
                pointsPtr[id+3] = 0;                  // alpha
            }
        }
    }

    return data;
}

vtkImageData* FrPointsDocObj::GetPointsYZ(int x){
    vtkImageData* data = vtkImageData::New();
	data->SetScalarTypeToUnsignedChar();
	data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(1, m_dimensions[1], m_dimensions[2]);
	data->SetSpacing(m_spacing);

    unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();

    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
        if ((*it)->x == x){
            int pos[3];
            pos[0] = 0; pos[1] = (*it)->y; pos[2] = (*it)->z;        

            int id = data->ComputePointId(pos); 

            if (id >= 0){      // point found
                pointsPtr[id] = (unsigned char)(*it)->color.red();
                pointsPtr[id+1] = (unsigned char)(*it)->color.green();
                pointsPtr[id+2] = (unsigned char)(*it)->color.blue();
                pointsPtr[id+3] = 0;                  // alpha
            }
        }
    }

    return data;
}

void FrPointsDocObj::InitTransparentData(){
}