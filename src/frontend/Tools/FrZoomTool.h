#ifndef FR_ZOOM_TOOL
#define FR_ZOOM_TOOL

#include "FrTool.h"

/// abstract class for all tools
// used by application
class FrZoomTool : public FrTool
{
public:
	/// Default constructor
	FrZoomTool();
	/// Destructor
	virtual ~FrZoomTool();
    
    virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrMouseParams& params);
	virtual bool OnMouseDown(FrMouseParams& params);
	virtual bool OnMouseMove(FrMouseParams& params);
	virtual bool OnMouseDrag(FrMouseParams& params);
};

#endif
