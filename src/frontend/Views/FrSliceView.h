#ifndef FR_SLICE_VIEW
#define FR_SLICE_VIEW

class QWidget;
class vtkInteractorStyle;
class vtkRenderer;
class vtkImageActor;

class QVTKWidget;
class FrMainWindow;
class Fr2DSliceActor;
class FrDocumentReader;
class FrSliceExtractor;
class FrLayeredImage;

#include "FrBaseView.h"
#include "FrTabSettingsDocObj.h"

// Wrapper for QVTKWidget.
class FrSliceView : public FrBaseView { 
public:
    FrSliceView(FrMainWindow* mainWindow);
    virtual ~FrSliceView();

    virtual void Initialize();
    virtual void UpdatePipeline(int point);
    virtual void SetupRenderers();
    virtual void RemoveRenderers();
    
    // Accessors
    FrLayeredImage* GetImage(){ return m_LayeredImage; }
    
private:
    // Pipeline
    FrDocumentReader* m_docReader;
    FrSliceExtractor* m_SliceExtractor;
	FrLayeredImage* m_LayeredImage;

};

#endif


