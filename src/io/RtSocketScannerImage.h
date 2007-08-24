/******************************************************************************
 * RtSocketScannerImage.h declares a class that implements scanner
 * image reading from a socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-22
 *
 *****************************************************************************/

#ifndef RTSOCKETSCANNERIMAGE_H
#define RTSOCKETSCANNERIMAGE_H

#include"RtSocketServiceServer.h"
#include"RtSocketListener.h"
#include"ExternalSenderImageInfo.h"
#include"RtDataImage.h"

// class that handles reading scanner images from a socket
class RtSocketScannerImage : public RtSocketListener {

public:

  // construct the socket and start listening
  RtSocketScannerImage(SocketService *ss, TCPSocket &ts);

  virtual ~RtSocketScannerImage();

  // do stuff when the socket is closed
  virtual void onClose();

  // accept data
  virtual void dataReceived(char *data, unsigned int count);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

  
protected:

  // extract an image info struct from the received data
  // checks for errors
  //  out: pointer to the image info struct received
  ExternalImageInfo *extractImageInfo(char *data, unsigned int count);

  // extracts an image 
  RtDataImage *extractImage(ExternalImageInfo &info, char *data, 
			    unsigned int count);


  // print out information about the image
  void dumpImageInfo(ostream &os, ExternalImageInfo &info);


};

class RtSocketServiceServerScannerImage : public RtSocketServiceServer {

public:

  // constructor
  RtSocketServiceServerScannerImage(InetHostAddress &host, int port) :
    RtSocketServiceServer(host, port) {
  }

  // destructor
  virtual ~RtSocketServiceServerScannerImage()
  { terminate(); delete socketService; }

  // create the scanner image socket
  // reason we derived this class
  virtual SocketPort *createSocketPort(SocketService *ss, TCPSocket &ts) {
    return (new RtSocketScannerImage(ss,ts));
  }
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

