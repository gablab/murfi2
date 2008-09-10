#ifndef FR_BASE_VIEW
#define FR_BASE_VIEW

class QVTKWidget;
class FrMainWindow;

#include "FrMacro.h"

#define QTVIEW3D (this->GetQtView())

// Base view for all other views
class FrBaseView
{ 
public:
    FrBaseView(FrMainWindow* mainWindow);
    virtual ~FrBaseView();
        
    // Virtual methods
    // This method is called when initialization is needed
    virtual void Initialize() = 0;
    // This methosd is called whenever 
    virtual void SetupPipeline() = 0;
    // This method is called when update needed
    virtual void UpdatePipeline() = 0;
    // This method setups renderers
    virtual void SetupRenderers() = 0;
    // This method removes renderers
    virtual void RemoveRenderers() = 0;

    // Properties
    FrGetPropMacro(QVTKWidget*, QtView);
    FrGetPropMacro(FrMainWindow*,MainWindow);
};

#endif


