#ifndef FR_MOSAIC_VIEW
#define FR_MOSAIC_VIEW

class vtkRenderer;

class QVTKWidget;
class FrMainWindow;

#include "FrBaseView.h"

// Mosaic view
class FrMosaicView : public FrBaseView
{ 
public:
    FrMosaicView(FrMainWindow* mainWindow);
    virtual ~FrMosaicView();

    virtual void Initialize();
    virtual void SetupPipeline();
    virtual void UpdatePipeline();
    virtual void SetupRenderers();
    virtual void RemoveRenderers();

private:
    vtkRenderer* m_renderer;
};

#endif


