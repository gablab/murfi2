#ifndef FR_SPHERE_TOOL
#define FR_SPHERE_TOOL

// includes
#include "FrBaseRoiTool.h"
#include "FrMacro.h"

// abstract class for all tools
// used by application
class FrSphereTool : public FrBaseRoiTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    FrSetPropMacro(int, ImageNumber);

public:
	/// Default constructor
	FrSphereTool();
	/// Destructor
	virtual ~FrSphereTool();
};

#endif // FR_RECTANGLE_TOOL
