#include "FrSelection.h"
#include "FrUtils.h"


// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageMapToColors.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkActor.h"
#include "vtkPointData.h"


FrSelection* FrSelection::New(){
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("FrSelection");
	if(ret){
		return (FrSelection*)ret;
	}
	// If the factory was unable to create the object, then create it here.
	return new FrSelection;
}

// Construct object with no children.
FrSelection::FrSelection(){
}

FrSelection::~FrSelection(){
}

