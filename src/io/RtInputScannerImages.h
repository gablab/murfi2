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
#include"RtDataImage.h"
#include"ace/Task.h"
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"
#include"ace/Asynch_IO.h"

//#include"RtSocketServiceServer.h"
//#include"RtSocketScannerImage.h"


class RtImageAcceptor : public ACE_SOCK_Acceptor {
public:

  RtImageAcceptor() : isOpen(false) {}

  int close() {
    isOpen = false;
    return ACE_SOCK_Acceptor::close();
  }

  int accept(ACE_SOCK_Stream &new_stream, ACE_Addr *remote_addr=0, ACE_Time_Value *timeout=0, int restart=1, int reset_new_handle=0) {
    isOpen = true;
    return ACE_SOCK_Acceptor::accept(new_stream, remote_addr, timeout, restart, reset_new_handle=0);
  }

  bool isOpen;

};

// class declaration
class RtInputScannerImages : public RtInput, public ACE_Task_Base {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInputScannerImages(); 

  // destructor
  virtual ~RtInputScannerImages();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // set the handler that should receive new data
  bool setHandler(ACE_Handler &handler);

  // close and clean up
  bool close();

  // run the scanner input
  virtual int svc(); 

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

  // saves an image
  //  in
  //   img: image to save
  bool saveImage(RtDataImage &img);

  // sends an image to a event handler
  //  in
  //   img: image to send
  bool sendImageToReader(RtDataImage &img);

  // validate the passed host and port
  bool validateHostAndPort(RtConfig &config);

private:

  //*** private data members  ***//

  string         host;
  unsigned short port;

  ACE_INET_Addr address;
  RtImageAcceptor acceptor;   
  ACE_SOCK_Stream stream;   

  bool   saveImagesToFile;
  string saveDir;
  string saveFilestem;
  string saveFileext;

  ACE_Asynch_Read_Stream reader;
  bool readerOpen;
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

