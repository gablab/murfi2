// class combining renderer and actors for single layer display
#ifndef FR_BASE_LAYER
#define FR_BASE_LAYER

//Forward declarations
class vtkRenderer;
class FrCameraSettings;

// Some includes
#include "vtkObject.h"
#include "FrMacro.h"

// Represents layer object
class FrBaseLayer : public vtkObject {
public:
    vtkTypeMacro(FrBaseLayer,vtkObject);
    static FrBaseLayer* New();

public:
    // Properties
    FrPropMacro(int, ID);

    void SetCameraSettings(FrCameraSettings& settings);
    void GetCameraSettings(FrCameraSettings& settings);

    // Accessors/Modifiers
    vtkRenderer* GetRenderer(){ return m_Renderer; }
    
protected:
    vtkRenderer* m_Renderer;

private:
    FrBaseLayer(const FrBaseLayer&);  // Not implemented.
    void operator=(const FrBaseLayer&);  // Not implemented.

protected:
    // Constructor and destructor
    FrBaseLayer();
    virtual ~FrBaseLayer();
};

#endif
