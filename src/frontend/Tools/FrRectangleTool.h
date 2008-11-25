#ifndef FR_RECTANGLE_TOOL
#define FR_RECTANGLE_TOOL

#include "FrBaseRoiTool.h"
#include "FrMacro.h"


/// abstract class for all tools
// used by application
class FrRectangleTool : public FrBaseRoiTool {
public:
    enum State{None, FirstPoint, Drawing};
public:
	/// Default constructor
	FrRectangleTool();
	/// Destructor
	virtual ~FrRectangleTool();

	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

	virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

private:
    FrPropMacro(State, CurrentState);

private:
    int m_firstPointX;
    int m_firstPointY;

};

#endif
