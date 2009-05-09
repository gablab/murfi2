/******************************************************************************
 * RtClientSocket.h declares a class that implements client io operations on a
 * socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-27
 * 
 *****************************************************************************/

#ifndef RTCLIENTSOCKET_H
#define RTCLIENTSOCKET_H

#include<string>
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"

using namespace std;

// class declaration
class RtClientSocket {

public:

  //*** constructors/destructors  ***//
  
  // constructor with port and host
  RtClientSocket(const string &host, unsigned short portNum); 

  // destructor
  virtual ~RtClientSocket();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

  // send a message to a server
  // in
  //  message to send
  // out
  //  success or failure
  virtual bool sendMessageToServer(const string &message);

protected:

  // client address
  ACE_INET_Addr address;

  // data transfer object
  ACE_SOCK_Stream stream;

  // connector
  ACE_SOCK_CONNECTOR connector;

  // termination character
  char messageTerminationChar;

private:
  RtClientSocket(); // never called  

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


