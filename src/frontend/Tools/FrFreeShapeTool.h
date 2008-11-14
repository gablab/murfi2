#ifndef FR_FREESHAPE_TOOL
#define FR_FREESHAPE_TOOL

// includes
#include "FrTool.h"
#include "FrMacro.h"
#include "FrSelection.h"

#include <vector>

// abstract class for all tools
// used by application
class FrFreeShapeTool : public FrTool {
public:
    enum State{None, FirstPoint, Drawing};
public:
	/// Default constructor
	FrFreeShapeTool();
	/// Destructor
	virtual ~FrFreeShapeTool();

public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    FrSetPropMacro(int, ImageNumber);

private:
    bool CheckPoint(Pos &pos);

private:
    FrPropMacro(State, CurrentState);
    std::vector<Pos> Points;

};

#endif // FR_FREESHAPE_TOOL
