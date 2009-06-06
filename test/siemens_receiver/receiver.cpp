/******************************************************************************
 * program to save images sent from a scanner using the external
 * sender ice functor
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-31
 *****************************************************************************/

#include"ExternalSenderImageInfo.h"

#include <winsock.h>

#include <cmath>
#include <iostream>
using namespace std;


// defaults
static const int defaultPort = 15000;


// prototypes
int runReceiverWin(int port);
SOCKET getListenerWin(int port);
SOCKET acceptWin(SOCKET sock, sockaddr_in& remote);
ExternalImageInfo *readImageInfoWin(SOCKET sock);
short *readImageDataWin(SOCKET sock, const ExternalImageInfo &info);
int closeWin(SOCKET sock);


// controller
int main(int argc, char** argv) {
  
  // get port if specified
  int port = defaultPort;
  if(argc > 1) {
    port = atoi(argv[1]);
  }

  cout << "trying to start scanner listener on port " << port << endl;
  runReceiverWin(port);
  cout << "done" << endl;

  return 0;
}


// listens for incomming images on a port until error
// out: 0 for sucess, 1 for error
int runReceiverWin(int port) {
  // start winsock
  WSAData dat;
  if(0 != WSAStartup(MAKEWORD(1,1),&dat)) {
    cout << "error starting winsock" << endl;
  }

  cout << "starting the receiver...";
  SOCKET sock = getListenerWin(htons(port));
  if(sock == INVALID_SOCKET) {
    cout << "failed" << endl;
    return 1;
  }
  cout << "success" << endl;

  while(1) {
    cout << "listening..." << endl << flush;
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

    // receive and build header
    ExternalImageInfo *info = readImageInfoWin(sd);

    // receive and build data
    short *data = readImageDataWin(sd,*info);

    // error check 
    if(info == NULL || data == NULL) {
      cout << "error receiving image, exit" << endl;
      break;
    }
    
    // close connection
    closeWin(sd);
  }

  return 0;
}

// get a new socket on a local port to listen on
// out: SOCKET on sucess, INVALID_SOCKET on error
SOCKET getListenerWin(int port) {
  u_long addr = inet_addr("0.0.0.0");
  SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd != INVALID_SOCKET) {
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = addr;
    sin.sin_port = port;
    if(bind(sd, (sockaddr*)&sin, sizeof(sockaddr_in)) != SOCKET_ERROR) {
      listen(sd, 1);
      return sd;
    }
  }

  return INVALID_SOCKET;
}

// accept an incoming connection
// out: return value of accept()
SOCKET acceptWin(SOCKET sock, sockaddr_in& remote) {
  int nAddrSize = sizeof(remote);
  return accept(sock, (sockaddr*)&remote, &nAddrSize);
}

// read an image header from the socket
//  in
//   sock: a socket to receive on
//  out
//   image info struct on successful read (NULL otherwise)
ExternalImageInfo *readImageInfoWin(SOCKET sock) {
  char buffer[EXTERNALSENDERSIZEOF];
  int rec;

  // read header
  for(rec = 0; rec < EXTERNALSENDERSIZEOF && rec != SOCKET_ERROR; 
      rec += recv(sock, buffer+rec, EXTERNALSENDERSIZEOF-rec, 0));

  // error check
  if(rec != EXTERNALSENDERSIZEOF) {
    cout << "error receiving header" << endl;
    return NULL;
  }

  cout << "received header" << endl;
  ExternalImageInfo *info = new ExternalImageInfo(buffer, rec);

  return info;
}

// read an image info from a socket
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//   info:   the last read image info struct
//  out
//   image data on successful read (NULL otherwise)
short *readImageDataWin(SOCKET sock, const ExternalImageInfo &info) {
  cout << "receiving image " << info.iAcquisitionNumber << endl;

  int numPix 
    = (int) pow((double)info.iMosaicGridSize,2)*info.nLin*info.nCol;
  char *buffer = new char[numPix]();
  int rec;

  // read image data
  for(rec = 0; rec < numPix*sizeof(short) && rec != SOCKET_ERROR;
      rec += recv(sock, buffer+rec, numPix*sizeof(short)-rec,0));

  // error check
  if(rec != numPix*sizeof(short)) {
    cout << "error receiving image data" << endl;
    return NULL;
  }

  short *img = new short[numPix];
  memcpy(img,buffer,numPix*sizeof(short));

  delete [] buffer;

  return img;
}

// gracefully close a socket
int closeWin(SOCKET sock) {
  if(shutdown(sock, 1) == SOCKET_ERROR) {
    return 1;
  }

  // clear remaining data
  char buffer[1024];
  int num = 1;
  while(0 != (num = recv(sock, buffer, 1024, 0)) && num != SOCKET_ERROR);

  if(num == SOCKET_ERROR) {
    return 1;
  }

  if(closesocket(sock) == SOCKET_ERROR) {
    return 1;
  }

  return 0;
}
