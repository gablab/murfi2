#ifndef FR_MASKMASK_TOOL
#define FR_MASKMASK_TOOL

// includes
#include "FrBaseRoiTool.h"
#include "FrMacro.h"

// abstract class for all tools
// used by application
class FrMaskMaskTool : public FrBaseRoiTool {
public:
    enum Mode { Undefined, Union, Intersect, Subtract, Copy };
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

	virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

    // Properties
    FrSetPropMacro(Mode, Mode);

public:
	/// Default constructor
	FrMaskMaskTool();
	/// Destructor
	virtual ~FrMaskMaskTool();
};

#endif // FR_MASKMASK_TOOL
