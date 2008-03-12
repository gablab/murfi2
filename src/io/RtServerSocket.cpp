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
  messageTerminationChar = '\n';
}

// constructor with port and host
RtServerSocket::RtServerSocket(unsigned short portNum) {
  messageTerminationChar = '\n';
  address.set(portNum);
}

// destructor
RtServerSocket::~RtServerSocket() {
  if(acceptor.isOpen) {
    close();
  }
}

// open and start accepting
bool RtServerSocket::open(RtConfig &config) {
  // find port
  if(config.isSet("infoserver:port")) {
    address.set_port_number((unsigned short) config.get("infoserver:port"));
  }

  if(acceptor.open(address,1) == -1) {
    cerr << "couldn't open acceptor to listen on " << address.get_host_name() 
	 << ":" << address.get_port_number() << endl;
    acceptor.isOpen = false;
    return false;
  }
  cerr << "opened acceptor to listen on " << address.get_host_name() 
       << ":" << address.get_port_number() << endl;

  acceptor.isOpen = true;
  return true;
}

// close and clean up
bool RtServerSocket::close() {
  acceptor.close();
  return true;
}

// receive a message
// in   string recieved
// out  string response
string RtServerSocket::recieveMessage(string &message, ACE_SOCK_Stream &stream) {
  cout << "in wrong version" << endl;
  return message;
}

// send a message to a client
// in
//  message to send
//  stream to send on
// out
//  success or failure
bool RtServerSocket::sendMessage(const string &message, ACE_SOCK_Stream &stream) {
  unsigned int sent = stream.send_n(message.c_str(), message.length());
  if(sent < message.length()) {
    cerr << "incomplete send" << endl;
    return false;
  }
  
  stream.send_n(&messageTerminationChar,1);

  cout << "sent " << message << endl;

  return true;
}

// listen for connections
int RtServerSocket::svc() {
  // continuously try to accept connections
  while(acceptor.isOpen && acceptor.accept(stream) != -1) {
    stringstream message;

    // get characters until null character
    char last = 'a';
    while(last != messageTerminationChar) {
      if(0 == stream.recv_n(&last,1)) {
	cout << "received empty message" << endl;
	break;
      }

      message << last;
    }

    // receive the message, store the response
    string recieved = message.str();
    string response = recieveMessage(recieved,stream);

    // send the response
    sendMessage(response, stream);

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


