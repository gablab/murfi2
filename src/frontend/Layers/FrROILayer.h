// class combining renderer and actors for single layer display
#ifndef FR_ROI_LAYER
#define FR_ROI_LAYER

//Forward declarations
class FrCameraSettings;
class FrSelection;
class vtkImageData;
class vtkImageActor;
class vtkRenderer;

// Some includes
#include "FrBaseLayer.h"
#include "FrMacro.h"

// Represents layer object
class FrROILayer : public FrBaseLayer {
public:
    vtkTypeMacro(FrROILayer, FrBaseLayer);
    static FrROILayer* New();

public:
    // Properties
    FrPropMacro(int, ID);

    // Accessors/Modifiers
    virtual void SetInput(vtkImageData* data);
    vtkImageData* GetInput();

    void SetOpacity(double value);
    double GetOpacity();

    void SetVisibility(bool value);
    bool GetVisibility();
    
    vtkImageActor* GetActor(){ return m_actor; }

    // Update methods
    virtual void UpdateCamera();

	// Selection methods
	void SetSelection();

protected:
    // Pipeline stuff
    vtkImageActor* m_actor;
	FrSelection* m_selection;

private:
    FrROILayer(const FrROILayer&);  // Not implemented.
    void operator=(const FrROILayer&);  // Not implemented.

protected:
    // Constructor and destructor
    FrROILayer();
    virtual ~FrROILayer();

};

#endif
