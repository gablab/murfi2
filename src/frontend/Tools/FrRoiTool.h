#ifndef FR_ROI_TOOL
#define FR_ROI_TOOL

#include "FrTool.h"

// ROI tool
class FrRoiTool : public FrTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

public:
	// Default constructor
	FrRoiTool();
	// Destructor
	virtual ~FrRoiTool();
};

#endif
