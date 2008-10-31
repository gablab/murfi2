// class combining renderer and actors for single layer display
#ifndef FR_IMG_LAYER
#define FR_IMG_LAYER

//Forward declarations
class FrColormapSettings;
class FrTBCSettings;
class FrCameraSettings;
class FrColormapFilter;
class FrTBCFilter;
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

public:
    // Properties
    FrPropMacro(int, ID);

    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    vtkImageData* GetInput();

    void SetColormapSettings(FrColormapSettings& settings);
    void GetColormapSettings(FrColormapSettings& settings);

    void SetTBCSettings(FrTBCSettings& settings);
    void GetTBCSettings(FrTBCSettings& settings);

    void SetOpacity(double value);
    double GetOpacity();

    void SetVisibility(bool value);
    bool GetVisibility();
    
    vtkImageActor* GetActor(){ return m_actor; }

    // Update methods
    virtual void UpdateColormap();
    virtual void UpdateTBC();
    virtual void UpdateCamera();

protected:
    // Pipline stuff
    FrColormapFilter* m_cmFilter;
    FrTBCFilter* m_tbcFilter;
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
