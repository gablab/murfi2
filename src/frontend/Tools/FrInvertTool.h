#ifndef FR_INVERT_TOOL
#define FR_INVERT_TOOL

// includes
#include "FrBaseRoiTool.h"
#include "FrMacro.h"

// abstract class for all tools
// used by application
class FrInvertTool : public FrBaseRoiTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

	virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

public:
	/// Default constructor
	FrInvertTool();
	/// Destructor
	virtual ~FrInvertTool();
};

#endif // FR_INVERT_TOOL
