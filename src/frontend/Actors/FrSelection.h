#ifndef FR_SELECTION
#define FR_SELECTION

#include "FrBaseActor.h"
#include "FrMacro.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include "vtkPlaneSource.h"
#include "vtkTexture.h"
#include "vtkProperty.h"
#include "vtkExtractVOI.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor2d.h"

#include <vector>

typedef struct _pos{
    int x, y, z;
}Pos;

typedef struct _selectionParams{
	int type;	                    // selection type (sphere, rectangle, free shape polygon)
    std::vector<Pos> points;        // for free shape polygon
    int radius;                     // for sphere
    int center;                     // for sphere
    Pos firstPoint;              // for rectangle
    Pos secondPoint;              // for rectangle
}SelectionParams;


class FrSelection : public FrBaseActor
{
public:
 //   enum Type { Unknown, Rectangle, Circle, Polygon };	// may be Points?

public:
	static FrSelection *New();
	vtkTypeMacro(FrSelection, FrBaseActor);

    // Properties
    //FrSetPropMacro(Type, CurrentType);
    void SetSelectionParams(SelectionParams params);
    FrGetPropMacro(SelectionParams, params);

protected:
	FrSelection();
	virtual ~FrSelection();

private:
    //VTK style
	FrSelection(const FrSelection&) {};
	void operator=(const FrSelection&) {};

private:
    void DrawRectangle();
    void DrawCircle();
    void DrawPolygon();
    void HideAll();

private:
	vtkActor* m_actor;
    vtkPolyDataMapper* m_mapper;
};

#endif





