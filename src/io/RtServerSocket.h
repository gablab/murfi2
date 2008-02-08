/******************************************************************************
 * RtServerSocket.h declares a class that implements server io operations on a
 * socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-27
 * 
 *****************************************************************************/

#ifndef RTSERVERSOCKET_H
#define RTSERVERSOCKET_H

#include"RtInput.h"
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"

using namespace std;

// class declaration
class RtServerSocket : public RtInput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtServerSocket(); 

  // destructor
  virtual ~RtServerSocket();

  // open and start accepting
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
  ACE_SOCK_Acceptor acceptor;
  
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


