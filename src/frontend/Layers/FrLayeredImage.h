// class combining renderer and actors for single layer display
#ifndef FR_LAYERED_IMG
#define FR_LAYERED_IMG

// Forward declarations
class vtkRenderer;
class vtkTextMapper;
class vtkActor2D;

// Some includes
#include "FrImageLayer.h"
#include "FrROILayer.h"
#include "FrSpecialLayer.h"
#include "FrSettings.h"
#include <vector>


// Represents layer object
class FrLayeredImage : public FrImageLayer {
public:
    vtkTypeMacro(FrLayeredImage,FrImageLayer);
    static FrLayeredImage* New();

public:
    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    void SetRoiInput(int id, vtkImageData* data);
    vtkImageData* GetRoiInput(int id);

    void SetColormapSettings(FrColormapSettings& settings, int layerId);
    void SetTBCSettings(FrTBCSettings& settings, int layerId);
    void SetCameraSettings(FrCameraSettings& settings, int layerId);
    void SetOpacity(double value, int layerId);
    void SetVisibility(bool value, int layerId);

    // Returns set of renderers
    void GetRenderers(std::vector<vtkRenderer*>& renderers);

    // Update methods
    virtual void UpdateColormap();
    virtual void UpdateTBC();
    virtual void UpdateCamera();

    // Initialization
    void InitImageLayerDefault(FrImageLayer* layer);
    void InitRoiLayerDefault(FrROILayer* layer);

    //
    // Layer management
    //
    int  AddImageLayer();
    bool RemoveImageLayer(int id);
    void RemoveImageLayers();

    int  AddRoiLayer();
    bool RemoveRoiLayer(int id);
    void RemoveRoiLayers();

    void RemoveAllLayers();

    // Returns layer by ID
    FrImageLayer* GetImageLayerByID(int id);
    FrROILayer* GetRoiLayerByID(int id);

    FrSpecialLayer* GetSpecialLayer(){return m_SpecialLayer;};

    //
    // Text management
    //
    void SetText(const char* text);
    void UpdateBorder();

protected:
    // Constructor and destructor
    FrLayeredImage();
    virtual ~FrLayeredImage();

private:
    int m_nextLayerID;
    typedef std::vector<FrImageLayer*> LayerCollection;
    typedef std::vector<FrROILayer*> ROILayerCollection;

    LayerCollection m_ImageLayers;
    ROILayerCollection m_ROILayers;

    FrSpecialLayer* m_SpecialLayer;

private:
    FrLayeredImage(const FrLayeredImage&);  // Not implemented.
    void operator=(const FrLayeredImage&);  // Not implemented.
};

#endif
