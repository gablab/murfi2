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
#include"RtSocketServiceServer.h"
#include"RtSocketScannerImage.h"

// class declaration
class RtInputScannerImages : public RtInput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInputScannerImages(); 

  // destructor
  virtual ~RtInputScannerImages();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // close and clean up
  bool close();

  //*** socket functions ***//

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // validate the passed host and port
  bool validateHostAndPort(RtConfig &config);

private:

  //*** private data members  ***//
  InetHostAddress *host;
  int    port;

  bool   saveImagesToFile;
  string saveDirectory;
  string saveFilestem;

  
  RtSocketServiceServer *socketService;

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

