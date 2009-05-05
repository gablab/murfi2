// class combining renderer and actors for single layer display
#ifndef FR_LAYERED_IMG
#define FR_LAYERED_IMG

// Forward declarations
class vtkRenderer;
class vtkTextMapper;
class vtkActor2D;
class vtkImageActor;

// Some includes
#include "FrImageLayer.h"
#include "FrROILayer.h"
#include "FrSpecialLayer.h"
#include "FrSettings.h"
#include <vector>
#include <limits>

#include "vtkObject.h"


// Represents multi layer object.
// It keeps and manages all layers.
class FrLayeredImage : public vtkObject {
public:
    typedef enum _LType { Roi, Image } LayerType;

public:
    vtkTypeMacro(FrLayeredImage, FrImageLayer);
    static FrLayeredImage* New();

public:
    // Accessors/Modifiers
    void SetImageInput(vtkImageData* data, unsigned long id);
    vtkImageData* GetImageInput(unsigned long id = std::numeric_limits<unsigned long>::max());

    void SetRoiInput(vtkImageData* data, unsigned long id);
    vtkImageData* GetRoiInput(unsigned long id);

    // Setup different settings
    void SetTbcSettings(FrTbcSettings& settings, unsigned long id);
    void SetCameraSettings(FrCameraSettings& settings, unsigned long id);
    void SetColormapSettings(FrColormapSettings& settings, unsigned long id);
    void SetOpacity(double value, unsigned long id);
    void SetVisibility(bool value, unsigned long id);
    

    // Update methods
    void UpdateTbc();
    void UpdateCamera();
    void UpdateColormap();
    
    //
    // Layer management
    //
    bool AddLayer(unsigned long id, LayerType type);
    bool RemoveLayer(unsigned long id);
    void RemoveLayers();
    //void RemoveColormapLayers();
    bool ChangeLayerPosition(unsigned long id, int inc);

    // Returns layer by ID
    FrBaseLayer* GetLayerByID(unsigned long id);
    FrSpecialLayer* GetSpecialLayer(){
        return m_SpecialLayer;
    };

    // the next layer to be focused after the one passed
    unsigned long GetNextLayerID(unsigned long id);

    // Returns set of renderers (sorted in predefined order)
    void GetRenderers(std::vector<vtkRenderer*>& renderers);
    bool IsInViewport(int mouseX, int mouseY);
    
    // get actor properties
    double* GetActorBounds();
    double* GetActorCenter();
    vtkImageActor* GetActor();

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
    
    //FrImageLayer*   m_ImageLayer;
    FrSpecialLayer* m_SpecialLayer;

private:
    FrLayeredImage(const FrLayeredImage&);  // Not implemented.
    void operator=(const FrLayeredImage&);  // Not implemented.
};

#endif
