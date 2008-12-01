// class combining renderer and actors for single layer display
#ifndef FR_CM_LAYER
#define FR_CM_LAYER

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
class FrColormapLayer : public FrBaseLayer {
public:
    vtkTypeMacro(FrColormapLayer,FrBaseLayer);
    static FrColormapLayer* New();

    virtual LayerTypes GetType(){
        return FrBaseLayer::LtColormap;
    }

public:
    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    vtkImageData* GetInput();

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

protected:
    // Pipline stuff
    FrColormapFilter* m_cmFilter;
    FrTbcFilter* m_tbcFilter;
    vtkImageActor* m_actor;

private:
    FrColormapLayer(const FrColormapLayer&);  // Not implemented.
    void operator=(const FrColormapLayer&);  // Not implemented.

protected:
    // Constructor and destructor
    FrColormapLayer();
    virtual ~FrColormapLayer();
};

#endif
