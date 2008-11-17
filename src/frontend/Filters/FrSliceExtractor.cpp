#include "FrSliceExtractor.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkObjectFactory.h"
#include "vtkImageReslice.h"

#include <vector>

// Defines
#define DEF_SPACING 1.0
#define DEF_FRAME 0
#define DEF_PORT_NUM 0


vtkStandardNewMacro(FrSliceExtractor);


FrSliceExtractor::FrSliceExtractor(){
    // Create default reslicer
    m_Reslicer.clear();

    vtkImageReslice* rs = vtkImageReslice::New();
    m_Reslicer[DEF_PORT_NUM] = rs;

	m_Slice = DEF_FRAME;    
    m_Orientation = FrSliceExtractor::XY;
    m_Spacing[0] = m_Spacing[1] = m_Spacing[2] = DEF_SPACING;
}

FrSliceExtractor::~FrSliceExtractor(){
    ReslicerCollection::iterator it, itEnd(m_Reslicer.end());
    for(it = m_Reslicer.begin(); it != itEnd; ++it){
        if(it->second){
            it->second->Delete();
            it->second = 0L;
        }
    }

    m_Reslicer.clear();
}

void FrSliceExtractor::SetInput(int port, vtkImageData* input){
    if(input == this->GetInput(port)) return;    

    ReslicerCollection::iterator it = m_Reslicer.find(port);
    if(it == m_Reslicer.end()){
        // add new reslicer
        vtkImageReslice* rs = vtkImageReslice::New();
        m_Reslicer[port] = rs;
        
        it = m_Reslicer.find(port);
    }

    it->second->SetInput(input);
    this->Modified();
    
    // Set spacing using info from def port data
    if(port == DEF_PORT_NUM){
        if(input != 0L){
            input->GetSpacing(m_Spacing);
        }
        else {
            m_Spacing[0] = m_Spacing[1] = m_Spacing[2] = DEF_SPACING;
        }
    }
}

void FrSliceExtractor::SetInput(vtkImageData* input){
    this->SetInput(DEF_PORT_NUM, input);
}

vtkImageData* FrSliceExtractor::GetInput(int port){
    vtkImageData* result = 0L;

    ReslicerCollection::iterator it = m_Reslicer.find(port);
    if(it != m_Reslicer.end()){
        result = (vtkImageData*)it->second->GetInput();
    }
    return result;
}

vtkImageData* FrSliceExtractor::GetInput(){
    return this->GetInput(DEF_PORT_NUM);
}

vtkImageData* FrSliceExtractor::GetOutput(int port){
    vtkImageData* result = 0L;

    ReslicerCollection::iterator it = m_Reslicer.find(port);
    if(it != m_Reslicer.end()){
        result = it->second->GetOutput();
    }
    return result;
}

vtkImageData* FrSliceExtractor::GetOutput(){
    return this->GetOutput(DEF_PORT_NUM);
}

void FrSliceExtractor::SetOrientation(FrSliceExtractor::Orientation orient){
    if(m_Orientation != orient){
        m_Orientation = orient;
        this->Modified();
    }
}

void FrSliceExtractor::SetSlice(int slice){
    if(m_Slice != slice){
        m_Slice = slice;
	    this->Modified();
    }
}

void FrSliceExtractor::Update(){
    // For safty
    if(m_Reslicer.size() <= 0 || 
      !m_Reslicer[0]->GetInput()) return;

    ReslicerCollection::iterator it, itEnd(m_Reslicer.end());
    for(it = m_Reslicer.begin(); it != itEnd; ++it){
	    switch (m_Orientation)
	    {
        case FrSliceExtractor::XY:		// XY
			    it->second->SetResliceAxesOrigin(0, 0, m_Slice);
			    it->second->SetResliceAxesDirectionCosines(1,0,0, 0,1,0, 0,0,1);
			    break;
        case FrSliceExtractor::XZ:		// XZ
                it->second->SetResliceAxesOrigin(0, m_Slice, 0);
                it->second->SetResliceAxesDirectionCosines(1,0,0, 0,0,1, 0,1,0);
			    break;
        case FrSliceExtractor::ZY:		// ZY
                it->second->SetResliceAxesOrigin(m_Slice, 0, 0);
                it->second->SetResliceAxesDirectionCosines(0,1,0, 0,0,1, 1,0,0);
			    break;
		    default:
			    // Do nothing
                vtkErrorMacro(<<"FrSliceExtrator: unrecognized orientation");
                return;
	    }
        it->second->InterpolateOff();
	    it->second->SetOutputDimensionality(2);
	    it->second->SetOutputSpacing(m_Spacing);
	    it->second->Update();
    }
}

int FrSliceExtractor::GetMaxSliceNumber(){
    int result = -1;

    if((m_Reslicer.size() > 0) && 
       (m_Reslicer[0]->GetInput() != 0L) ){
        
        int dimentions[3];
        vtkImageData* img = (vtkImageData*)m_Reslicer[0]->GetInput();
        img->GetDimensions( dimentions);	
	
	    switch (m_Orientation)
	    {
	    case FrSliceExtractor::XY:		// XY
		    result = dimentions[2]; 
		    break;
	    case FrSliceExtractor::XZ:		// XZ	
		    result = dimentions[1];
		    break;
	    case FrSliceExtractor::ZY:		// ZY
		    result = dimentions[0];
		    break;
	    }
    }
	return result;
}

void FrSliceExtractor::ClearAdditionalPorts(){
    std::vector<int> portsToDelete;

    // Get all port# to be deleted
    ReslicerCollection::iterator it, itEnd(m_Reslicer.end());
    for(it = m_Reslicer.begin(); it != itEnd; ++it){
        if(it->first != DEF_PORT_NUM) {
            portsToDelete.push_back(it->first);
            it->second->Delete();
            it->second = 0L;
        }
    }

    std::vector<int>::iterator itr, itrEnd(portsToDelete.end());
    for(itr = portsToDelete.begin(); itr != itrEnd; ++itr){
        m_Reslicer.erase( (*itr) );
    }
}