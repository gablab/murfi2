#ifndef FR_MASKRECTANGLE_TOOL
#define FR_MASKRECTANGLE_TOOL


#include "FrTool.h"
#include "FrMacro.h"

class vtkPointPicker;


/// abstract class for all tools
// used by application
class FrMaskRectangleTool : public FrTool {
public:
    enum State{None, FirstPoint, Drawing};
public:
	/// Default constructor
	FrMaskRectangleTool();
	/// Destructor
	virtual ~FrMaskRectangleTool();

	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

private:
    FrPropMacro(State, CurrentState);

private:
    int m_firstPointX;
    int m_firstPointY;
    vtkPointPicker* m_pointPicker;

};

#endif
