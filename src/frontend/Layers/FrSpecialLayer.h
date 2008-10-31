#ifndef FR_SPECIAL_LAYER
#define FR_SPECIAL_LAYER

// Forward declarations
class vtkTextMapper;
class vtkPoints;
class vtkPolyDataMapper2D;
class vtkActor2D;

// Includes
#include "FrBaseLayer.h"

class FrSpecialLayer : public FrBaseLayer {
public:
    vtkTypeMacro(FrSpecialLayer,FrBaseLayer);
    static FrSpecialLayer *New();

public:
    void SetText(const char* text);
    void UpdateBorder(int winWidth, int winHeight);

private:
	// Text support
	vtkTextMapper*  m_TextMapper;
	vtkActor2D*     m_TextActor;
    // Border support
    vtkPoints*           m_BorderPts;
    vtkPolyDataMapper2D* m_BorderMapper;
    vtkActor2D*          m_BorderActor;

private:
    FrSpecialLayer(const FrSpecialLayer&); // Not implemented
    void operator=(const FrSpecialLayer&); // Not implemented

    // Helpers
    void InitializeText();
    void InitializeBorder();

protected:
    FrSpecialLayer();
    ~FrSpecialLayer();
};

#endif
