/*=========================================================================
 *  RtClientSocket.cpp defines a class that implements client io operations on a
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


