/******************************************************************************
 * RtInputScannerImages.h declares a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTINPUTSCANNERIMAGES_H
#define RTINPUTSCANNERIMAGES_H

#include"RtInput.h"
#include"RtConfig.h"

// class declaration
class RtInputScannerImages : public RtInput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInputScannerImages(); 

  // destructor
  virtual ~RtInputScannerImages();

  // open and start accepting input
  bool open(const RtConfig &config);

  // close and clean up
  bool close();
  
  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

private:

  //*** private data members  ***//

  string tcpPort;
  bool   saveImagesToFile;
  string saveDirectory;
  string saveFilestem;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


