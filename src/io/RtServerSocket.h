/*=========================================================================
 *  RtServerSocket.h declares a class that implements server io operations on a
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

#ifndef RTSERVERSOCKET_H
#define RTSERVERSOCKET_H

#include"RtOutput.h"
#include"ace/Task.h"
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"

using namespace std;


// acceptor to allow connections from the scanner

class RtSocketAcceptor : public ACE_SOCK_Acceptor {
 public:

  RtSocketAcceptor() : isOpen(false) {
  }

  // called when connection is closed

  int close() {
    isOpen = false;
    return ACE_SOCK_Acceptor::close();
  }

  // accept a new connection
  // just pass all the arguments along and set isOpen true

  int accept(ACE_SOCK_Stream &new_stream, ACE_Addr *remote_addr = 0,
             ACE_Time_Value *timeout = 0, int restart = 1,
             int reset_new_handle = 0) {
    int ret = ACE_SOCK_Acceptor::accept(new_stream, remote_addr, timeout,
                                        restart, reset_new_handle = 0);
    isOpen = ret == 0;
    return ret;
  }

  // keep track if we are currently open
  bool isOpen;
};



// class declaration

class RtServerSocket : public ACE_Task_Base, public RtOutput {
 public:

  //*** constructors/destructors  ***//

  // constructor with port and host
  RtServerSocket(unsigned short portNum);

  // destructor
  virtual ~RtServerSocket();

  // open and start accepting
  virtual bool open(RtConfig &config);

  // close and clean up
  virtual bool close();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

 protected:

  // receive a message
  // in
  //    string received
  //    stream received on
  // out  string response
  virtual string receiveMessage(string &message, ACE_SOCK_Stream &stream);

  // send a message to a client
  // in
  //  message to send
  //  stream to send on
  // out
  //  success or failure
  virtual bool sendMessage(const string &message, ACE_SOCK_Stream &stream);

  // run the scanner input
  virtual int svc();

  // client address
  ACE_INET_Addr address;

  // data transfer object
  ACE_SOCK_Stream stream;

  // connector
  RtSocketAcceptor acceptor;

  // termination character
  char messageTerminationChar;

 private:

  // default constructor
  RtServerSocket();

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
