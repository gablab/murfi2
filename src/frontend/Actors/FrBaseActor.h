// .NAME vtkpxBaseCurve - an abstract base class derived from vtkAssembly
// .SECTION Description
// This is an abstract base class of vtkAssembly used to parent
// classes aiming at constructing complex objects consisting of multiple actors
// Methos SetMapper SetTexture SetProperty etc are made protected
// so as not to allow external access.
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkpxImageSlice 

#ifndef FR_BASE_ACTOR
#define FR_BASE_ACTOR

#include "vtkAssembly.h"


class FrBaseActor : public vtkAssembly {
public:
	static FrBaseActor *New();
	vtkTypeMacro(FrBaseActor, vtkAssembly);


protected:
	// This allow us to construct and destruct in derived classes
	FrBaseActor();
	~FrBaseActor();

private:
	// VTK style
	FrBaseActor(const FrBaseActor&);		// Not implemented.
	void operator=(const FrBaseActor&);		// Not implemented.

};
#endif
