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
    
    void SaveCurrentViewToTab();
    void ChangeView(int view);

    void ChangeBookmark(int id);
    void DeleteBookmark(int id);

	// Override from base class
    virtual void Notify(int notifyCode);

    // Properties
    FrGetPropMacro(FrMainWindow*, MainView);
    FrGetPropMacro(FrMainDocument*, MainDocument);

private:
    friend class FrInteractorStyle;

private:
    FrToolController* m_toolController;
};

#endif