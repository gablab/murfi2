/******************************************************************************
 * program to save images sent from the scanner
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-31
 *****************************************************************************/

#include"ExternalSenderImageInfo.h"

#ifdef WIN32
#include <winsock.h>
#else

#endif

#include <iostream>
using namespace std;

const int bufferLen = 1024;


// prototypes
SOCKET SetUpListener(const char* pcAddress, int nPort);
SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in& sinRemote);
bool EchoIncomingPackets(SOCKET sd);


int runWinReceiver(unsigned int port) {
  cout << "starting the receiver...";
  SOCKET sock = getWinListener(htons(port));
  if(sock == INVALID_SOCKET) {
    cout << "failed"
    return 1;
  }
  cout << "success" << endl;

  while(1) {
    cout << "listening..." << flush;
    sockaddr_in remote;
    SOCKET sd = acceptWin(sock, remote);
    if(sd != INVALID_SOCKET) {
      cout << "receiving image from " << inet_ntoa(remote.sin_addr) << ":" <<
	ntohs(remote.sin_port)  << endl;
    }
    else {
      cout << "connection error" << endl;
      return 1;
    }

    // receive and build image
    

    // close connection

  }

  return 0;
}


SOCKET getWinListener(unsigned int port) {
  SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd != INVALID_SOCKET) {
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = port;
    if(bind(sd, (sockaddr*)&sin, sizeof(sockaddr_in)) != SOCKET_ERROR) {
      listen(sd, 1);
      return sd;
    }
  }

  return INVALID_SOCKET;
}


//// AcceptConnection //////////////////////////////////////////////////
// Waits for a connection on the given socket.  When one comes in, we
// return a socket for it.  If an error occurs, we return
// INVALID_SOCKET.

SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in& sinRemote)
{
  int nAddrSize = sizeof(sinRemote);
  return accept(ListeningSocket, (sockaddr*)&sinRemote, &nAddrSize);
}


//// EchoIncomingPackets ///////////////////////////////////////////////
// Bounces any incoming packets back to the client.  We return false
// on errors, or true if the client closed the socket normally.

bool EchoIncomingPackets(SOCKET sd)
{
  // Read data from client
  char acReadBuffer[kBufferSize];
  int nReadBytes;
  do {
    nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
    if (nReadBytes > 0) {
      cout << "Received " << nReadBytes <<
	" bytes from client." << endl;

      int nSentBytes = 0;
      while (nSentBytes < nReadBytes) {
	int nTemp = send(sd, acReadBuffer + nSentBytes,
			 nReadBytes - nSentBytes, 0);
	if (nTemp > 0) {
	  cout << "Sent " << nTemp <<
	    " bytes back to client." << endl;
	  nSentBytes += nTemp;
	}
	else if (nTemp == SOCKET_ERROR) {
	  return false;
	}
	else {
	  // Client closed connection before we could reply to
	  // all the data it sent, so bomb out early.
	  cout << "Peer unexpectedly dropped connection!" <<
	    endl;
	  return true;
	}
      }
    }
    else if (nReadBytes == SOCKET_ERROR) {
      return false;
    }
  } while (nReadBytes != 0);

  cout << "Connection closed by peer." << endl;
  return true;
}


bool ShutdownConnection(SOCKET sd)
{
    // Disallow any further data sends.  This will tell the other side
    // that we want to go away now.  If we skip this step, we don't
    // shut the connection down nicely.
    if (shutdown(sd, SD_SEND) == SOCKET_ERROR) {
        return false;
    }

    // Receive any extra data still sitting on the socket.  After all
    // data is received, this call will block until the remote host
    // acknowledges the TCP control packet sent by the shutdown above.
    // Then we'll get a 0 back from recv, signalling that the remote
    // host has closed its side of the connection.
    char acReadBuffer[kBufferSize];
    while (1) {
        int nNewBytes = recv(sd, acReadBuffer, kBufferSize, 0);
        if (nNewBytes == SOCKET_ERROR) {
            return false;
        }
        else if (nNewBytes != 0) {
            cerr << endl << "FYI, received " << nNewBytes <<
                    " unexpected bytes during shutdown." << endl;
        }
        else {
            // Okay, we're done!
            break;
        }
    }

    // Close the socket.
    if (closesocket(sd) == SOCKET_ERROR) {
        return false;
    }

    return true;
}
