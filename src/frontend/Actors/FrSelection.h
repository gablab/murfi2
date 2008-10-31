#ifndef FR_SELECTION
#define FR_SELECTION

#include "FrBaseActor.h"
#include "FrMacro.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include "vtkPlaneSource.h"
#include "vtkTexture.h"
#include "vtkProperty.h"
#include "vtkExtractVOI.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkFollower.h"


class FrSelection : public FrBaseActor
{
public:
    enum Type { Unknown, Rectangle, Circle, Point };	// may be Points?

public:
	static FrSelection *New();
	vtkTypeMacro(FrSelection, FrBaseActor);

    // Properties
    FrSetPropMacro(Type, CurrentType);

protected:

	FrSelection();
	virtual ~FrSelection();
	FrSelection(const FrSelection&) {};
	void operator=(const FrSelection&) {};

};

#endif





