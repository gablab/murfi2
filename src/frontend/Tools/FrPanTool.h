#ifndef FR_PAN_TOOL
#define FR_PAN_TOOL


#include "FrTool.h"

/// abstract class for all tools
// used by application
class FrPanTool : public FrTool {
public:
	/// Default constructor
	FrPanTool();
	/// Destructor
	virtual ~FrPanTool();

	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

	virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

private:
    int m_oldX;
    int m_oldY;
};

#endif
