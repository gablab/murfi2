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
#include"RtExternalSenderImageInfo.h"
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"

//#include"RtSocketServiceServer.h"
//#include"RtSocketScannerImage.h"



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

  // run the scanner input
  bool run(); 

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // read the scanner image info from a socket stream
  // NOTE: performes blocking read
  //  in
  //   stream: a socket stream to receive on
  //  out
  //   image info struct on successful read (NULL otherwise)
  RtExternalImageInfo *receiveImageInfo(ACE_SOCK_Stream &stream);

  // read an image info from a socket stream
  // NOTE: performes blocking read
  //  in
  //   stream: a socket stream to receive on
  //   info:   the last read image info struct
  //  out
  //   image data on successful read (NULL otherwise)
  unsigned short *receiveImage(ACE_SOCK_Stream &stream, 
			       const RtExternalImageInfo &info);

  // print info about a received image
  //  in
  //   info:   the last read image info struct
  void displayImageInfo(const RtExternalImageInfo &info);

  // write a received image to a file
  //  in
  //   img:  pixel data
  //   info: info struct for the image
  //  out
  //   success or failure
  bool writeImage(unsigned short *img, const RtExternalImageInfo &info);

  // validate the passed host and port
  bool validateHostAndPort(RtConfig &config);

private:

  //*** private data members  ***//

  string         host;
  unsigned short port;

  ACE_INET_Addr address;
  ACE_SOCK_Connector connector;   
  ACE_SOCK_Stream stream;   

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

