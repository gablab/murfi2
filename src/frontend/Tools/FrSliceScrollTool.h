#ifndef FR_SLICE_SCROLL_TOOL
#define FR_SLICE_SCROLL_TOOL

#include "FrTool.h"
#include "FrMacro.h"

// Forward declaration
class FrMainDocument;

// This tool allows scrolling of slices
class FrSliceScrollTool : public FrTool {
public:
    /// Default constructor
	FrSliceScrollTool();
	/// Destructor
	virtual ~FrSliceScrollTool();
	
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
    int m_changingSlice;
};

#endif
