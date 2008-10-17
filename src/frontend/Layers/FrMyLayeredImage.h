// class combining renderer and actors for single layer display
#ifndef FR_MYLAYERED_IMG
#define FR_MYLAYERED_IMG

// Forward declarations
class vtkRenderer;

// Some includes
#include "FrMyLayer.h"
#include "FrSettings.h"
#include <vector>


// Represents layer object
class FrMyLayeredImage : public FrMyLayer {
public:
    static FrMyLayeredImage* New();
    
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
    void InitDefault(FrMyLayer* layer);

    // Layer management
    int  AddLayer();
    bool RemoveLayer(int id);
    void RemoveLayers();

    // Returns layer by ID
    FrMyLayer* GetLayerByID(int id);

protected:
    // Constructor and destructor
    FrMyLayeredImage();
    virtual ~FrMyLayeredImage();

private:
   typedef std::vector<FrMyLayer*> LayerCollection;
   LayerCollection m_layers;
   int m_nextLayerId;

private:
    FrMyLayeredImage(const FrMyLayeredImage&);  // Not implemented.
    void operator=(const FrMyLayeredImage&);  // Not implemented.
};

#endif