#include "FrBaseActor.h"
#include "vtkObjectFactory.h"

//------------------------------------------------------------------------------
FrBaseActor* FrBaseActor::New(){
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("FrBaseActor");
	if(ret){
		return (FrBaseActor*)ret;
	}
	// If the factory was unable to create the object, then create it here.
	return new FrBaseActor;
}

// Construct object with no children.
FrBaseActor::FrBaseActor(){
}

FrBaseActor::~FrBaseActor(){
}

