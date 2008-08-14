#ifndef FR_MAIN_CONTROLLER
#define FR_MAIN_CONTROLLER

// Forward declaration
class FrMainWindow;
class FrMainDocument;
class FrToolController;
class FrInteractorStyle;
class FrTool;

class FrMainController {
public:
    FrMainController(FrMainWindow* view = 0, FrMainDocument* doc = 0);
    ~FrMainController();
    
    void Initialize();

    // Interface to tools
    bool HasActiveTool();
    FrTool* GetCurrentTool();

    // Interface for MainWindow
    //void LoadImage(QString& fileName);

private:
    friend class FrInteractorStyle;

private:
    FrMainWindow* m_view;
    FrMainDocument* m_document;
    FrToolController* m_toolController;
};

#endif