#ifndef FR_MAIN_CONTROLLER
#define FR_MAIN_CONTROLLER

#include "FrController.h"

// Forward declaration
class FrMainWindow;
class FrMainDocument;
class FrToolController;
class FrInteractorStyle;
class FrTool;

class QString;

class FrMainController : public FrController {
public:
    FrMainController(FrMainWindow* view = 0, FrMainDocument* doc = 0);
    ~FrMainController();
    
    void Initialize();

public:
    // Interface to tools
    bool HasActiveTool();
    FrTool* GetCurrentTool();

public:
    // Interface for MainWindow
    void LoadImage(QString& fileName);

    // Threshold/brightness/contrast interface
    enum TBC { Threshold, Brightness, Contrast };
    void SetValueTBC(TBC target, double value);
	void SetNextSlice();
    
	// Override from base class
    virtual void Notify(int notifyCode);

private:
    friend class FrInteractorStyle;

private:
    FrMainWindow* m_view;
    FrMainDocument* m_document;
    FrToolController* m_toolController;
};

#endif