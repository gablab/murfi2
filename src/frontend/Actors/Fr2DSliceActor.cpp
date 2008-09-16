#include "Fr2DSliceActor.h"
#include "FrUtils.h"

// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
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
	this->ImageVOI = NULL;

	this->Level = -1;              
	this->Opacity = 1.0;            
	this->Interpolation = 0;      
	this->ColorMap = NULL;           
	this->OwnsColorMap = 1;   

	this->CurrentPlane = 2;		// 2
	this->CurrentFrame = 0;

	this->ColorMap = vtkLookupTable::New();
	this->ColorMap->SetNumberOfTableValues(256);
	this->ColorMap->SetTableRange(0, 255);

	for (int i = 0; i<=255; i++){
		float v = float(i)/float(255.0);
		this->ColorMap->SetTableValue(i, v, v, v, 1.0);
	}

	//bwLut = vtkLookupTable::New();
	//bwLut->SetTableRange (0, 2000);
	//bwLut->SetSaturationRange (0, 0);
	//bwLut->SetHueRange (0, 0);
	//bwLut->SetValueRange (0, 1);

	this->AutoUpdate = 1;
}

Fr2DSliceActor::~Fr2DSliceActor(){
	if (this->CurrentImage)
		this->CurrentImage->Delete();

	if (this->ImagePlane)
		this->ImagePlane->Delete();

	if (this->ImageSlice)
		this->ImageSlice->Delete();

	if (this->ImageTexture)
		ImageTexture->Delete();

	if (this->OwnsColorMap){
		if (this->ColorMap)
			this->ColorMap->Delete();

	if (this->ImageVOI)
		this->ImageVOI->Delete();
	
	}
}

void Fr2DSliceActor::SetInput(vtkImageData* image){
	if (image==NULL)
		return;

	int dim[3];
	image->GetDimensions(dim);

	//if (dim[0]!=1 && dim[1]!=1 && dim[2]!=1){
	//	return;
	//}

	if (this->CurrentImage==NULL)
		this->CurrentImage = vtkImageData::New();

	this->CurrentImage->ShallowCopy(image);

	BuildImageSlice();
//	if (this->OwnsColorMap)
//		AutoUpdateColormapRange(this->ColorMap, this->CurrentImage);
//	SetStepColorMap(this->ColorMap, 0, 255, 0, 255, 256, 0);

	// Create New Image Voi and Texture 
//	this->ImageVOI->SetInput(this->CurrentImage);
//	this->ImageVOI->Modified();

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

void Fr2DSliceActor::SetCurrentFrame(int currentframe){
	if (currentframe!=this->CurrentFrame){
		this->CurrentFrame = currentframe;
	//	if (this->ImageSlice && this->AutoUpdate==1)
	//		UpdateSlice();
	}
}

void Fr2DSliceActor::SetCurrentPlane(int currentplane){
	if (currentplane!=this->CurrentPlane){
		this->CurrentPlane = currentplane;
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

		//this->ImageVOI = vtkExtractVOI::New();
		//this->ImageVOI->SetInput(this->CurrentImage);
		//this->ImageVOI->SetVOI(0, dims[0]-1, 0, dims[1]-1, this->CurrentFrame, this->CurrentFrame);
		//this->ImageVOI->Update();

		//this->ImageTexture->SetInput(this->ImageVOI->GetOutput());
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

		AddPart(this->ImageSlice);
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

	this->CurrentPlane = Irange(this->CurrentPlane, 0,2);

	if (this->Level==-1)
		this->Level = dim[this->CurrentPlane]/2;

	this->Level = Irange(this->Level, 0, dim[this->CurrentPlane]-1);

	double d1[3];
	double d0[3];
	for (int i = 0; i<=2; i++){
		if (i!=this->CurrentPlane){
			d1[i] = sp[i]*double(dim[i]-0.5)+ori[i];
			d0[i] = sp[i]*double(-0.5)+ori[i];
		}
		else{
			d1[i] = sp[i]*double(this->Level)+ori[i];
			d0[i] = d1[i];
		}
	}

	switch (this->CurrentPlane){
		case 0: // yz
			this->ImagePlane->SetOrigin(d1[0] , d0[1] , d0[2]);
			this->ImagePlane->SetPoint1(d1[0] , d1[1] , d0[2]);
			this->ImagePlane->SetPoint2(d1[0] , d0[1] , d1[2]);
		//	this->ImageVOI->SetVOI(this->CurrentFrame, this->CurrentFrame, 0, dim[1]-1, 0, dim[2]-1);
		break;

		case 1: // xz
			this->ImagePlane->SetOrigin(d0[0] , d1[1] , d0[2]);
			this->ImagePlane->SetPoint1(d1[0] , d1[1] , d0[2]);
			this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);
		//	this->ImageVOI->SetVOI(0, dim[0]-1, this->CurrentFrame, this->CurrentFrame, 0, dim[2]-1);
		break;

		case 2: // xy
			this->ImagePlane->SetOrigin(d0[0] , d0[1] , d1[2]);
			this->ImagePlane->SetPoint1(d1[0] , d0[1] , d1[2]);
			this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);
		//	this->ImageVOI->SetVOI(0, dim[0]-1, 0, dim[1]-1, this->CurrentFrame, this->CurrentFrame);
		break;
	}

	//this->ImageVOI->Modified();
	this->ImagePlane->Update();
}

//void Fr2DSliceActor::AutoUpdateColormapRange(vtkLookupTable* cmap, vtkImageData* img){
//	if (img==NULL)
//		return;
//
//	double range[2];
//	img->GetPointData()->GetScalars()->GetRange(range);
//
//	SetStepColorMap(cmap, range[0], range[1], range[0], range[1], 256);
//}
//
//void Fr2DSliceActor::SetStepColorMap(vtkLookupTable *cmap, float min, float max, float min_value, float max_value,
//				      int num_colors, int vol){
//	double gap = fabs(min_value-max_value)*0.1;
//	if (gap<1e-7){
//		max=min+1e-7;
//		gap=(min_value-max_value)*0.1;
//	}
//
//	min = Frange(min,min_value,max_value-gap);
//	max = Frange(max,min_value+gap,max_value);
//
//	if (max_value<(min_value+gap))
//		max_value+=gap;
//
//	float range = max_value-min_value;
//
//	if (num_colors!=cmap->GetNumberOfColors() && num_colors>0)
//		cmap->SetNumberOfTableValues(num_colors);
//
//	cmap->SetTableRange(min_value,max_value);
//
//	float numc = float(num_colors-1);
//
//	int imin = int(0.5+numc*float(min-min_value)/range);
//	int imax = int(0.5+numc*float(max-min_value)/range);
//
//	for (int i = 0;i<num_colors;i++){
//		float v = 0.0;
//		if (i<=imin)
//			v = 0.0;
//		else if (i>=imax)
//			v = 1.0;
//		else
//			v = float(i-imin)/float(imax-imin);
//
//		if (vol)
//			cmap->SetTableValue(i,v,v,v,v*0.5);
//		else if (v>0.0)
//			cmap->SetTableValue(i,v,v,v,1.0);
//		else
//			cmap->SetTableValue(i,v,v,v,0.5);
//	}
//}

int Fr2DSliceActor::GetMaxSliceNumber(){
    int result = -1;
    if(this->CurrentImage){
        int dims[3];
        this->CurrentImage->GetDimensions(dims);

        result = dims[2]-1;
    }
    return result;
}