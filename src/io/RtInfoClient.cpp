/******************************************************************************
 * RtInfoClient.cpp declares a class for serving info about real to clients
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-02-11
 *
 *****************************************************************************/

#include"RtInfoClient.h"
#include"RtExperiment.h"
#include"RtEvent.h"

#include"RtDataIDs.h"
#include"tinyxml/tinyxml.h"


static char *VERSION = "$Id: RtInfoServer.cpp 415 2009-02-11 22:20:24Z ohinds $";

//// default constructor
//RtInfoClient::RtInfoClient() : RtServerSocket(), RtClientSocket() {
//  addToID(":infoclient");
//}

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

  cout << "adding " << templ << endl;

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
//  cout << "found " << data->getDataID() << endl
//       << "looking for " << (*listenData.begin()) << endl
//       << "==? " << (data->getDataID() == (*listenData.begin())) << endl;


    set<RtDataID, RtDataIDPartialCompare>::iterator i = listenData.find(data->getDataID());
  if(i != listenData.end()) { // find the appropriate data in the listened

    // build an xml document to send the data
    TiXmlDocument response;
    TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "", "" );
    response.LinkEndChild(decl);
    TiXmlElement *infoEl = new TiXmlElement("info");
    response.LinkEndChild(infoEl);

    TiXmlElement *element = NULL; //TODO this is a dummy thing
    TiXmlElement *dataEl = data->serializeAsXML(element);
    infoEl->LinkEndChild(dataEl);

    // send the data
    sendMessageToServer(buildXMLString(response));
  }
  else {
    cout << "infoclient: ignoring a " << data->getDataID() << endl;
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
    for(TiXmlElement *add = 0; (add = (TiXmlElement*) info->IterateChildren("add", add)); ) {
      // get the dataName and roiName
      string dataName(add->Attribute("name"));
      string roiName(add->Attribute("roi"));

      // add it!
      addListenData(dataName, roiName);
    }

    // remove
    for(TiXmlElement *remove = 0; (remove = (TiXmlElement*) info->IterateChildren("remove", remove)); ) {
      // get the dataName and roiName
      string dataName(remove->Attribute("name"));
      string roiName(remove->Attribute("roi"));

      // remove it!
      removeListenData(dataName, roiName);
    }

    // acknowledge tags
    for(TiXmlElement *acknowledge = 0; (acknowledge = (TiXmlElement*) info->IterateChildren("acknowledge", acknowledge)); ) {
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

// get the version
//  out: char array that represents the cvs version
char *RtInfoClient::getVersionString() {
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
