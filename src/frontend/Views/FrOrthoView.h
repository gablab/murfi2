#ifndef FR_ORTHO_VIEW
#define FR_ORTHO_VIEW

// Forward declarations
class vtkRenderer;

class FrMainWindow;
class FrDocumentReader;
class FrSliceExtractor;
class FrMyLayeredImage;
class FrLayerSettings;
class FrOrthoViewSettings;

// Some includes
#include "FrBaseView.h"
#include <vector>

#define ORTHO_IMAGE_COUNT   3
#define CORONAL_IMAGE 0
#define SAGITAL_IMAGE 1
#define AXIAL_IMAGE   2

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
    FrMyLayeredImage* GetImage(int idx){ return m_LayeredImage[idx]; }

private:
    FrLayerSettings* GetLayerAndInitLayers(std::vector<FrLayerSettings*>& layers, 
                                           FrOrthoViewSettings* viewSets, 
                                           int rendererID);

private:
    // Render pipline
    FrDocumentReader* m_docReader;
    FrSliceExtractor* m_SliceExtractor[ORTHO_IMAGE_COUNT];
    FrMyLayeredImage* m_LayeredImage[ORTHO_IMAGE_COUNT];

    vtkRenderer* m_dummyRenderer;
};

#endif


