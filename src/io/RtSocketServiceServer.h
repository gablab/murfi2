/******************************************************************************
 * RtSocketServiceServer.h declares an abstract class that serves a socket 
 * Uses the GNU CommonC++ library for sockets.
 * This implementation is based on SampleSocketPort.cpp, available at
 * http://www.gnu.org/software/commoncpp/docs/refman/html/examples.html, 
 * which is  Copyright 2001 - Nick Liebmann <nick@ukmail.org>
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-22
 *
 *****************************************************************************/


#ifndef RTSOCKETSERVICESERVER_H
#define RTSOCKETSERVICESERVER_H


#include<cc++/socket.h>
#include<cc++/socketport.h>

#include"RtSocketListener.h"

using namespace std;
using namespace ost;


class RtSocketServiceServer :  
  public TCPSocket, public Thread {

public:

  RtSocketServiceServer(InetHostAddress &host, int port) 
        : TCPSocket(host, port), Thread(), quitServer(true) {
    socketService = new SocketService(0); 
    socketService->start(); 
  }

  virtual ~RtSocketServiceServer() { 
    terminate(); 
    delete socketService; 
  }
	
  virtual void run() {
    waitMutex.enterMutex();

    quitServer = false;
    while(!quitServer) {
      try {
	// new does all the work to accept a new connection
	// and attach itself to the SocketService.
	createSocketPort(socketService, *((TCPSocket *)this));
      }
      catch(...) {
	cerr << "ERROR: creation of socket server failed." << endl;
	exit();
      }
    }
    waitMutex.leaveMutex();
  }

  // abstract function used to create a SocketPort of the desired type.
  virtual SocketPort *createSocketPort(SocketService *ss, TCPSocket &ts) = 0;


  virtual void startServer() {
    quitServer = true;
    start();
    while(quitServer) {
      Thread::yield();
    }
  }

  // If the server is not stopped like this then the SocketPort
  // created in createSocketPort is leaked. This allows it to complete
  // construction, and be deleted cleanly.
  virtual void stopServer() {
    quitServer = true;

    InetHostAddress host;
    tpport_t port;
    host = getLocal(&port);

    //This is required so that CreateSocketPort can return.
    TCPStream strm(host, port);
		
    waitMutex.enterMutex();
    waitMutex.leaveMutex();
  }

protected:
  
  SocketService *socketService;
  bool quitServer;
  Mutex waitMutex;

private:

};

#endif
