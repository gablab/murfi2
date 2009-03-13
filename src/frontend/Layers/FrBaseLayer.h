// class combining renderer and actors for single layer display
#ifndef FR_BASE_LAYER
#define FR_BASE_LAYER

//Forward declarations
class vtkRenderer;
class vtkImageData;
class FrCameraSettings;

// Some includes
#include "FrMacro.h"
#include "FrLayerSettings.h"
#include "vtkObject.h"

// Represents layer object.
// Base abstract class.
class FrBaseLayer : public vtkObject {
public:
    typedef enum _LayerTypes { 
        LtBase, LtImage, LtRoi, LtSpecial 
    } LayerTypes;

public:
    vtkTypeMacro(FrBaseLayer,vtkObject);
    static FrBaseLayer* New();

public:
    // Properties

    //FrPropMacro(unsigned long, ID);
    unsigned long GetID() { return m_ID; }
    void SetID(unsigned long l) { 
      m_ID = l; 
      //cout << "FrBaseLayer::SetID(): setting layer ID to " << m_ID << endl; 
    }

    FrPropMacro(RtDataID, DataID);

    vtkRenderer* GetRenderer(){ 
        return m_Renderer; 
    }

    // Input management
    virtual void SetInput(vtkImageData* data){ /* NOTE: nothing to do */ }
    virtual vtkImageData* GetInput(){ return 0L; }

    // Opacity and visibility
    virtual void SetOpacity(double value){ /* NOTE: nothing */ }
    virtual double GetOpacity(){ return 1.0; }

    virtual void SetVisibility(bool value){ /* NOTE: nothing */ }
    virtual bool GetVisibility(){ return true; }

    // Get/Set camera settings
    void SetCameraSettings(FrCameraSettings& settings);
    void GetCameraSettings(FrCameraSettings& settings);

    // Update camera
    void UpdateCamera();

    // Type identifiction.
    // Have to be overridden in all derived layers.
    virtual LayerTypes GetType(){
        return LtBase;
    }
    
protected:
    vtkRenderer* m_Renderer;

private:
    unsigned long m_ID;
    FrBaseLayer(const FrBaseLayer&);  // Not implemented.
    void operator=(const FrBaseLayer&);  // Not implemented.

protected:
    // Constructor and destructor
    FrBaseLayer();
    virtual ~FrBaseLayer();
};

#endif
