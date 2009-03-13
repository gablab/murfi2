// class combining renderer and actors for single layer display
#ifndef FR_IMG_LAYER
#define FR_IMG_LAYER

//Forward declarations
class FrColormapSettings;
class FrTbcSettings;
class FrCameraSettings;
class FrColormapFilter;
class FrTbcFilter;
class vtkImageData;
class vtkImageActor;
class vtkRenderer;

// Some includes
#include "FrBaseLayer.h"
#include "FrMacro.h"

// Represents layer object
class FrImageLayer : public FrBaseLayer {
public:
    vtkTypeMacro(FrImageLayer,FrBaseLayer);
    static FrImageLayer* New();

    virtual LayerTypes GetType(){
        return FrBaseLayer::LtImage;
    }

public:
    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    virtual vtkImageData* GetInput();

    void SetColormapSettings(FrColormapSettings& settings);
    void GetColormapSettings(FrColormapSettings& settings);

    void SetTbcSettings(FrTbcSettings& settings);
    void GetTbcSettings(FrTbcSettings& settings);

    virtual void SetOpacity(double value);
    virtual double GetOpacity();

    virtual void SetVisibility(bool value);
    virtual bool GetVisibility();
    
    vtkImageActor* GetActor(){ 
        return m_actor; 
    }

    // Update methods
    virtual void UpdateColormap();
    virtual void UpdateTbc();
    virtual void UpdateCamera();

protected:
    // Pipline stuff
    FrColormapFilter* m_cmFilter;
    FrTbcFilter* m_tbcFilter;
    vtkImageActor* m_actor;

private:
    FrImageLayer(const FrImageLayer&);  // Not implemented.
    void operator=(const FrImageLayer&);  // Not implemented.

protected:
    // Constructor and destructor
    FrImageLayer();
    virtual ~FrImageLayer();

};

#endif
