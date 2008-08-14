#ifndef FR_PAN_TOOL
#define FR_PAN_TOOL


#include "FrTool.h"

/// abstract class for all tools
// used by application
class FrPanTool : public FrTool
{
public:
	/// Default constructor
	FrPanTool();
	/// Destructor
	virtual ~FrPanTool();

	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrMouseParams& params);
	virtual bool OnMouseDown(FrMouseParams& params);
	virtual bool OnMouseMove(FrMouseParams& params);
	virtual bool OnMouseDrag(FrMouseParams& params);

};

#endif
