#include "Fr2DSliceActor.h"
#include "FrUtils.h"

// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageMapToColors.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkActor.h"
#include "vtkPointData.h"


Fr2DSliceActor* Fr2DSliceActor::New(){
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("Fr2DSliceActor");
	if(ret){
		return (Fr2DSliceActor*)ret;
	}
	// If the factory was unable to create the object, then create it here.
	return new Fr2DSliceActor;
}

// Construct object with no children.
Fr2DSliceActor::Fr2DSliceActor(){
	this->ImageSlice = NULL;         
	this->ImagePlane = NULL;         
	this->ImageTexture = NULL; 
	this->CurrentImage = NULL;

	// slice outline
	this->ImageOutline = NULL;

	this->Level = -1;              
	this->Opacity = 1.0;            
	this->Interpolation = 0;      
	this->ColorMap = NULL;           
	this->OwnsColorMap = 1;   

	this->ColorMap = vtkLookupTable::New();
	this->ColorMap->SetNumberOfTableValues(256);
	this->ColorMap->SetTableRange(0, 255);

	for (int i = 0; i<=255; i++){
		float v = float(i)/float(255.0);
		this->ColorMap->SetTableValue(i, v, v, v, 1.0);
	}

	this->AutoUpdate = 1;
}

Fr2DSliceActor::~Fr2DSliceActor(){
	if (this->CurrentImage)
		this->CurrentImage->Delete();

	if (this->ImagePlane)
		this->ImagePlane->Delete();

	if (this->ImageSlice)
		this->ImageSlice->Delete();

	if (this->ImageOutline)
		this->ImageOutline->Delete();

	if (this->ImageTexture)
		ImageTexture->Delete();

	if (this->OwnsColorMap){
		if (this->ColorMap)
			this->ColorMap->Delete();
	}
}

void Fr2DSliceActor::SetInput(vtkImageData* image){
	if (image==NULL)
		return;

	int dim[3];
	image->GetDimensions(dim);

	if (this->CurrentImage==NULL)
		this->CurrentImage = vtkImageData::New();

	this->CurrentImage->ShallowCopy(image);

	BuildImageSlice();
	UpdateSlice();
}

void Fr2DSliceActor::SetLookupTable(vtkLookupTable* table){
	if (!table)
		return;

	if (table==this->ColorMap)
		return;

	if (this->ImageTexture)
		this->ImageTexture->SetLookupTable(table);

	if (this->OwnsColorMap){
		this->ColorMap->Delete();
		this->OwnsColorMap = 0;
	}

	this->ColorMap=table;  
	this->Update();
}

void Fr2DSliceActor::SetInterpolation(int on){
	this->Interpolation = on>0;
	if (this->ImageTexture){
		if (this->Interpolation==1)
			this->ImageTexture->InterpolateOn();
		else
			this->ImageTexture->InterpolateOff();
	}
}

void Fr2DSliceActor::SetOpacity(float opacity){
	this->Opacity = Frange(opacity, 0.0, 1.0);
	if (this->ImageSlice)
		this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
}

void Fr2DSliceActor::SetLevel(int level){
	if (this->Level!=level){
		this->Level = level;
		if (this->ImageSlice && this->AutoUpdate==1)
			UpdateSlice();
	}
}

void Fr2DSliceActor::BuildImageSlice()
{
	if (!this->ImageSlice){
		this->ImagePlane = vtkPlaneSource::New();
		this->ImagePlane->SetXResolution(1);
		this->ImagePlane->SetYResolution(1);
        
		// ImageSlice
		this->ImageSlice = vtkActor::New();
		this->ImageSlice->PickableOff();
		this->ImageTexture = vtkTexture::New();
        		
		int dims[3];  
		this->CurrentImage->GetDimensions(dims);

		this->ImageTexture->SetInput(this->CurrentImage);
		if (this->Interpolation==0)
			this->ImageTexture->InterpolateOn();
		else
			this->ImageTexture->InterpolateOff();
		this->ImageTexture->SetLookupTable(this->ColorMap); 
		this->ImageTexture->MapColorScalarsThroughLookupTableOn();
		this->ImageTexture->RepeatOff();

		vtkPolyDataMapper* imageMapper = vtkPolyDataMapper::New();
		imageMapper->SetInput(this->ImagePlane->GetOutput());

		this->ImageSlice->SetMapper(imageMapper);
		this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
		this->ImageSlice->GetProperty()->SetInterpolationToFlat();
		this->ImageSlice->SetTexture(this->ImageTexture);
        this->ImageSlice->SetPickable(this->Pickable);

		// build image outline
		vtkOutlineFilter* outlineFilter = vtkOutlineFilter::New();
		vtkPolyDataMapper* mapOutline = vtkPolyDataMapper::New();
		this->ImageOutline = vtkFollower::New();
        this->ImageOutline->PickableOff();
            
		outlineFilter->SetInput(this->CurrentImage);
		outlineFilter->Update();

		mapOutline->SetInput(outlineFilter->GetOutput());
		mapOutline->Update();

		this->ImageOutline->SetMapper(mapOutline);
		this->ImageOutline->GetProperty()->SetColor(0.0,1.0,0.0);
		this->ImageOutline->GetProperty()->SetAmbient(1.0);
		this->ImageOutline->GetProperty()->SetDiffuse(0.0);
		this->ImageOutline->GetProperty()->SetSpecular(0.0);
		this->ImageOutline->GetProperty()->SetRepresentationToWireframe();
		this->ImageOutline->PickableOff();

		AddPart(this->ImageSlice);
		AddPart(this->ImageOutline);

        // clear all the stuff		
        mapOutline->Delete();
        outlineFilter->Delete();
        imageMapper->Delete();
    }
}

void Fr2DSliceActor::UpdateTexture(){
	if (!this->CurrentImage || !this->ImageSlice)
		return;

	this->ImageSlice->GetTexture()->Modified();
	this->ImageSlice->Modified();
}

void Fr2DSliceActor::UpdateSlice(){
	if (!this->CurrentImage)
		return;

	vtkImageData* img = this->CurrentImage;
	int dim[3];  
	img->GetDimensions(dim);
	double sp[3];   
	img->GetSpacing(sp);
	double ori[3];  
	img->GetOrigin(ori);

	if (this->Level==-1)
		this->Level = dim[2]/2;		// only XY plane

	this->Level = Irange(this->Level, 0, dim[2]-1);		// only XY plane

	double d1[3];
	double d0[3];
	for (int i = 0; i<=2; i++){
		if (i!=2){		// only XY plane
			d1[i] = sp[i]*double(dim[i]-0.5)+ori[i];
			d0[i] = sp[i]*double(-0.5)+ori[i];
		}
		else{
			d1[i] = sp[i]*double(this->Level)+ori[i];
			d0[i] = d1[i];
		}
	}

	this->ImagePlane->SetOrigin(d0[0] , d0[1] , d1[2]);
	this->ImagePlane->SetPoint1(d1[0] , d0[1] , d1[2]);
	this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);

	this->ImagePlane->Update();
}

