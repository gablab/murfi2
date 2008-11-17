#ifndef FR_BASEROI_TOOL
#define FR_BASEROI_TOOL

// includes
#include "FrTool.h"
#include "FrMacro.h"

class FrMainController;

// Base class for all ROI tools.
// Provides common functionality.
class FrBaseRoiTool : public FrTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    // Properties
    void SetImageNumber(int value){
        m_ImageNumber = value;
    }

public:
	/// Default constructor
	FrBaseRoiTool();
	/// Destructor
	virtual ~FrBaseRoiTool();

protected:
    FrMainController* GetMainController();

protected:
    int m_ImageNumber;

};

#endif // FR_BASEROI_TOOL
