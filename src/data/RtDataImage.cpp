/******************************************************************************
 * RtDataImage.cpp defines a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtDataImage.h"

#include"ace/Log_Msg.h"
#include<fstream>

using namespace std;
  
// default constructor
RtDataImage::RtDataImage() : RtData() {
  ACE_TRACE(("RtDataImage::RtDataImage()")); 
  addToID("image");

  data = NULL;
}

// constructor that accepts a filename to read an image from
RtDataImage::RtDataImage(const string &filename) : RtData(), data(NULL) {
  ACE_TRACE(("RtDataImage::RtDataImage(string)"));
  addToID("image");
  
  read(filename);
}

// construct from raw bytes sent by RtInputScannerImages
// BE CAREFUL WITH THIS
RtDataImage::RtDataImage(char *bytes, unsigned int len) : RtData() {
  ACE_TRACE(("RtDataImage::RtDataImage(char*,unsigned int)"));

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
  ACE_TRACE(("RtDataImage::RtDataImage(RtExternalImageInfo,unsigned short*)"));
  addToID("image");


  // allocate and copy the img data
  data = new unsigned short[info.numPix];
  memcpy(data,bytes,info.imgDataLen);
}


// construct from an image info struct and (possibly blank) data
RtDataImage::RtDataImage(RtDataImageInfo &_info, unsigned short *_data) 
    : RtData() {
  ACE_TRACE(("RtDataImage::RtDataImage(RtDataImageInfo,unsigned short*)"));
  addToID("image");

  setImage(_info, _data);
}

// construct from another image (deep copy)
RtDataImage::RtDataImage(RtDataImage &img) : RtData() {
  ACE_TRACE(("RtDataImage::RtDataImage(RtDataImage)"));
  persistent = img.persistent;
  id = img.id;

  info = img.info;
  data = new unsigned short[info.numPix];
  memcpy(data, img.data, info.imgDataLen);
}

// set this image based on a passed image info and data
//  in
//   info: struct
//   data: array (optional, image data will be  allocated and set
//         to all zeros if null) 
void RtDataImage::setImage(RtDataImageInfo &_info, unsigned short *_data) {
  info = _info;
  data = new unsigned short[info.numPix];

  if(_data != NULL) {
    memcpy(data, _data, info.imgDataLen);
  }
  else {
    for(unsigned int i = 0; i < info.numPix; i++) {
      data[i] = 0;
    }
  }
}

// print info about this image
void RtDataImage::printInfo(ostream &os) {
  info.print(os);
}


// destructor
RtDataImage::~RtDataImage() {
  ACE_TRACE(("RtDataImage::~RtDataImage"));

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
  ACE_TRACE(("RtDataImage::write"));

  ofstream imgFile(filename.c_str(), ios::out | ios::binary);
  
  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("could not open %s for writing an image\n"),
				  filename));
    imgFile.close();
    return false;
  }

  imgFile << MAGIC_NUMBER;

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, 
	       ACE_TEXT("couldnt write magic number %ld to %s\n"),
	       MAGIC_NUMBER, filename));
    imgFile.close();
    return false;
  }


  imgFile.write((char*) &info, sizeof(RtDataImageInfo));

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, 
	       ACE_TEXT("couldnt write image info to %s\n"),
	       filename));
    imgFile.close();
    return false;
  }


  imgFile.write((char*) data, info.imgDataLen);

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, 
	       ACE_TEXT("couldnt write image data to %s\n"),
	       filename));
    imgFile.close();
    return false;
  }

  imgFile.close();

  return true;
}


// read the image from a file
//  in
//   filename: string filename
//  out
//   success or failure
bool RtDataImage::read(const string &filename) {
  ACE_TRACE(("RtDataImage::read"));

  // delete our current image data, if we have it
  if(data != NULL) {
    delete [] data;
  }

  ifstream imgFile(filename.c_str(), ios::in | ios::binary);
  
  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("could not open %s for reading an image\n"),
				  filename));
    imgFile.close();
    return false;
  }

  long magNum;
  imgFile >> magNum;

  if(magNum != MAGIC_NUMBER) {
    ACE_DEBUG((LM_DEBUG, 
	       ACE_TEXT("magic number %ld does not match expected %ld\n"),
	       magNum, MAGIC_NUMBER));
    imgFile.close();
    return false;
  }

  // read the image info
  imgFile.read((char*) &info, sizeof(RtDataImageInfo));

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("error reading image info from %s\n"),
				  filename));
    imgFile.close();
    return false;
  }
  

  // alloc data based on info and read it
  data = new unsigned short[info.numPix];
  imgFile.read((char*) data, info.imgDataLen);

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("error reading image data from %s\n"),
				  filename));
    imgFile.close();
    return false;
  }

  imgFile.close();

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

// get the series number
unsigned int RtDataImage::getSeriesNum() const {
  return info.seriesNum;
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
  ACE_TRACE(("RtDataImage::setMinMax"));

  int mini = -1, maxi = -1;

  info.maxVal = 0;
  info.minVal = USHRT_MAX;
  for(unsigned int i = 0; i < info.numPix; i++) {
    if(data[i] > info.maxVal) {
      info.maxVal = data[i];
      maxi = i;
    }
    if(data[i] < info.minVal) {
      info.minVal = data[i];
      mini = i;
    }
  }
//  cout << "min " << info.minVal << " at " << mini 
//       << " max " << info.maxVal << " at " << maxi << endl;

  
  info.minMaxSet = true;
}

// get a smart contrast level
float RtDataImage::getAutoContrast() {
  ACE_TRACE(("RtDataImage::getAutoContrast"));
  
  if(!info.minMaxSet) {
    setMinMax();
  }

  return USHRT_MAX/(float) info.maxVal;
}


// get a smart brightness level
float RtDataImage::getAutoBrightness() {
  ACE_TRACE(("RtDataImage::getAutoBrightness"));

  if(!info.minMaxSet) {
    setMinMax();
  }

  return (float) info.minVal;

}


// set the series number
void RtDataImage::setSeriesNum(unsigned int sn) {
  info.seriesNum = sn;
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
  if((lock == NULL || lock == locker) && i < info.numPix) {
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


