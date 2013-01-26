/*=========================================================================
 *  RtOutputSocket.h declares a class that implements output operations on a
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

  // output data to the socket
  void setData(RtData *data);

  // close and clean up
  bool close();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


 protected:

  // client address
  ACE_INET_Addr address;

  // data transfer object
  ACE_SOCK_Stream stream;

  // connector
  ACE_SOCK_Connector connector;

};

#endif
