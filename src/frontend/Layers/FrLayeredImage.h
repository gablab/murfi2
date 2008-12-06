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

#include "vtkObject.h"


// Represents multi layer object.
// It keeps and manages all layers.
class FrLayeredImage : public vtkObject {
public:
    typedef enum _LType { Roi, Image, Colormap } LayerType;

public:
    vtkTypeMacro(FrLayeredImage, FrImageLayer);
    static FrLayeredImage* New();

public:
    // Accessors/Modifiers
    void SetImageInput(vtkImageData* data);
    vtkImageData* GetImageInput();

    void SetRoiInput(vtkImageData* data, unsigned int id);
    vtkImageData* GetRoiInput(unsigned int id);

    // Setup different settings
    void SetTbcSettings(FrTbcSettings& settings, unsigned int id);
    void SetCameraSettings(FrCameraSettings& settings, unsigned int id);
    void SetColormapSettings(FrColormapSettings& settings, unsigned int id);
    void SetOpacity(double value, unsigned int id);
    void SetVisibility(bool value, unsigned int id);
    

    // Update methods
    void UpdateTbc();
    void UpdateCamera();
    void UpdateColormap();
    
    //
    // Layer management
    //
    bool AddLayer(unsigned int id, LayerType type);
    bool RemoveLayer(unsigned int id);
    void RemoveLayers();

    // Returns layer by ID
    FrBaseLayer* GetLayerByID(unsigned int id);
    FrSpecialLayer* GetSpecialLayer(){
        return m_SpecialLayer;
    };

    // Returns set of renderers (sorted in predefined order)
    void GetRenderers(std::vector<vtkRenderer*>& renderers);
    bool IsInViewport(int mouseX, int mouseY);
    
    // get bounds of actor
    double* GetActorBounds();

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
    typedef std::vector<FrBaseLayer*> LayersCollection;
    LayersCollection m_Layers;
    
    FrImageLayer*   m_ImageLayer;
    FrSpecialLayer* m_SpecialLayer;

private:
    FrLayeredImage(const FrLayeredImage&);  // Not implemented.
    void operator=(const FrLayeredImage&);  // Not implemented.
};

#endif
