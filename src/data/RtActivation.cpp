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
  ACE_TRACE(("RtMRIImage::RtActivation()")); 
  addToID("activation");
  magicNumber = MAGIC_NUMBER;
  threshold = 10.0;
  ceiling = 50.0;
  bytesPerPix = sizeof(double);
}

// constructor with MRIInfo
RtActivation::RtActivation(RtMRIImage &img) 
  : RtDataImage<double>() {
  ACE_TRACE(("RtMRIImage::RtActivation(RtMRIImage)")); 
  addToID("activation");
  magicNumber = MAGIC_NUMBER;

  setInfo(img);
  bytesPerPix = sizeof(double);
  threshold = 10.0;
  ceiling = 50.0;
}

// constructor with number of elements 
RtActivation::RtActivation(unsigned int numElements) 
  : RtDataImage<double>() {
  ACE_TRACE(("RtMRIImage::RtActivation(unsigned int)")); 
  addToID("activation");
  magicNumber = MAGIC_NUMBER;

  dims.reserve(1);
  dims.clear();
  dims[0] = numElements;

  numPix = numElements;
  bytesPerPix = sizeof(double);
  imgDataLen = numPix * bytesPerPix;

  reallocateData();

  threshold = 0.0;
  ceiling = 50.0;
}

// destructor
RtActivation::~RtActivation() {
}

// create a mask by thresholding the activation
// in:
//  sign of activation to take (POS,NEG,POSNEG)
// out:
//  binary mask image 
RtMaskImage *RtActivation::toMask(Sign direction) {
  // create a boolean image the same size as this image
  RtMaskImage *activationMask = new RtMaskImage(*this);
  
  // assign the mask based on activation and direction
  for(unsigned int i = 0; i < getNumEl(); i++) {
    double voxel = getPixel(i);
    
    switch(direction) {
    case POS:
      activationMask->setPixel(i,voxel >= threshold ? true : false);
      break;
    case NEG:
      activationMask->setPixel(i,voxel <= -threshold ? true : false);
      break;
    case POSNEG:
      activationMask->setPixel(i,fabs(voxel) >= threshold ? true : false);
      break;
    }
  }

  return activationMask;
}

// serialize the data as xml for transmission or saving to a file
TiXmlElement *RtActivation::serializeAsXML() {
  // add an element for the data
  TiXmlElement *element = new TiXmlElement("data");
  element->SetAttribute("name","activation");
  element->SetAttribute("roiID",roiID);

  // build text string containing all the data separated by spaces
  #define NUM_SIGFIGS 6
  stringstream dataStr;
  dataStr << fixed << setprecision(NUM_SIGFIGS);
  for(unsigned int i = 0; i < getNumEl(); i++) {    
    dataStr << getPixel(i);
  }

  TiXmlText *text = new TiXmlText(dataStr.str());
  element->LinkEndChild(text);

  return element;
}

// print info about this image
void RtActivation::printInfo(ostream &os) {
  ACE_TRACE(("RtActivation::printInfo"));
  
  int wid = 30;
  
  RtDataImage<double>::printInfo(os);
  
  os << setw(wid) << "roiID: " << roiID << endl;
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

  numPix = img.getNumPix();
  bytesPerPix = sizeof(double);
  imgDataLen = numPix*bytesPerPix;

  vxl2ras = img.getVxl2Ras();
  ras2ref = img.getRas2Ref();

  reallocateData();
}

// reallocate data array based on current number of pixels
void RtActivation::reallocateData() {
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


