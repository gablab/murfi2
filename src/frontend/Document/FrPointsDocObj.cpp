#include "FrCrosshair.h"
#include "FrPointsDocObj.h"
#include "FrDocument.h"
#include "FrDocumentReader.h"
#include "FrGraphSettings.h"
#include "RtMRIImage.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>

#include <algorithm>

FrPointsDocObj::FrPointsDocObj(){
    m_Points.clear();
 
    m_dimensions[0] = 0; 
    m_dimensions[1] = 0; 
    m_dimensions[2] = 0;
    m_spacing[0] = 0; 
    m_spacing[1] = 0; 
    m_spacing[2] = 0;

    crosshairOn = true;
}

void FrPointsDocObj::GetDimsFromImg(RtMRIImage* img){
    if (img){
        m_dimensions[0] = img->getDim(0);
        m_dimensions[1] = img->getDim(1);
        m_dimensions[2] = img->getDim(2);
        m_spacing[0] = img->getPixDim(0);
        m_spacing[1] = img->getPixDim(1);
        m_spacing[2] = img->getPixDim(2);
    }
}

//FrPointsDocObj::FrPointsDocObj(FrPointsDocObj* src){
//    if (src){
//        // copy points
//        this->CopyPoints(src);
//
//        // copy dimensions and spacing
//        m_dimensions[0] = src->m_dimensions[0];
//        m_dimensions[1] = src->m_dimensions[1];
//        m_dimensions[2] = src->m_dimensions[2];
//        m_spacing[0] = src->m_spacing[0];
//        m_spacing[1] = src->m_spacing[1];
//        m_spacing[2] = src->m_spacing[2];
//    }
//}

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

void FrPointsDocObj::CopySettings(FrPointsDocObj* src){
    if (src){
        m_Points.clear();

        // copy points 
        for (int i = 0; i < src->m_Points.size(); i++){
            FrPoint* tmp = src->m_Points[i];
            FrPoint* point = new FrPoint(tmp->x, tmp->y, tmp->z, tmp->color);
            m_Points.push_back(point);
        }

        // copy dimensions and spacing
        m_dimensions[0] = src->m_dimensions[0];
        m_dimensions[1] = src->m_dimensions[1];
        m_dimensions[2] = src->m_dimensions[2];
        m_spacing[0] = src->m_spacing[0];
        m_spacing[1] = src->m_spacing[1];
        m_spacing[2] = src->m_spacing[2];
    }
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
    if (point){
        PointCollection::iterator it, itEnd(m_Points.end());
        for(it = m_Points.begin(); it != itEnd; ++it){
            if ((*it)->x == point->x && (*it)->y == point->y
                && (*it)->z == point->z){
                    m_Points.erase(it);
                    //delete (*it);
                    delete point;
                    break;
            }
        }
    }
}

vtkImageData* FrPointsDocObj::GetPointsXY(int z){
    if (m_Points.size() == 0)
        return 0L;

    vtkImageData* data = vtkImageData::New();
	data->SetScalarTypeToUnsignedChar();
	data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(m_dimensions[0], m_dimensions[1], 1);
    //data->SetExtent(0, m_dimensions[0]-1, 0, m_dimensions[1]-1, 0, 0);
    data->SetSpacing(m_spacing[0], m_spacing[1], 1.0);
    data->AllocateScalars();
    //data->UpdateInformation();
 

    //unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();
    vtkDataArray* inArray = data->GetPointData()->GetScalars();
    // init data with transparent values
    memset(inArray->GetVoidPointer(0), 0, inArray->GetSize());

    //// init data with transparent values
    //unsigned char* pointsPtr = (unsigned char*)inArray->GetVoidPointer(0);
    //for(int i=0; i < inArray->GetSize()/4; ++i){
    //    pointsPtr[0] = 255;
    //    pointsPtr[1] = 255;
    //    pointsPtr[2] = 255; 
    //    pointsPtr[3] = 0;
    //    pointsPtr += 4;
    //}
  
    unsigned char* dataPtr = (unsigned char*)inArray->GetVoidPointer(0);
    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
      //if ((*it)->z == z){
            int pos[3];
            pos[0] = (*it)->x; pos[1] = (*it)->y; pos[2] = 0;        
            
	    // draw a cross hair if thats enabled
	    if(crosshairOn) {
	      
	      // x line

	      int itPos[3];
	      itPos[1] = pos[1];
	      itPos[2] = pos[2];
	      for(int itx = 0; itx < m_dimensions[0]; itx++) {
		int pid = data->FindPoint((double)itx, (double)pos[1], (double)pos[2]);
		if (pid >= 0){
		  itPos[0] = itx;
		  int id = data->ComputePointId(itPos); 
		  
		  if (id >= 0){      // point found
		    dataPtr[id*4] = (unsigned char)(*it)->color.red();
		    dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		    dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		    dataPtr[id*4+3] = 255;                  // alpha
		  }
		}
		
	      }

	      // y line

	      itPos[0] = pos[0];
	      itPos[2] = pos[2];
	      for(int ity = 0; ity < m_dimensions[1]; ity++) {
		int pid = data->FindPoint((double)pos[0], (double)ity, (double)pos[2]);
		if (pid >= 0){
		  itPos[1] = ity;
		  int id = data->ComputePointId(itPos); 
		  
		  if (id >= 0){      // point found
		    dataPtr[id*4] = (unsigned char)(*it)->color.red();
		    dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		    dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		    dataPtr[id*4+3] = 255;                  // alpha
		  }
		}
		
	      }

	    }
	    else { // just fill the pixel

	      // check if point is inside of data
	      int pid = data->FindPoint((double)pos[0], (double)pos[1], (double)pos[2]);
	      if (pid >= 0){
                int id = data->ComputePointId(pos); 

                if (id >= 0){      // point found
		  dataPtr[id*4] = (unsigned char)(*it)->color.red();
		  dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		  dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		  dataPtr[id*4+3] = 255;                  // alpha
                }
	      }
	    }
	    //}
    }

    return data;
}

vtkImageData* FrPointsDocObj::GetPointsXZ(int y){
    if (m_Points.size() == 0)
        return 0L;

    vtkImageData* data = vtkImageData::New();
	data->SetScalarTypeToUnsignedChar();
	data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(m_dimensions[0], m_dimensions[2], 1);
    data->SetSpacing(m_spacing[0], m_spacing[2], 1.0);
    data->AllocateScalars(); 

    //unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();
    vtkDataArray* inArray = data->GetPointData()->GetScalars();
    // init data with transparent values
    memset(inArray->GetVoidPointer(0), 0, inArray->GetSize());

    unsigned char* dataPtr = (unsigned char*)inArray->GetVoidPointer(0);
    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
      //if ((*it)->y == y){
            int pos[3];
            pos[0] = (*it)->x; pos[1] = (*it)->z; pos[2] = 0;        

	    // draw a cross hair if thats enabled
	    if(crosshairOn) {
	      
	      // x line

	      int itPos[3];
	      itPos[1] = pos[1];
	      itPos[2] = pos[2];
	      for(int itx = 0; itx < m_dimensions[0]; itx++) {
		int pid = data->FindPoint((double)itx, (double)pos[1], (double)pos[2]);
		if (pid >= 0){
		  itPos[0] = itx;
		  int id = data->ComputePointId(itPos); 
		  
		  if (id >= 0){      // point found
		    dataPtr[id*4] = (unsigned char)(*it)->color.red();
		    dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		    dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		    dataPtr[id*4+3] = 255;                  // alpha
		  }
		}
		
	      }

	      // z line

	      itPos[0] = pos[0];
	      itPos[2] = pos[2];
	      for(int itz = 0; itz < m_dimensions[2]; itz++) {
		int pid = data->FindPoint((double)pos[0], (double)itz, (double)pos[2]);
		if (pid >= 0){
		  itPos[1] = itz;
		  int id = data->ComputePointId(itPos); 
		  
		  if (id >= 0){      // point found
		    dataPtr[id*4] = (unsigned char)(*it)->color.red();
		    dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		    dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		    dataPtr[id*4+3] = 255;                  // alpha
		  }
		}
		
	      }

	    }
	    else { // just fill the pixel
	      int pid = data->FindPoint((double)pos[0], (double)pos[1], (double)pos[2]);
	      if (pid >= 0){
                int id = data->ComputePointId(pos); 

                if (id >= 0){      // point found
		  dataPtr[id*4] = (unsigned char)(*it)->color.red();
		  dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		  dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		  dataPtr[id*4+3] = 255;                  // alpha
                }
	      }
	    }
	    //}
    }

    return data;
}

vtkImageData* FrPointsDocObj::GetPointsYZ(int x){
    if (m_Points.size() == 0)
        return 0L;

    vtkImageData* data = vtkImageData::New();
	data->SetScalarTypeToUnsignedChar();
	data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(m_dimensions[1], m_dimensions[2], 1);
    data->SetSpacing(m_spacing[1], m_spacing[2], 1.0);
    data->AllocateScalars(); 

    //unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();
    vtkDataArray* inArray = data->GetPointData()->GetScalars();
    // init data with transparent values
    memset(inArray->GetVoidPointer(0), 0, inArray->GetSize());

    unsigned char* dataPtr = (unsigned char*)inArray->GetVoidPointer(0);
    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
      //if ((*it)->x == x){
            int pos[3];
            pos[0] = (*it)->y; pos[1] = (*it)->z; pos[2] = 0;        

            
	    // draw a cross hair if thats enabled
	    if(crosshairOn) {
	      
	      // y line

	      int itPos[3];
	      itPos[1] = pos[1];
	      itPos[2] = pos[2];
	      for(int ity = 0; ity < m_dimensions[1]; ity++) {
		int pid = data->FindPoint((double)ity, (double)pos[1], (double)pos[2]);
		if (pid >= 0){
		  itPos[0] = ity;
		  int id = data->ComputePointId(itPos); 
		  
		  if (id >= 0){      // point found
		    dataPtr[id*4] = (unsigned char)(*it)->color.red();
		    dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		    dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		    dataPtr[id*4+3] = 255;                  // alpha
		  }
		}
		
	      }

	      // z line

	      itPos[0] = pos[0];
	      itPos[2] = pos[2];
	      for(int itz = 0; itz < m_dimensions[2]; itz++) {
		int pid = data->FindPoint((double)pos[0], (double)itz, (double)pos[2]);
		if (pid >= 0){
		  itPos[1] = itz;
		  int id = data->ComputePointId(itPos); 
		  
		  if (id >= 0){      // point found
		    dataPtr[id*4] = (unsigned char)(*it)->color.red();
		    dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		    dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		    dataPtr[id*4+3] = 255;                  // alpha
		  }
		}
		
	      }

	    }
	    else { // just fill the pixel
	      int pid = data->FindPoint((double)pos[0], (double)pos[1], (double)pos[2]);
	      if (pid >= 0){
		int id = data->ComputePointId(pos); 
		
                if (id >= 0){      // point found
		  dataPtr[id*4] = (unsigned char)(*it)->color.red();
		  dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		  dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		  dataPtr[id*4+3] = 255;                  // alpha
                }
	      }
	    }
	    //}
    }

    return data;
}

vtkImageData* FrPointsDocObj::GetMosaicData(int dimx, int dimy){
    if (m_Points.size() == 0)
        return 0L;

    vtkImageData* data = vtkImageData::New();
    data->SetScalarTypeToUnsignedChar();
    data->SetNumberOfScalarComponents(4); // ARGB
    data->SetDimensions(dimx, dimy, 1);
    data->SetSpacing(m_spacing[0], m_spacing[1], 1.0);
    data->AllocateScalars();     

    //unsigned char* pointsPtr = (unsigned char*)data->GetScalarPointer();
    vtkDataArray* inArray = data->GetPointData()->GetScalars();

    // init data with transparent values
    memset(inArray->GetVoidPointer(0), 0, inArray->GetSize());
  
    int row, col, num;
    num = dimx/m_dimensions[0];

    unsigned char* dataPtr = (unsigned char*)inArray->GetVoidPointer(0);
    PointCollection::iterator it, itEnd(m_Points.end());
    for(it = m_Points.begin(); it != itEnd; ++it){
        // calculate point position in mosaic
        int pos[3];
        int tmp = (*it)->z/num;
        col = (*it)->z%num;//(*it)->z - num*tmp;
        row = tmp;
        
        pos[0] = col*m_dimensions[0] + (*it)->x; 
        pos[1] = row*m_dimensions[1] + (*it)->y; 
        pos[2] = 0;        

	// draw a cross hair if thats enabled
	if(crosshairOn) {
	      
	  // x line
	  
	  int itPos[3];
	  itPos[1] = pos[1];
	  itPos[2] = pos[2];
	  for(int itx = 0; itx < dimx; itx++) {
	    int pid = data->FindPoint((double)itx, (double)pos[1], (double)pos[2]);
	    if (pid >= 0){
	      itPos[0] = itx;
	      int id = data->ComputePointId(itPos); 
	      
	      if (id >= 0){      // point found
		dataPtr[id*4] = (unsigned char)(*it)->color.red();
		dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		dataPtr[id*4+3] = 255;                  // alpha
	      }
	    }
	    
	  }
	  
	  // y line
	  
	  itPos[0] = pos[0];
	  itPos[2] = pos[2];
	  for(int ity = 0; ity < dimy; ity++) {
	    int pid = data->FindPoint((double)pos[0], (double)ity, (double)pos[2]);
	    if (pid >= 0){
	      itPos[1] = ity;
	      int id = data->ComputePointId(itPos); 
	      
	      if (id >= 0){      // point found
		dataPtr[id*4] = (unsigned char)(*it)->color.red();
		dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
		dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
		dataPtr[id*4+3] = 255;                  // alpha
	      }
	    }
	    
	  }
	}
	else { // just fill the pixel
	  
	  int pid = data->FindPoint((double)pos[0], (double)pos[1], (double)pos[2]);
	  if (pid >= 0){
	    int id = data->ComputePointId(pos); 
	    
	    if (id >= 0){      // point found
	      dataPtr[id*4] = (unsigned char)(*it)->color.red();
	      dataPtr[id*4+1] = (unsigned char)(*it)->color.green();
	      dataPtr[id*4+2] = (unsigned char)(*it)->color.blue();
	      dataPtr[id*4+3] = 255;                  // alpha
	    }
	  }
	}

    }

    return data;
}


CrosshairParams FrPointsDocObj::GetCrosshairParams(int orientation){
  CrosshairParams result;

  if (m_Points.size() == 0)
    return result;

  switch(orientation) {
  case FrDocumentReader::XY:
      result.x = m_Points[0]->x;
      result.y = m_Points[0]->y;    

      result.xmax = m_dimensions[0];
      result.ymax = m_dimensions[1];

      break;

    case FrDocumentReader::XZ:
      result.x = m_Points[0]->x;
      result.y = m_Points[0]->z;    

      result.xmax = m_dimensions[0];
      result.ymax = m_dimensions[2];

      break;

    case FrDocumentReader::YZ:
      result.x = m_Points[0]->y;
      result.y = m_Points[0]->z;  

      result.xmax = m_dimensions[1];
      result.ymax = m_dimensions[2];
  
      break;
  }

  result.xmin = 0;
  result.ymin = 0;

  return result;
}


CrosshairParams FrPointsDocObj::GetCrosshairParamsMosaic(int dimx, int dimy){
  CrosshairParams result;

  if (m_Points.size() == 0)
    return result;

  int row, col, num;
  num = dimx/m_dimensions[0];

  int tmp = m_Points[0]->z/num;
  col = m_Points[0]->z%num;
  row = tmp;
        
  result.x = col*m_dimensions[0] + m_Points[0]->x; 
  result.y = row*m_dimensions[1] + m_Points[0]->y; 

  result.xmin = 0;
  result.xmax = dimx;
  result.ymin = 0;
  result.ymax = dimy;

  return result;
}



void FrPointsDocObj::GetPoint(int* point){
    if (m_Points.size() == 0){
       point[0] = -1;
       return;
    }

    point[0] = m_Points[0]->x;
    point[1] = m_Points[0]->y;
    point[2] = m_Points[0]->z;
}

void FrPointsDocObj::InitTransparentData(){
}

