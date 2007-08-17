/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtInputScannerImages.h"
  
// default constructor
RtInputScannerImages::RtInputScannerImages() {

}

// destructor
RtInputScannerImages::~RtInputScannerImages() {

}

// configure
bool RtInputScannerImages::init(RtConfig &config) {
  RtInput::init(config);

  return true;
}

// open and start accepting input
bool RtInputScannerImages::open() {
  RtInput::open();

  return true;
}

// close and clean up
bool RtInputScannerImages::close() {
  RtInput::close();  

  return true;
}


//  //*** private data members  ***//
//
//  string tcpPort;
//  bool   saveImagesToFile;
//  string saveDirectory;
//  string saveFilestem;

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


