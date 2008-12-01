// class combining renderer and actors for single layer display
#ifndef FR_ROI_LAYER
#define FR_ROI_LAYER

//Forward declarations
class FrMaskToRgbaFilter;
class FrCameraSettings;
class vtkImageData;
class vtkImageActor;
class vtkRenderer;

// Some includes
#include "FrBaseLayer.h"
#include "FrMacro.h"

// Represents layer object
class FrRoiLayer : public FrBaseLayer {
public:
    vtkTypeMacro(FrRoiLayer, FrBaseLayer);
    static FrRoiLayer* New();

    virtual LayerTypes GetType(){
        return FrBaseLayer::LtRoi;
    }

public:
    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    virtual vtkImageData* GetInput();

    virtual void SetOpacity(double value);
    virtual double GetOpacity();

    virtual void SetVisibility(bool value);
    virtual bool GetVisibility();

    vtkImageActor* GetActor(){ 
        return m_actor; 
    }

    // Update methods
    void UpdateData();

protected:
    // Pipeline stuff
    FrMaskToRgbaFilter* m_filter;
    vtkImageActor* m_actor;

private:
    FrRoiLayer(const FrRoiLayer&);  // Not implemented.
    void operator=(const FrRoiLayer&);  // Not implemented.

protected:
    // Constructor and destructor
    FrRoiLayer();
    virtual ~FrRoiLayer();

};

#endif
