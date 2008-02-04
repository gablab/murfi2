/******************************************************************************
 * RtOutputSocket.h declares a class that implements output operations on a
 * socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-27
 * 
 *****************************************************************************/

#ifndef RTOUTPUTSOCKET_H
#define RTOUTPUTSOCKET_H

#include"RtOutput.h"
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"

using namespace std;

// class declaration
class RtOutputSocket : public RtOutput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtOutputSocket(); 

  // destructor
  virtual ~RtOutputSocket();

  // open and start accepting input
  bool open(RtConfig &config);

  // output data to the socket
  void setData(RtData *data);

  // close and clean up
  bool close();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


protected:

  // client address
  ACE_INET_Addr address;

  // data transfer object
  ACE_SOCK_Stream stream;

  // connector
  ACE_SOCK_Connector connector;
  
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


