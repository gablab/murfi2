/******************************************************************************
 * RtClientSocket.cpp defines a class that implements client io operations on a
 * socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-27
 *
 *****************************************************************************/

#include"RtClientSocket.h"

#include<iostream>

static char *VERSION = "$Id: RtClientSocket.cpp 195 2008-08-29 22:40:54Z ohinds $";

// constructor with port and host
RtClientSocket::RtClientSocket(const string &host, unsigned short portNum) {
  messageTerminationChar = '\n';
  address.set(portNum,host.c_str());
}

// destructor
RtClientSocket::~RtClientSocket() {
}

// open and start accepting
bool RtClientSocket::sendMessageToServer(const string &message) {
  if(!connector.connect(stream, address)) {
    unsigned int sent = stream.send_n(message.c_str(), message.length());
    if(sent < message.length()) {
      cerr << "incomplete send" << endl;
      return false;
    }
  
    stream.send_n(&messageTerminationChar,1);

    stream.close();

    return true;
  }
  return false;
}

// get the version
//  out: char array that represents the cvs version
char *RtClientSocket::getVersionString() {
  return VERSION;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


