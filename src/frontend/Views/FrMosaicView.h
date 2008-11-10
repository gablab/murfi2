#ifndef FR_MOSAIC_VIEW
#define FR_MOSAIC_VIEW

class vtkRenderer;
class vtkImageActor;

class QVTKWidget;
class FrMainWindow;
class FrDocumentReader;
class FrLayeredImage;
class FrUpdateParams1;

#include "FrBaseView.h"


// Mosaic view
class FrMosaicView : public FrBaseView { 
public:
    FrMosaicView(FrMainWindow* mainWindow);
    virtual ~FrMosaicView();

    virtual void Initialize();
    virtual void UpdatePipeline(int point);
    virtual void SetupRenderers();
    virtual void RemoveRenderers();

    // Accessors
    FrLayeredImage* GetImage(){ return m_LayeredImage; }

private:
    // Update pipline helpers
    void InitUpdateParams(FrUpdateParams1& params);
    void ReadDocument(FrUpdateParams1& params);
    void UpdateColormap(FrUpdateParams1& params);
    void UpdateTBC(FrUpdateParams1& params);
    void UpdateOpacityVisibility(FrUpdateParams1& params);

private:
    // Pipline 
    FrDocumentReader* m_docReader;
	FrLayeredImage* m_LayeredImage;
};

#endif


