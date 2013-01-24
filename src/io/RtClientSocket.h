/*=========================================================================
 *  RtClientSocket.h declares a class that implements client io operations on a
 *  socket
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

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
