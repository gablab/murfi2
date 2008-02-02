/******************************************************************************
 * RtOutputsocket.cpp defines a class that implements output operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";


#include"RtOutputSocket.h"

#include"ace/Mutex.h"

// default constructor
RtOutputSocket::RtOutputSocket() : RtOutput() {
  id += ":socket";
}

// destructor
RtOutputSocket::~RtOutputSocket() {
}

// open and start accepting input
bool RtOutputSocket::open(RtConfig &config) {

  if(!RtOutput::open(config)) {
    return false;
  }
  
  // setup the socket address
  //address.set(portNum, hostId);

  return true;
}

// write from a stringstream to the output socket
void RtOutputSocket::write(stringstream &ss) {
  write(ss.str());
}

// write a string to the output socket
void RtOutputSocket::write(const string &s) {
  if(!isOpen) {
    return;
  }

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  //outfp << s;

  //outfp.flush();

  mutx.release();
}

// close and clean up
bool RtOutputSocket::close() {

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  //outfp << "closed ";
  //RtOutput::printNow(outfp);
  //outfp << endl;
  //outfp << endl;
  //
  //outfp << "################################################################"
  //	<< endl << endl;
  //
  //outfp.close();

  isOpen = false;

  mutx.release();

  return true;
}

// prints the current time to the socket
void RtOutputSocket::printNow() {
  //RtOutput::printNow(outfp);
}

// prints the configuration to the socket
void RtOutputSocket::writeConfig(RtConfig &config) {

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  //outfp << "configuration:" << endl 
  //	<< "--------------" << endl;
  //config.dumpConfig(outfp);
  //outfp << "--------------" << endl << endl;

  mutx.release();
}

// gets the version
//  out:
//   cvs version string for this class
char *RtOutputSocket::getVersionString() {
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


