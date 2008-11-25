#ifndef FR_PEN_TOOL
#define FR_PEN_TOOL

// includes
#include "FrBaseRoiTool.h"
#include "FrMacro.h"

// abstract class for all tools
// used by application
class FrPenTool : public FrBaseRoiTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

    FrSetPropMacro(int, ImageNumber);

public:
	/// Default constructor
	FrPenTool();
	/// Destructor
	virtual ~FrPenTool();
};

#endif // FR_PEN_TOOL
