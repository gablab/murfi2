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
class FrUpdateParams0;

#include "FrBaseView.h"
#include "FrTabSettingsDocObj.h"

// Wrapper for QVTKWidget.
class FrSliceView : public FrBaseView { 
public:
    FrSliceView(FrMainWindow* mainWindow);
    virtual ~FrSliceView();

    // Overrides
    virtual void Initialize();
    virtual void UpdatePipeline(int point);
    virtual void SetupRenderers();
    virtual void RemoveRenderers();
        
    // Accessors
    FrLayeredImage* GetImage(){ return m_LayeredImage; }

private:
    // Update pipline helpers
    void InitUpdateParams(FrUpdateParams0& params);
    void ReadDocument(FrUpdateParams0& params);
    void ExtractSlice(FrUpdateParams0& params);
    void UpdateColormap(FrUpdateParams0& params);
    void UpdateTBC(FrUpdateParams0& params);
    void UpdateOpacityVisibility(FrUpdateParams0& params);

private:
    // Pipeline
    FrDocumentReader* m_docReader;
    FrSliceExtractor* m_SliceExtractor;
	FrLayeredImage* m_LayeredImage;
};

#endif


