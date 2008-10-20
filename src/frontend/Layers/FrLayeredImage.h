// class combining renderer and actors for single layer display
#ifndef FR_LAYERED_IMG
#define FR_LAYERED_IMG

// Forward declarations
class vtkRenderer;

// Some includes
#include "FrLayer.h"
#include "FrSettings.h"
#include <vector>


// Represents layer object
class FrLayeredImage : public FrLayer {
public:
    static FrLayeredImage* New();
    
    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    
    void SetColormapSettings(FrColormapSettings& settings, int layerId);
    void SetTBCSettings(FrTBCSettings& settings, int layerId);
    void SetCameraSettings(FrCameraSettings& settings, int layerId);
    void SetOpacity(double value, int layerId);
    void SetVisibility(bool value, int layerId);

    // Setiurns set of renderers
    // NOTE: index of every renderer in collection equal to
    // render layer number!!!
    void GetRenderers(std::vector<vtkRenderer*>& renderers);

    // Update methods
    virtual void UpdateColormap();
    virtual void UpdateTBC();
    virtual void UpdateCamera();

    // Initialization
    void InitDefault(FrLayer* layer);

    // Layer management
    int  AddLayer();
    bool RemoveLayer(int id);
    void RemoveLayers();

    // Returns layer by ID
    FrLayer* GetLayerByID(int id);

protected:
    // Constructor and destructor
    FrLayeredImage();
    virtual ~FrLayeredImage();

private:
   typedef std::vector<FrLayer*> LayerCollection;
   LayerCollection m_layers;
   int m_nextLayerId;

private:
    FrLayeredImage(const FrLayeredImage&);  // Not implemented.
    void operator=(const FrLayeredImage&);  // Not implemented.
};

#endif