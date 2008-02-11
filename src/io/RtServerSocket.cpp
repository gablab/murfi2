/******************************************************************************
 * RtServerSocket.cpp defines a class that implements server io operations on a
 * socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-27
 *
 *****************************************************************************/

#include"RtServerSocket.h"

static char *VERSION = "$Id$";

// default constructor
RtServerSocket::RtServerSocket() {

}

// constructor with port and host
RtServerSocket::RtServerSocket(unsigned short portNum, string hostName = "") {
  address.set(portNum, hostName.c_str());
}

// destructor
RtServerSocket::~RtServerSocket() {
  if(acceptor.isOpen) {
    close();
  }
}

// open and start accepting
bool RtServerSocket::open(RtConfig &config) {
  // find host and port
  if(config.isSet("socket:host")) {
    string host = config.get("socket:host");
    address.set_address(host.c_str(). host.length());
  }
  if(config.isSet("socket:port")) {
    address.set_port_number((unsigned short) config.get("socket:port"));
  }

  acceptor.open(address,1);
}

// close and clean up
bool RtServerSocket::close() {
  acceptor.close();
}

// receive a message
// in   string recieved
// out  success or failure
bool RtServerSocket::recieveMessage(string message, ACE_SOCK_Stream &stream) {
  cout << "recieved " << message << endl;
  return true;
}

// listen for connections
int RtServerSocket::svc() {
  // continuously try to accept connections
  while(acceptor.isOpen && acceptor.accept(stream) != -1) {
    stringstream message;

    // get characters until null character
    char last = 'a';
    while(last != '\0') {
      stream.recv_n(&last,1);
      message << last;
    }

    // pass the message along
    recieveMessage(message.str(),stream);

    // close the stream (scanner connects anew for each image)
    stream.close();
  }
  acceptor.isOpen = false;

  return 0;
}

// get the version
//  out: char array that represents the cvs version
char *RtServerSocket::getVersionString() {
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


