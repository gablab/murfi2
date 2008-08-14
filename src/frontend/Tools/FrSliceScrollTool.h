#ifndef FR_SLICE_SCROLL_TOOL
#define FR_SLICE_SCROLL_TOOL


#include "FrTool.h"

/// abstract class for all tools
// used by application
class FrSliceScrollTool : public FrTool
{
public:
    /// Default constructor
	FrSliceScrollTool();
	/// Destructor
	virtual ~FrSliceScrollTool();
	
    virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrMouseParams& params);
	virtual bool OnMouseDown(FrMouseParams& params);
	virtual bool OnMouseMove(FrMouseParams& params);
	virtual bool OnMouseDrag(FrMouseParams& params);
	
};

#endif
