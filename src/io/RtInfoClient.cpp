/*=========================================================================
 *  RtInfoClient.cpp declares a class for serving info about real to clients
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

#include"RtInfoClient.h"
#include"RtExperiment.h"
#include"RtEvent.h"

#include"RtDataIDs.h"
#include"tinyxml/tinyxml.h"

// constructor with port and host
RtInfoClient::RtInfoClient(unsigned short localPortNum,
                           const string &remoteHost,
                           unsigned short remotePortNum)
    : RtServerSocket(localPortNum), RtClientSocket(remoteHost,remotePortNum) {

  addToID(":infoclient");
}

// destructor
RtInfoClient::~RtInfoClient() {

}

// add data to listen for
bool RtInfoClient::addListenData(const string &dataName,
                                 const string &roiName) {

  RtDataID templ;
  templ.setDataName(dataName);
  templ.setRoiID(roiName);

  cout << "adding data listener " << templ << endl;

  // insert
  listenData.insert(templ);

  return true;
}

// remove data from the listener
bool RtInfoClient::removeListenData(const string &dataName,
                                    const string &roiName) {
  RtDataID templ;
  templ.setDataName(dataName);
  templ.setRoiID(roiName);

  // insert
  listenData.erase(templ);

  return true;
}

// acknowledge receipt of data from the server
bool RtInfoClient::acknowledgeListenData(const string &dataName,
                                         const string &roiName,
                                         unsigned int tr) {
  cerr << "data acknowledgement is not yet implemented" << endl;

  return true;
}

// set some data
void RtInfoClient::setData(RtData *data) {
  for(set<RtDataID, RtDataIDPartialCompare>::iterator i = listenData.begin();
      i != listenData.end(); i++) {
    if(i->getDataName() == data->getDataID().getDataName()
       && i->getRoiID() == data->getDataID().getRoiID()) {
      // build an xml document to send the data
      TiXmlDocument response;
      TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "", "" );
      response.LinkEndChild(decl);
      TiXmlElement *element = NULL; //TODO this is a dummy thing
      TiXmlElement *dataEl = data->serializeAsXML(element);
      response.LinkEndChild(dataEl);

      // send the data
      sendMessageToServer(buildXMLString(response));
      cout << "infoclient sent: " << response << endl;
      break;
    }
  }
}

// receive an XML message
// in
//  string received
//  stream received on
// out XML string response
string RtInfoClient::receiveMessage(string &message, ACE_SOCK_Stream &stream) {
  cout << "infoclient received: " << message << endl;

  // parse the request
  TiXmlDocument request;
  request.Parse(message.c_str());
  if(request.Error()) {
    string errString = "could not parse request XML";
    cerr << errString << endl;
    return "";
  }
  // search for info tags
  for(TiXmlNode *info = 0; (info = request.IterateChildren("info", info)); ) {
    //// find specific tags

    // add
    for(TiXmlElement *add = 0;
        (add = (TiXmlElement*) info->IterateChildren("add", add)); ) {
      // get the dataName and roiName
      string dataName(add->Attribute("name"));
      string roiName(add->Attribute("roi"));

      // add it!
      addListenData(dataName, roiName);
    }

    // remove
    for(TiXmlElement *remove = 0;
        (remove = (TiXmlElement*) info->IterateChildren("remove", remove)); ) {
      // get the dataName and roiName
      string dataName(remove->Attribute("name"));
      string roiName(remove->Attribute("roi"));

      // remove it!
      removeListenData(dataName, roiName);
    }

    // acknowledge tags
    for(TiXmlElement *acknowledge = 0;
        (acknowledge =
         (TiXmlElement*) info->IterateChildren("acknowledge", acknowledge)); ) {
      // get the dataName and roiName
      string dataName(acknowledge->Attribute("name"));
      string roiName(acknowledge->Attribute("roi"));
      unsigned int tr = atoi(acknowledge->Attribute("tr"));

      // acknowledge it!
      acknowledgeListenData(dataName, roiName, tr);
    }
  }

  return "";
}

// build a string from an XML document
// in
//  XML document
// out
//  string representation
string RtInfoClient::buildXMLString(TiXmlDocument &doc) {
  // get string version of xml
  TiXmlPrinter printer;
  printer.SetStreamPrinting();
  doc.Accept(&printer);
  return printer.Str();
}
