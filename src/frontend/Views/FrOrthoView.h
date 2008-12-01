#ifndef FR_ORTHO_VIEW
#define FR_ORTHO_VIEW

// Forward declarations
class vtkRenderer;

class FrMainWindow;
class FrDocumentReader;
class FrSliceExtractor;
class FrLayeredImage;
class FrLayerSettings;
class FrOrthoViewSettings;
class FrUpdateParams2;

// Some includes
#include "FrBaseView.h"
#include "FrViewSettings.h"

// Class represents ortho view
class FrOrthoView : public FrBaseView { 
public:
    FrOrthoView(FrMainWindow* mainWindow);
    virtual ~FrOrthoView();

    virtual void Initialize();
    virtual void UpdatePipeline(int point);
    virtual void SetupRenderers();
    virtual void RemoveRenderers();

    // Accessors
    FrLayeredImage* GetImage(int idx){ 
        return m_LayeredImage[idx]; 
    }

private:

    // Update pipline helpers
    bool InitUpdateParams(FrUpdateParams2& params);
    void ReadDocument(FrUpdateParams2& params);
    void UpdateColormap(FrUpdateParams2& params);
    void UpdateTbc(FrUpdateParams2& params);

private:
    // Render pipline
    FrDocumentReader* m_docReader;
    FrLayeredImage* m_LayeredImage[ORTHO_VIEWS_CNT];

    vtkRenderer* m_dummyRenderer;
};

#endif


