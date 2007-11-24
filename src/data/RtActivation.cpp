/******************************************************************************
 * RtActivation.cpp defines a class for an activation image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#include"RtActivation.h"
#include<cmath>

//*** constructors/destructors  ***//

// default constructor
RtActivation::RtActivation() : RtDataImage<double>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage()")); 
  addToID("activation");
  magicNumber = MAGIC_NUMBER;
  threshold = 10.0;
  ceiling = 16.0;
}

// constructor with MRIInfo
RtActivation::RtActivation(RtMRIImage &img) 
  : RtDataImage<double>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage()")); 
  addToID("activation");
  magicNumber = MAGIC_NUMBER;

  setInfo(img);

  threshold = 10.0;
  ceiling = 16.0;
}

// destructor
RtActivation::~RtActivation() {
}

// print info about this image
void RtActivation::printInfo(ostream &os) {
  ACE_TRACE(("RtActivation::printInfo"));
  
  int wid = 30;
  
  RtDataImage<double>::printInfo(os);
  
  os << setw(wid) << "threshold: " << threshold << endl;
  os << setw(wid) << "ceiling: "   << ceiling << endl;
}

//********  methods for getting data from the image *******//

// get the threshold
double RtActivation::getThreshold() const {
  return threshold;
}

// get the scale inverted flag
bool RtActivation::getScaleIsInverted() const {
  return scaleIsInverted;
}

// get the ceiling
double RtActivation::getCeiling() const {
  return ceiling;
}

// get a smart contrast level
float RtActivation::getAutoContrast() {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));
  
  if(!minMaxSet) {
    setMinMax();
  }

  double c = fabs(minVal) > fabs(maxVal) ? minVal : maxVal;
  ceiling = c > 0.0 ? c : 0.0;

  return (float) ceiling;
}

// get a smart brightness level
float RtActivation::getAutoBrightness() {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));
  
  if(!minMaxSet) {
    setMinMax();
  }

  return (float) threshold;
}

//*** sets  ***//

// sets the threshold
void RtActivation::setThreshold(double thresh) {
  threshold = thresh;
}

// sets the ceiling
void RtActivation::setCeiling(double _ceiling) {
  ceiling = _ceiling;
}

// set the scale inverted flag
void RtActivation::setScaleIsInverted(bool sii) {
  scaleIsInverted = sii;
}

// set the info based on a generic data image info
void RtActivation::setInfo(RtMRIImage &img) {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));

  dims = img.getDims();

  imgDataLen = img.getImgDataLen();
  numPix = img.getNumPix();
  bytesPerPix = img.getBytesPerPix();

  gsl_matrix_memcpy(vxl2ras, img.getVxl2Ras());
  gsl_matrix_memcpy(ras2ref, img.getRas2Ref());

  if(data != NULL) {
    delete [] data;
  }

  data = new double[numPix];
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


