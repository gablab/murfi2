#ifndef FR_ROIINFO_TOOL
#define FR_ROIINFO_TOOL

// includes
#include "FrBaseRoiTool.h"
#include "FrMacro.h"

// abstract class for all tools
// used by application
class FrRoiInfoTool : public FrBaseRoiTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

public:
	/// Default constructor
	FrRoiInfoTool();
	/// Destructor
	virtual ~FrRoiInfoTool();
};

#endif // FR_ROIINFO_TOOL
