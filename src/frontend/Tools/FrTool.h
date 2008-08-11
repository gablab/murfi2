#ifndef FR_TOOL
#define FR_TOOL


class FrToolController;

/// abstract class for all tools
// used by application
class FrTool
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
    virtual bool OnMouseUp(int x, int y, bool shift = false, bool control = false) = 0;
	virtual bool OnMouseDown(int x, int y, bool shift = false, bool control = false) = 0;
	virtual bool OnMouseMove(int x, int y, bool shift = false, bool control = false) = 0;
	virtual bool OnMouseDrag(int x, int y, bool shift = false, bool control = false) = 0;
	virtual bool OnMouseDoubleClick(int x, int y, bool shift = false, bool control = false) = 0;

public:
	/// Default constructor
	FrTool();
	/// Destructor
	virtual ~FrTool();

    FrToolController* GetController();
    void SetController(FrToolController* ctrl);

private:
	/// Controller owning the tool.
	FrToolController* m_controller;
};

#endif
