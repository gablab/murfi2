/******************************************************************************
 * RtSocketListener.cpp defines a class for input from a tcp socket. 
 * Uses the GNU CommonC++ library for sockets.
 * This implementation is based on SampleSocketPort.cpp, available at
 * http://www.gnu.org/software/commoncpp/docs/refman/html/examples.html, 
 * which is  Copyright 2001 - Nick Liebmann <nick@ukmail.org>
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTSOCKETLISTENER_H
#define RTSOCKETLISTENER_H

#include<cc++/socketport.h>

#include"RtConfig.h"

using namespace ost;
using namespace std;

// maximum number of bytes in a 'packet' 
#define MAX_RXBUF	32768 

// how long we will wait for a complete packet 
#define MAX_RXTIMEOUT	10000

// timeout for the diconnect timer xb
#define	DISCONNECT_MS	500			

class RtSocketListener : public SocketPort {

public:

  // constructor
  RtSocketListener(SocketService *ss, TCPSocket &ts);

  // destructor
  virtual ~RtSocketListener();

  // indicates data ready to be receive()ed
  virtual void pending();

  // handle disconnect
  virtual void disconnect(void);

  // called on timeout
  virtual void expired(void);

  // closes the socket
  virtual bool close();

  // do stuff on read timeout
  virtual void onRxTimeout();

  // time the read
  void setReadTimer(timeout_t timeout);

  // flush on error
  void flushRxData();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


  //*** these functions to be defined in derived class ***//

  // do stuff when data received
  virtual void dataReceived(char *data, unsigned int count) = 0;

  // do stuff when port closes
  virtual void onClose() = 0;

protected:

  //*** protected members ***//

  bool open;
  bool disconnecting;
  bool timedOut;
  bool rxStarted;
  int  lastNumAvail;
  char *buffer;

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

