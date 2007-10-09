/******************************************************************************
 * RtMRIImage.h declares a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 * 
 *****************************************************************************/

#include"RtMRIImage.h"
#include"ace/Log_Msg.h"
#include<fstream>

using namespace std;
  
// default constructor
RtMRIImage::RtMRIImage() : RtDataImage<unsigned short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage()")); 
  addToID("mri");

  data = NULL;

  info.setBytesPerPix(sizeof(unsigned short));
}

// constructor that accepts a filename to read an image from
RtMRIImage::RtMRIImage(const string &filename) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(string)"));
  addToID("mri");
  
  data = NULL;

  info.setBytesPerPix(sizeof(unsigned short));

  read(filename);
}

// construct from raw bytes sent by RtInputScannerImages
// BE CAREFUL WITH THIS
RtMRIImage::RtMRIImage(char *bytes, unsigned int len) 
    : RtDataImage<unsigned short>(bytes,len) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(char*,unsigned int)"));

  addToID("mri");
}

// construct from an image info struct and some byte data
RtMRIImage::RtMRIImage(RtExternalImageInfo &extinfo, unsigned short *bytes) 
    : RtDataImage<unsigned short>(), info(extinfo) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtExternalImageInfo,unsigned short*)"));

  addToID("mri");

  info.setBytesPerPix(sizeof(unsigned short));

  // allocate and copy the img data
  data = new unsigned short[info.numPix];
  memcpy(data,bytes,info.imgDataLen);
}


// construct from an image info struct and (possibly blank) data
RtMRIImage::RtMRIImage(RtMRIImageInfo &_info, unsigned short  *_data) 
    : RtDataImage<unsigned short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtMRIImageInfo,T*)"));
  addToID("mri");

  setImage(_info, _data);
}

// construct from another image (deep copy)
// only use this with like datatypes
RtMRIImage::RtMRIImage(RtMRIImage &img) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtMRIImage)"));

  persistent = img.persistent;
  id = img.id;

  info = img.info;
  info.setBytesPerPix(sizeof(unsigned short));

  data = new unsigned short[info.numPix];
  
  // copy the data 
  if(img.info.bytesPerPix == info.bytesPerPix) {
    memcpy(data, img.data, info.imgDataLen);
  }

}

// set this image based on a passed image info and data
//  in
//   info: struct
//   data: array (optional, image data will be  allocated and set
//         to all zeros if null) 
void RtMRIImage::setImage(RtMRIImageInfo &_info, unsigned short *_data) {
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
void RtMRIImage::printInfo(ostream &os) {
  info.print(os);
}


// destructor
RtMRIImage::~RtMRIImage() {
  ACE_TRACE(("RtMRIImage::~RtMRIImage"));

  // notify our locker that we are being deleted
  if(lock != NULL) {
    lock->beingDeleted();
  }

  delete [] data;
}

// write the image to a file
// TODO: templatize this or something so dont have to copy (info is the issue)
//  in
//   filename: string filename
//  out
//   success or failure
bool RtMRIImage::write(const string &filename) {
  ACE_TRACE(("RtMRIImage::write"));

  ofstream imgFile(filename.c_str(), ios::out | ios::binary);
  
  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("could not open %s for writing an image\n"),
				  filename));
    imgFile.close();
    return false;
  }

  imgFile << info.magicNumber;

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, 
	       ACE_TEXT("couldnt write magic number %ld to %s\n"),
	       MAGIC_NUMBER, filename));
    imgFile.close();
    return false;
  }


  imgFile.write((char*) &info, sizeof(RtMRIImageInfo));

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
bool RtMRIImage::read(const string &filename) {
  ACE_TRACE(("RtMRIImage::read"));

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

  if(!info.validateMagicNumber(magNum)) {
    ACE_DEBUG((LM_DEBUG, 
	       ACE_TEXT("magic number %ld is invalid\n"), magNum));
    imgFile.close();
    return false;
  }

  // read the image info
  imgFile.read((char*) &info, sizeof(RtMRIImageInfo));

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
string RtMRIImage::getCreationTime() const {
  return RtDataImageInfo::ACE_Date_Time2SiemensTime(creationTime);
}

// get the acquisition number
unsigned int RtMRIImage::getAcquisitionNum() const {
  return info.acqNum;
}

// get the series number
unsigned int RtMRIImage::getSeriesNum() const {
  return info.seriesNum;
}

// get the image info
RtMRIImageInfo &RtMRIImage::getInfo() {
  return info;
}

// set info struct
//  in
//   _info: struct to copy
void RtMRIImage::setInfo(const RtExternalImageInfo &_info) {
  info = _info;
}

// sets the min and max pixel value for this data image
void RtMRIImage::setMinMax() {
  ACE_TRACE(("RtMRIImage::setMinMax"));

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
float RtMRIImage::getAutoContrast() {
  ACE_TRACE(("RtMRIImage::getAutoContrast"));
  
  if(!info.minMaxSet) {
    setMinMax();
  }

  return USHRT_MAX/(float) info.maxVal;
}


// get a smart brightness level
float RtMRIImage::getAutoBrightness() {
  ACE_TRACE(("RtMRIImage::getAutoBrightness"));

  if(!info.minMaxSet) {
    setMinMax();
  }

  return (float) info.minVal;

}


// set the series number
void RtMRIImage::setSeriesNum(unsigned int sn) {
  info.seriesNum = sn;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


