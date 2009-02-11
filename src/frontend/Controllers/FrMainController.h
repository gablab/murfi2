#ifndef FR_MAIN_CONTROLLER
#define FR_MAIN_CONTROLLER

// Forward declaration
class FrTool;
class FrMainWindow;
class FrMainDocument;
class FrToolController;
class FrInteractorStyle;

class QString;


// Come includes
#include "FrController.h"
#include "FrBackground.h"
#include <vector>
#include "RtExperiment.h"

class FrMainController : public FrController {
public:
    FrMainController(FrMainWindow* view = 0, FrMainDocument* doc = 0, RtConductor* con = 0);
    virtual ~FrMainController();
    
    void Initialize();

public:
    // Interface to tools
    bool HasActiveTool();
    FrTool* GetCurrentTool();

public:
    // Testing ... 
    void Test();

    // Interface for MainWindow
    void LoadImageFromFile(std::vector<QString>& fileNames);
    void OpenDataStore();
    void IoTabSettings(QString& fileName, bool isInput);
    void IoGraphTabSettings(QString& fileName, bool isInput);
    
    void SaveCurrentViewToTab();
    void SaveCurrentGraphToTab();

    void ChangeView(int view);

    void SelectLayer(int id);
	void AddLayer();
    void DeleteLayer();
    void ChangeLayer(int action);
    void ChangeImageSettings();

    void ChangeBookmark(int id);
    void DeleteBookmark(int id);
    void ChangeGraphBookmark(int id);
    void DeleteGraphBookmark(int id);

    void SetCurrentTool(int tool);

    void ResetImage();
    void CreatNewROI();
    void UpdateRoiTool();

    void SetLiveMode(bool newLiveMode);
    void SetCurrentTimePoint(int newTimePoint);
    void SetPreviousTimePoint();
    void SetNextTimePoint();
    void ChangeGraph(int id, bool add);

	// Override from base class
    virtual void Notify(int notifyCode);
   

    // Properties
    FrGetPropMacro(FrMainWindow*, MainView);
    FrGetPropMacro(FrMainDocument*, MainDocument);
    FrGetPropMacro(FrToolController*, ToolController);
    /* ohinds 2009-02-02
     * this is unnecessary, RtExperiment takes care of conductors
    FrGetPropMacro(RtConductor*, Conductor);
    */
    //FrGetPropMacro(FrBackground, ConductorThr);

private:
    friend class FrInteractorStyle;

private:
    /* ohinds 2009-02-02
     * this is unnecessary, RtExperiment takes care of conductors
    static void* ConductorThread(void *arg);
    int ThrID;
    FrBackground ConductorThr;
    */
};

#endif
