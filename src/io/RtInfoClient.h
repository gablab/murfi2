/*=========================================================================
 *  RtInfoClient.h declares a class that sends new data to a remote server
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

#ifndef RTINFOCLIENT_H
#define RTINFOCLIENT_H

#include"RtServerSocket.h"
#include"RtClientSocket.h"
#include"RtOutput.h"

#include<set>

using namespace std;

// class declaration
class RtInfoClient : public RtServerSocket, RtClientSocket  {

public:

  //*** constructors/destructors  ***//
  
  // constructor with port and host
  explicit RtInfoClient(unsigned short localPortNum, 
			const string &remoteHost, 
			unsigned short remotePortNum);

  // destructor
  virtual ~RtInfoClient();

  // add data to listen for
  bool addListenData(const string &dataName, const string &roiName);

  // remove data from the listener
  bool removeListenData(const string &dataName, const string &roiName);

  // acknowledge receipt of data from the server
  bool acknowledgeListenData(const string &dataName, const string &roiName, unsigned int tr);

  // hand of some data to be output
  virtual void setData(RtData *data);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // receive a message
  // in 
  //  string received
  //  stream received on
  // out  success or failure 
  virtual string receiveMessage(string &message, ACE_SOCK_Stream &stream);

  // build a string from an XML document
  // in
  //  XML document 
  // out
  //  string representation
  string buildXMLString(TiXmlDocument &doc);

  // data

  // list of dataids to listen for
  set<RtDataID,RtDataIDPartialCompare> listenData;

private:
  RtInfoClient();   

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


