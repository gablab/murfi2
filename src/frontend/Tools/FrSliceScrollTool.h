#ifndef FR_SLICE_SCROLL_TOOL
#define FR_SLICE_SCROLL_TOOL

#include "FrTool.h"
#include "FrMacro.h"

// Forward declaration
class FrMainDocument;

/// abstract class for all tools
// used by application
class FrSliceScrollTool : public FrTool
{
public:
    /// Default constructor
	FrSliceScrollTool(FrMainDocument* document=0);
	/// Destructor
	virtual ~FrSliceScrollTool();
	
    virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    FrPropMacro(FrMainDocument*, Document);

private:
	bool SetSlice(int inc, FrInteractorStyle* is);
	bool SetMousePosition(int x, int y);

private:
    int m_oldX;
    int m_oldY;
	int m_sliceNumber;
};

#endif
