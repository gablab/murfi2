#ifndef FR_COMPOSITE_TOOL
#define FR_COMPOSITE_TOOL

#include "FrTool.h"
#include "FrMacro.h"

class FrPanTool;
class FrZoomTool;
class FrTBCTool;
class FrSliceScrollTool;
class FrMainDocument;

// Composite tool. This tool delagate
// all events to incapsulated tools.
class FrCompositeTool : public FrTool
{
public:
    // Default constructor
	FrCompositeTool();
	// Destructor
	virtual ~FrCompositeTool();
	
    virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);
    
	// Check mouse coordinates for zoom/pan tool selection
	bool CheckMouseParams(FrInteractorStyle* is, FrMouseParams& params);

private:
    FrPanTool*  m_panTool;
    FrZoomTool* m_zoomTool;
    FrTBCTool*  m_tbcTool;
	FrSliceScrollTool* m_ssTool;
	bool isZoom;

};

#endif
