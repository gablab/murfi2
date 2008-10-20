// class combining renderer and actors for single layer display
#ifndef FR_LAYER
#define FR_LAYER

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
#include "vtkObject.h"
#include "FrMacro.h"

// Represents layer object
class FrLayer : public vtkObject {
public:
    static FrLayer* New();

    // Properties
    FrPropMacro(int, ID);

    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    vtkImageData* GetInput();

    void SetColormapSettings(FrColormapSettings& settings);
    void GetColormapSettings(FrColormapSettings& settings);

    void SetTBCSettings(FrTBCSettings& settings);
    void GetTBCSettings(FrTBCSettings& settings);

    void SetCameraSettings(FrCameraSettings& settings);
    void GetCameraSettings(FrCameraSettings& settings);

    void SetOpacity(double value);
    double GetOpacity();

    void SetVisibility(bool value);
    bool GetVisibility();

    vtkRenderer* GetRenderer(){ return m_renderer; }
    vtkImageActor* GetActor(){ return m_actor; }

    // Update methods
    virtual void UpdateColormap();
    virtual void UpdateTBC();
    virtual void UpdateCamera();

protected:
    // Constructor and destructor
    FrLayer();
    virtual ~FrLayer();

protected:
    // Pipline stuff
    FrColormapFilter* m_cmFilter;
    FrTBCFilter* m_tbcFilter;
    vtkImageActor* m_actor;
    vtkRenderer* m_renderer;

private:
    FrLayer(const FrLayer&);  // Not implemented.
    void operator=(const FrLayer&);  // Not implemented.
};

#endif