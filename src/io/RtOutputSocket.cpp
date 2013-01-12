/*=========================================================================
 *  RtOutputsocket.cpp defines a class that implements output operations
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

static char *VERSION = "$Id$";


#include"RtOutputSocket.h"
#include"tinyxml/tinyxml.h"

#include"ace/Mutex.h"

#define DEFAULT_PORT 15001
#define DEFAULT_HOST "localhost"

// default constructor
RtOutputSocket::RtOutputSocket() : RtOutput() {
  addToID(":socket");
}

// destructor
RtOutputSocket::~RtOutputSocket() {
}

// open and start accepting input
bool RtOutputSocket::open(RtConfig &config) {
  unsigned short port = DEFAULT_PORT;
  string host = DEFAULT_HOST;

  if(!RtOutput::open(config)) {
    return false;
  }

  // see if host info has been provided
  if(config.isSet("port")) {
    port = (unsigned short) config.get("port");
  }

  if(config.isSet("host")) {
    host = config.get("host").str();
  }
  
  cout << "opened output socket: host " << host << ":" << port << endl;

  // setup the socket address
  address.set(port, host.c_str());

  return true;
}

// close and clean up
bool RtOutputSocket::close() {

  // close stream and socket

  isOpen = false;

  return true;
}

// output data to the socket
void RtOutputSocket::setData(RtData *data) {
  // get a serialized version of this data as xml
  TiXmlDocument serializedDataDoc;
  TiXmlDeclaration decl( "1.0", "", "" );
  serializedDataDoc.LinkEndChild(&decl);

  TiXmlElement *element = NULL; //TODO this is a dummy thing
  TiXmlElement *serializedData = data->serializeAsXML(element);
  if(serializedData == NULL) {
    cerr << "couldn't serialize data, not sending" << endl;    
    return;
  }

  serializedDataDoc.LinkEndChild(serializedData);

  // conect to server (listener)
  if(!connector.connect(stream, address)) {
    cerr << "couldn't connect to server, not sending data" << endl;
    return;
  }

  // get string version of xml
  TiXmlPrinter printer;
  printer.SetStreamPrinting();
  serializedDataDoc.Accept(&printer);
  char *serializedStr = new char[printer.Size()+1]; 
  strcpy(serializedStr, printer.CStr());

  // lock the socket
  ACE_Mutex mutx;
  mutx.acquire();  

  cout << "connected to server, sending data " << endl << serializedStr;

  // send it
  stream.send_n(serializedStr,strlen(serializedStr));

  mutx.release();

  delete [] serializedStr;
  delete serializedData;  // ??
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


