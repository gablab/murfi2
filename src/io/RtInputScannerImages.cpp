/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"
  
// default constructor
RtInputScannerImages::RtInputScannerImages() {

}

// destructor
RtInputScannerImages::~RtInputScannerImages() {

}

// configure and open
bool RtInputScannerImages::open(const RtConfig &config) {
  RtInput::open(config);

  return true;
}

// close and clean up
bool RtInputScannerImages::close() {
  RtInput::close();  

  return true;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtInputScannerImages::getVersionString() {
  return VERSION;
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


