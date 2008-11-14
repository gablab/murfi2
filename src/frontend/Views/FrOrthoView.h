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
    FrLayeredImage* GetImage(int idx){ return m_LayeredImage[idx]; }

private:

    // Update pipline helpers
    void InitUpdateParams(FrUpdateParams2& params);
    void ReadImage(FrUpdateParams2& params);
    void ReadRoi(FrUpdateParams2& params);
    void ExtractSlice(FrUpdateParams2& params);
    void UpdateColormap(FrUpdateParams2& params);
    void UpdateTBC(FrUpdateParams2& params);
    void UpdateOpacityVisibility(FrUpdateParams2& params);

private:
    // Render pipline
    FrDocumentReader* m_imgReader;
    FrDocumentReader* m_roiReader;
    FrSliceExtractor* m_SliceExtractor[ORTHO_IMAGE_COUNT];
    FrLayeredImage* m_LayeredImage[ORTHO_IMAGE_COUNT];

    vtkRenderer* m_dummyRenderer;
};

#endif


