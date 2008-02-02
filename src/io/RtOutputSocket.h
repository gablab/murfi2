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

  // write the string from a stringstream to the output socket
  void write(stringstream &ss);

  // write a string to the output socket
  void write(const string &s);

  // close and clean up
  bool close();

  // write to the socket 
  template<class T>
  RtOutputSocket &operator<<(const T &t) {
    if(isOpen) {
      //      outfp << t;
    }
    return *this;
  }

  // outstream operator
  void writeConfig(RtConfig &config);

  // prints the current time 
  void printNow();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


protected:

  // client address
  ACE_INET_Addr address;

  // data transfer object
  ACE_SOCK_Stream stream;

  // Initialize the connector.
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


