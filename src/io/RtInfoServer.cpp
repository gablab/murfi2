/*=========================================================================
 *  RtInfoServer.cpp declares a class for serving info about real to clients
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

#include"RtInfoServer.h"
#include"RtExperiment.h"
#include"RtEvent.h"
#include "RtDataStore.h"
#include "RtDesignMatrix.h"

#include"RtDataIDs.h"
#include"tinyxml/tinyxml.h"
#include<vnl/vnl_matrix.h>

static char *VERSION = "$Id$";

const unsigned short DEFAULT_PORT_NUM = 15001;

// default constructor
RtInfoServer::RtInfoServer() : RtServerSocket(DEFAULT_PORT_NUM) {
  addToID(":infoserver");

  lastGoodTriggerTR = -1;
  lastBadTriggerTR = -1;

  posAct = new RtData*[1024];
  negAct = new RtData*[1024];
}

// constructor with port and host

RtInfoServer::RtInfoServer(unsigned short portNum) : RtServerSocket(portNum) {
  addToID(":infoserver");

  lastGoodTriggerTR = -1;
  lastBadTriggerTR = -1;

  posAct = new RtData*[1024];
  negAct = new RtData*[1024];
}

// destructor

RtInfoServer::~RtInfoServer() {
  delete [] posAct;
}

// open and start accepting
bool RtInfoServer::open(RtConfig &config) {
  // find port
  if(config.isSet("infoserver:port")) {
    address.set_port_number((unsigned short) config.get("infoserver:port"));
  }

  numPos = 1;
  numNeg = 1;

  return RtServerSocket::open(config);
}

// set some data
// NOTE: at the moment this only takes activation sums and stores them for
// each tr in an xml document

void RtInfoServer::setData(RtData *data) {

  // note about "good" and "bad" triggers: these are just to denote two
  // types of triggers, one based on activation in the direction you
  // expect, and the other opposite. neither is an error
  if(data->getDataID().getRoiID() ==
     getExperimentConfig().get("infoserver:posActivationDataID").str()) {
    posAct[data->getDataID().getTimePoint()] = data;
    numPos++;
  }
  else if(data->getDataID().getRoiID() ==
          getExperimentConfig().get("infoserver:negActivationDataID").str()) {
    negAct[data->getDataID().getTimePoint()] = data;
    numNeg++;
  }
  else if (data->getDataID().getDataName() == NAME_EVENTTRIGGER_BAD) {
    lastBadTriggerTR = data->getDataID().getTimePoint();
  }
  else if (data->getDataID().getDataName() == NAME_EVENTTRIGGER_GOOD) {
    lastGoodTriggerTR = data->getDataID().getTimePoint();
  }
  else {
    cout << "infoserver: ignoring a " << data->getDataID() << endl;
  }
}

// receive an XML message
// in
//  string received
//  stream received on
// out XML string response

string RtInfoServer::receiveMessage(string &message, ACE_SOCK_Stream &stream) {
  cout << message << endl;

  // set up the response
  TiXmlDocument response;
  TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
  response.LinkEndChild(decl);

  // parse the request
  TiXmlDocument request;
  request.Parse(message.c_str());
  if (request.Error()) {
    string errString = "could not parse request XML";
    cerr << errString << endl;
    // put an unsupported msg in XML
    TiXmlElement *infoResponse = new TiXmlElement("info");
    response.LinkEndChild(infoResponse);
    TiXmlElement *errEl = createErrorElement(errString);
    infoResponse->LinkEndChild(errEl);
  }

  for (TiXmlNode *info = 0; (info = request.IterateChildren("info", info));) {
    // create an info node for this request
    TiXmlElement *infoResponse = new TiXmlElement("info");
    response.LinkEndChild(infoResponse);

    // look for gets
    for (TiXmlElement *get = 0;
         (get = (TiXmlElement*) info->IterateChildren("get", get));) {
      // TODO figure out a way to check the data id string?

      // create data id object from input attribute
      string dataIDString = get->Attribute("dataid");
      RtDataID dataID;
      dataID.setFromString(dataIDString);

      // retrieve from data store using data id from input string
      RtData *retrievedData = getDataStore().getData(dataID);

      // check for NULL
      if (retrievedData == NULL) {
        cerr << "Data from data store is NULL." << endl;
        continue;
      }

      // call serializeAsXML for retrievedData class with the element from
      // which it came
      TiXmlElement *dataElement = retrievedData->serializeAsXML(get);

      // set data id attribute and link to inforesponse
      dataElement->SetAttribute("dataid",
                                retrievedData->getDataID().toString());
      infoResponse->LinkEndChild(dataElement);
    }

    // look for sets
    for (TiXmlElement *set = 0;
         (set = (TiXmlElement*) info->IterateChildren("set", set));) {
      // TODO figure out a way to check the data id string?

      // create data id object from input attribute
      string dataIDString = set->Attribute("dataid");
      RtDataID dataID;
      dataID.setFromString(dataIDString);

      // retrieve from data store using data id from input string
      RtData *retrievedData = getDataStore().getData(dataID);

      // check for NULL
      if (retrievedData == NULL) {
        cerr << "Data from data store is NULL." << endl;
        continue;
      }

      // call unserializeXML for retrievedData class
      retrievedData->unserializeXML(set);
    }
  }
  return buildXMLString(response);
}

// build a string from an XML document
// in
//  XML document
// out
//  string representation

string RtInfoServer::buildXMLString(TiXmlDocument &doc) {
  // get string version of xml
  TiXmlPrinter printer;
  printer.SetStreamPrinting();
  doc.Accept(&printer);
  return printer.Str();
}

// build an error element
// in
//  name of the error
// out
//  XML element containing error info

TiXmlElement *RtInfoServer::createErrorElement(string error) {
  TiXmlElement *errEl = new TiXmlElement("data");
  errEl->SetAttribute("name", "error");
  errEl->SetAttribute("type", error);
  return errEl;
}

// get the version
//  out: char array that represents the cvs version

char *RtInfoServer::getVersionString() {
  return VERSION;
}
