#ifndef FR_ZOOM_TOOL
#define FR_ZOOM_TOOL

#define MAX_ZOOM 1000
#define MIN_ZOOM 5.0

#include "FrTool.h"

/// abstract class for all tools
// used by application
class FrZoomTool : public FrTool {
public:
	/// Default constructor
	FrZoomTool();
	/// Destructor
	virtual ~FrZoomTool();
    
    virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

	virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

private:
    void ZoomByScaling(double factor, FrInteractorStyle* is);

private:
    int m_oldX;
    int m_oldY;
    double m_zoomFactor;

};

#endif
