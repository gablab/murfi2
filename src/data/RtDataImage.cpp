/******************************************************************************
 * RtDataImage.cpp defines a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtDataImage.h"

  
// default constructor
RtDataImage::RtDataImage() : RtData() {
  addToID("image");

  data = NULL;
}

// construct from raw bytes sent by RtInputScannerImages
// BE CAREFUL WITH THIS
RtDataImage::RtDataImage(char *bytes, unsigned int len) : RtData() {
  addToID("image");

  // try to do some checks
  if(len < sizeof(RtDataImageInfo)) {
    return;
  }

  // extract the info
  memcpy(&info,bytes,sizeof(RtDataImageInfo));

  // check if we have enough for the image data
  if(len-sizeof(RtDataImageInfo) != info.imgDataLen) {
    return;
  }

  data = new unsigned short[info.numPix];
  memcpy(data,bytes+sizeof(RtDataImageInfo),info.imgDataLen);
}

// construct from an image info struct and some byte data
RtDataImage::RtDataImage(RtExternalImageInfo &extinfo, unsigned short *bytes) 
    : RtData(), info(extinfo) {
  addToID("image");


  // allocate and copy the img data
  data = new unsigned short[info.numPix];
  memcpy(data,bytes,info.imgDataLen);
}


// construct from an image info struct and (possibly blank) data
RtDataImage::RtDataImage(RtDataImageInfo &_info, unsigned short *_data) 
    : RtData() {
  addToID("image");

  info = _info;
  data = new unsigned short[info.numPix];

  if(_data != NULL) {
    memcpy(data, _data, info.imgDataLen);
  }
}

// construct from another image (deep copy)
RtDataImage::RtDataImage(RtDataImage &img) {
  persistent = img.persistent;
  id = img.id;

  info = img.info;
  data = new unsigned short[info.numPix];
  memcpy(data, img.data, info.imgDataLen);
}


// print info about this image
void RtDataImage::printInfo(ostream &os) {
  info.print(os);
}


// destructor
RtDataImage::~RtDataImage() {
  // notify our locker that we are being deleted
  if(lock != NULL) {
    lock->beingDeleted();
  }

  delete [] data;
}

// write the image to a file
//  in
//   filename: string filename
//  out
//   success or failure
bool RtDataImage::write(const string &filename) {

  unsigned int one = 1;
  unsigned int rows = info.dims[0];
  unsigned int cols = info.dims[1];
  

  FILE *imgFile = fopen(filename.c_str(),"wb");
  
  fwrite(&rows, sizeof(unsigned int), 1, imgFile);
  fwrite(&cols, sizeof(unsigned int), 1, imgFile);
  fwrite(&one,  sizeof(unsigned int), 1, imgFile);
  fwrite(data,  info.bytesPerPix, info.numPix, imgFile);
  fclose(imgFile);

  return true;
}


// get the acquisition number
string RtDataImage::getCreationTime() const {
  return RtDataImageInfo::ACE_Date_Time2SiemensTime(creationTime);
}

// get the acquisition number
unsigned int RtDataImage::getAcquisitionNum() const {
  return info.acqNum;
}

// get dimensions
int RtDataImage::getDim(int i) {
  return info.numDims > i && i >= 0 ? info.dims[i] : -1;
}

// get number of pix
int RtDataImage::getNumPix() {
  return info.numPix;
}

// get pixel value
unsigned short RtDataImage::getPixel(unsigned int i) {
  return i < info.numPix ? data[i] : 0;
}

// get data size
unsigned int RtDataImage::getImgDataLen() {
  return info.imgDataLen;
}

// get the image info
RtDataImageInfo &RtDataImage::getInfo() {
  return info;
}

// get a pointer to the image data
unsigned short *RtDataImage::getData() {
  return data;
}


// set info struct
//  in
//   _info: struct to copy
void RtDataImage::setInfo(const RtExternalImageInfo &_info) {
  info = _info;
}

// sets the min and max pixel value for this data image
void RtDataImage::setMinMax() {
  info.maxVal = 0;
  info.minVal = USHRT_MAX;
  for(unsigned int i = 0; i < info.numPix; i++) {
    if(data[i] > info.maxVal) {
      info.maxVal = data[i];
    }
    if(data[i] < info.minVal) {
      info.minVal = data[i];
    }
  }
  
  info.minMaxSet = true;
}

// get a smart contrast level
float RtDataImage::getAutoContrast() {
  
  if(!info.minMaxSet) {
    setMinMax();
  }

  return USHRT_MAX/(float) info.maxVal;
}


// get a smart brightness level
float RtDataImage::getAutoBrightness() {
  if(!info.minMaxSet) {
    setMinMax();
  }

  return (float) info.minVal;

}

// set pixel value
void RtDataImage::setPixel(unsigned int i, unsigned short v) {
  if(lock == NULL && i < info.numPix) {
    data[i] = v;
  }
}

// set pixel value when locked
void RtDataImage::setPixelLocked(RtLocker *locker, 
				 unsigned int i, unsigned short v) {
  if(lock == NULL || lock == locker && i < info.numPix) {
    data[i] = v;
  }
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


