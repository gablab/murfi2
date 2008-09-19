// .NAME vtkpxImageSlice - create hierarchies of actors
// .SECTION Description
#ifndef FR_2DSLICE_ACTOR
#define FR_2DSLICE_ACTOR

#include "FrBaseActor.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include "vtkPlaneSource.h"
#include "vtkTexture.h"
#include "vtkProperty.h"
#include "vtkExtractVOI.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkFollower.h"


class Fr2DSliceActor : public FrBaseActor
{
public:
	static Fr2DSliceActor *New();
	vtkTypeMacro(Fr2DSliceActor, FrBaseActor);

	// Description:
	// Add a part to the list of parts.
	virtual void SetInput(vtkImageData* image);
	virtual void SetLookupTable(vtkLookupTable* table);
	virtual void SetInterpolation(int on);
	virtual void SetOpacity(float opacity);

	// Description:
	// Set Key Location Parameters 
	virtual void SetLevel(int level);

	vtkGetMacro(Level,int);

	vtkGetMacro(Opacity,float);
	vtkGetMacro(Interpolation,int);

	vtkGetMacro(OwnsColorMap,int);

	vtkSetClampMacro(AutoUpdate,int,0,1);
	vtkGetMacro(AutoUpdate,int);
	vtkBooleanMacro(AutoUpdate,int);


	virtual void Update() { this->UpdateSlice();}
	virtual void UpdateTexture();

	vtkGetObjectMacro(CurrentImage,vtkImageData);

protected:
	vtkActor*                    ImageSlice;
	vtkPlaneSource*              ImagePlane;
	vtkImageData*                CurrentImage;
	vtkTexture*                  ImageTexture;
	vtkFollower*				 ImageOutline;

	int                          Level;
	float                        Opacity;
	int                          Interpolation;
	int                          AutoUpdate;

	vtkLookupTable*              ColorMap;
	int                          OwnsColorMap;   

	Fr2DSliceActor();
	virtual ~Fr2DSliceActor();
	Fr2DSliceActor(const Fr2DSliceActor&) {};
	void operator=(const Fr2DSliceActor&) {};

	// Slice Stuff
	virtual void BuildImageSlice();
	virtual void UpdateSlice();

};


#endif





