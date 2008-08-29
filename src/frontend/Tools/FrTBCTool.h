#ifndef FR_TBC_TOOL
#define FR_TBC_TOOL

#include "FrTool.h"
#include "FrMacro.h"

// Forward declaration
class FrMainDocument;

#define MOVE_THRESHOLD 3.0f

// This tool can be used for threshold/brightness/contrast
// adjusting of medical images. We use logarithmic scale here.
// The futher mouse pointer from click point more delta is 
// added to/subtracted from adjusted value.
// Left button y-motion for brightness
// Right button y-motion for contrast
// Middle button y-motion for threshold
class FrTBCTool : public FrTool
{
public:
    FrTBCTool(FrMainDocument* document=0);
    virtual ~FrTBCTool();

	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    FrPropMacro(FrMainDocument*, Document);

private:
    int m_downX;
    int m_downY;
};

#endif
