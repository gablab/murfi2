/******************************************************************************
 * RtDataImage.h declares a base class for a generic image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTDATAIMAGE_H
#define RTDATAIMAGE_H

#include<iostream>
#include"ace/Date_Time.h"

#include"RtDataImageInfo.h"
#include"RtData.h"

using namespace std;


// class declaration
template<class T>
class RtDataImage : public RtData {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtDataImage(); 

  // constructor with a filename to read the image from
  RtDataImage(const string &filename); 

  // construct from raw bytes -- BE CAREFUL WITH THIS
  RtDataImage(char *bytes, unsigned int len);

  // construct from an image info struct and (possibly blank) data
  RtDataImage(RtDataImageInfo &info, T *data = NULL); 

  // construct from another image
  RtDataImage(RtDataImage &img);

  // set this image based on a passed image info and data
  //  in
  //   info: struct
  //   data: array (optional, image data will be  allocated and set
  //         to all zeros if null) 
  void setImage(RtDataImageInfo &_info, T*_data = NULL);

  // save the image to a file (already set filename)
  //  out
  //   success or failure
  bool save();

  // write an image to a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool write(const string &filename);

  // load the image to a file (already set filename)
  //  out
  //   success or failure
  bool load();

  // read an image from a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool read(const string &filename);

  // print info about this image
  void printInfo(ostream &os);

  // destructor
  virtual ~RtDataImage();

  //********  methods for getting data from the image *******//

  // get dimensions
  int getDim(int i);

  // get number of pix
  int getNumPix();

  // get pixel value
  T getPixel(unsigned int i);

  // get data size
  unsigned int getImgDataLen();

  // get the image info
  RtDataImageInfo &getInfo();

  // get a pointer to the image data
  T *getData();

  //*** sets  ***//

  // set pixel value
  void setPixel(unsigned int i, T v);

  // set pixel value when locked
  void setPixelLocked(RtLocker *locker, unsigned int i, T v); 

protected:

  //*** private data members  ***//
  
  ACE_Date_Time creationTime;

  T *data;

  RtDataImageInfo info;
  
};


// definition of template class here
// (because you can't separate the declaration and definition (STUPUD!!)

#include"ace/Log_Msg.h"
#include<fstream>

using namespace std;
  
// default constructor
template<class T>
RtDataImage<T>::RtDataImage() : RtData() {
  ACE_TRACE(("RtDataImage<T>::RtDataImage()")); 

  addToID("image");

  data = NULL;

  info.setBytesPerPix(sizeof(T));
}

// constructor that accepts a filename to read an image from
template<class T>
RtDataImage<T>::RtDataImage(const string &filename) : RtData(), data(NULL) {
  ACE_TRACE(("RtDataImage<T>::RtDataImage(string)"));
  
  addToID("image");

  info.setBytesPerPix(sizeof(T));

  read(filename);
}

// construct from raw bytes sent by RtInputScannerImages
// BE CAREFUL WITH THIS
template<class T>
RtDataImage<T>::RtDataImage(char *bytes, unsigned int len) : RtData() {
  ACE_TRACE(("RtDataImage<T>::RtDataImage(char*,unsigned int)"));

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

  data = new T[info.numPix];
  memcpy(data,bytes+sizeof(RtDataImageInfo),info.imgDataLen);
}


// construct from an image info struct and (possibly blank) data
template<class T>
RtDataImage<T>::RtDataImage(RtDataImageInfo &_info, T *_data) 
    : RtData() {
  ACE_TRACE(("RtDataImage<T>::RtDataImage(RtDataImageInfo,T*)"));

  addToID("image");

  setImage(_info, _data);
}

// construct from another image (deep copy)
// only use this with like datatypes
template<class T>
RtDataImage<T>::RtDataImage(RtDataImage &img) : RtData() {
  ACE_TRACE(("RtDataImage<T>::RtDataImage(RtDataImage)"));
  persistent = img.persistent;
  id = img.id;

  info = img.info;
  info.setBytesPerPix(sizeof(T));

  data = new T[info.numPix];
  
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
template<class T>
void RtDataImage<T>::setImage(RtDataImageInfo &_info, T *_data) {
  info = _info;
  data = new T[info.numPix];

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
template<class T>
void RtDataImage<T>::printInfo(ostream &os) {
  info.print(os);
}


// destructor
template<class T>
RtDataImage<T>::~RtDataImage() {
  ACE_TRACE(("RtDataImage<T>::~RtDataImage"));

  // notify our locker that we are being deleted
  if(lock != NULL) {
    lock->beingDeleted();
  }

  delete [] data;
}


// save the image to a file (already set filename)
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::save() {
  ACE_TRACE(("RtDataImage<T>::save"));
  
  if(info.filename == "") {
    return false;
  }

  return write(info.filename);
}

// write the image to a file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::write(const string &filename) {
  ACE_TRACE(("RtDataImage<T>::write"));

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
	       info.magicNumber, filename));
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

// load the image to a file (already set filename)
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::load() {
  ACE_TRACE(("RtDataImage<T>::save"));
  
  if(info.filename == "") {
    return false;
  }

  return read(info.filename);
}


// read the image from a file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::read(const string &filename) {
  ACE_TRACE(("RtDataImage<T>::read"));

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
  imgFile.read((char*) &info, sizeof(RtDataImageInfo));

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("error reading image info from %s\n"),
				  filename));
    imgFile.close();
    return false;
  }
  

  // alloc data based on info and read it
  data = new T[info.numPix];
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

// get dimensions
template<class T>
int RtDataImage<T>::getDim(int i) {
  return info.numDims > i && i >= 0 ? info.dims[i] : -1;
}

// get number of pix
template<class T>
int RtDataImage<T>::getNumPix() {
  return info.numPix;
}

// get pixel value
template<class T>
T RtDataImage<T>::getPixel(unsigned int i) {
  return i < info.numPix ? data[i] : 0;
}

// get data size
template<class T>
unsigned int RtDataImage<T>::getImgDataLen() {
  return info.imgDataLen;
}

// get the image info
template<class T>
RtDataImageInfo &RtDataImage<T>::getInfo() {
  return info;
}

// get a pointer to the image data
template<class T>
T *RtDataImage<T>::getData() {
  return data;
}

// set pixel value
template<class T>
void RtDataImage<T>::setPixel(unsigned int i, T v) {
  if(lock == NULL && i < info.numPix) {
    data[i] = v;
  }
}

// set pixel value when locked
template<class T>
void RtDataImage<T>::setPixelLocked(RtLocker *locker, 
				 unsigned int i, T v) {
  if((lock == NULL || lock == locker) && i < info.numPix) {
    data[i] = v;
  }
}


#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


