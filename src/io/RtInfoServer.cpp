/******************************************************************************
 * RtInfoServer.cpp declares a class for serving info about real to clients
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-02-11
 * 
 *****************************************************************************/

#include"RtInfoServer.h"
#include"RtEvent.h"

#include"RtDataIDs.h"
#include"tinyxml/tinyxml.h"

static char *VERSION = "$Id$";

// default constructor
RtInfoServer::RtInfoServer() : RtServerSocket() {
  addToID(":infoserver");

  lastGoodTriggerTR = -1;
  lastBadTriggerTR = -1;
} 
  
// constructor with port and host
RtInfoServer::RtInfoServer(unsigned short portNum) : RtServerSocket(portNum) {
  addToID(":infoserver");  

  lastGoodTriggerTR = -1;
  lastBadTriggerTR = -1;
}

// destructor
RtInfoServer::~RtInfoServer() {

}

// set some data
// NOTE: at the moment this only takes activation sums and stores them for 
// each tr in an xml document 
void RtInfoServer::setData(RtData *data) {
  //cout << data->getID() << endl;
  if(data->getDataID().getModuleID() == ID_ACTIVATIONSUMDIFFERENCE) {
    database.push_back(data);
  }
  else if(data->getDataID().getModuleID() == ID_EVENTTRIGGER) {
    // note about "good" and "bad" triggers: these are just to denote two
    // types of triggers, one based on activation in the direction you
    // expect, and the other opposite. neither is an error
    
    if(data->getDataID().getDataName() == NAME_EVENTTRIGGER_BAD) { // bad trigger
      lastBadTriggerTR = data->getDataID().getTimePoint();
    }
    else { // otherwise its good
      lastGoodTriggerTR = data->getDataID().getTimePoint();
    }
  }
  else {
    cout << "infoserver: ignoring a " << data->getDataID() << endl;
  }
}

// receive an XML message
// in 
//  string recieved
//  stream recieved on
// out XML string response
string RtInfoServer::recieveMessage(string &message, ACE_SOCK_Stream &stream) {
  // set up the response
  TiXmlDocument response;
  TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "", "" );
  response.LinkEndChild(decl);

  // parse the request
  TiXmlDocument request;
  request.Parse(message.c_str());
  if(request.Error()) {
    string errString = "could not parse request XML";
    cerr << errString << endl;
    // put an unsupported msg in XML 
    TiXmlElement *infoResponse = new TiXmlElement("info");
    response.LinkEndChild(infoResponse);
    TiXmlElement *errEl = createErrorElement(errString);
    infoResponse->LinkEndChild(errEl);
    // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
  }  

  // search for info tags
  for(TiXmlNode *info = 0; (info = request.IterateChildren("info", info)); ) {
    // create an info node for this request
    TiXmlElement *infoResponse = new TiXmlElement("info");
    response.LinkEndChild(infoResponse);

    //// find specific tags
    // trigger tags
    for(TiXmlElement *trigger = 0; (trigger = (TiXmlElement*) info->IterateChildren("trigger", trigger)); ) {
      // create a trigger node
      TiXmlElement *triggerResponse = new TiXmlElement("trigger");
      infoResponse->LinkEndChild(triggerResponse);
      
      // check name (now must be "last")
      if(strcmp(trigger->Attribute("time"), "last")) {
	// put an unsupported msg in XML 
	TiXmlElement *errEl = createErrorElement("unsupported time (try last)");
	triggerResponse->LinkEndChild(errEl);
	// I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	continue;
      }

      // check type(good or bad, defaults to good)
      bool goodTrigger = true;
      if(!strcmp(trigger->Attribute("type"), "bad")) {
	goodTrigger = false;
      }

      triggerResponse->SetAttribute("time","last");

      // give the last trigger time of appropriate type
      if(goodTrigger) {
	triggerResponse->SetAttribute("type","good");
	triggerResponse->SetAttribute("tr",lastGoodTriggerTR);
      }
      else {
	triggerResponse->SetAttribute("type","bad");
	triggerResponse->SetAttribute("tr",lastBadTriggerTR);
      }
    }

    // roi tags
    for(TiXmlElement *roi = 0; 
	(roi = (TiXmlElement*) info->IterateChildren("roi", roi)); ) {
      // create an roi node
      TiXmlElement *roiResponse = new TiXmlElement("roi");
      infoResponse->LinkEndChild(roiResponse);
      
      // set the id
      if(roi->Attribute("id")) {
	roiResponse->SetAttribute("id", roi->Attribute("id"));
      }

      // find get tags
      for(TiXmlElement *get = 0; 
	  (get = (TiXmlElement*) roi->IterateChildren("get", get)); ) {
	// check name
	if(strcmp(get->Attribute("name"), "activation")) {
	  // put an unsupported msg in XML 
	  TiXmlElement *errEl = createErrorElement("unsupported data type");
	  roiResponse->LinkEndChild(errEl);
	  // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	  continue;
	}

	//// get tr range
	unsigned int trStart, trEnd;
	
	// check for single tr
	if(get->Attribute("tr")) {
	  // check for proper format and get tr
	  if(!strcmp(get->Attribute("tr"),"last")) {
	    trStart = database.size();
	  }
	  else if(!RtConfigVal::convert<unsigned int>(trStart,
						 get->Attribute("tr"))) {
	    string errString = "error converting tr attribute to integer";
	    cerr << errString << endl;
	    TiXmlElement *errEl = createErrorElement(errString);
	    roiResponse->LinkEndChild(errEl);
	    // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	    continue;	   
	  }
	  
	  // just set trEnd to the same as start
	  trEnd = trStart;
	}
	else if(get->Attribute("trStart")) {
	  // check for proper format and get trStart
	  if(!strcmp(get->Attribute("trStart"),"first")) {
	    trStart = 1;
	  }
	  else if(!RtConfigVal::convert<unsigned int>(trStart,
						 get->Attribute("trStart"))) {
	    string errString = "error converting trStart attribute to integer";
	    cerr << errString << endl;
	    TiXmlElement *errEl = createErrorElement(errString);
	    roiResponse->LinkEndChild(errEl);
	    // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	    continue;	   
	  }

	  // check for proper format and get trEnd
	  if(!strcmp(get->Attribute("trEnd"),"last")) {
	    trEnd = database.size();
	  }
	  else if(!RtConfigVal::convert<unsigned int>(trEnd,
						 get->Attribute("trEnd"))) {
	    string errString = "error converting trEnd attribute to integer";
	    cerr << errString << endl;
	    TiXmlElement *errEl = createErrorElement(errString);
	    roiResponse->LinkEndChild(errEl);
	    // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	    continue;	   
	  }

	}
	  
	cout << trStart << " trend=" << trEnd << " database size=" << database.size() << endl; 

	// check range (fix this when database is better)
	if(trStart < 1 || trEnd > database.size()) {
	  string errString = "tr range requested is invalid";
	  cerr << errString << endl;
	  TiXmlElement *errEl = createErrorElement(errString);
	  roiResponse->LinkEndChild(errEl);
	  // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	  continue;
	}
	  
	// get the data as xml (zero vs one based)
	for(unsigned int tr=trStart; tr <= trEnd; tr++) {
	  TiXmlElement *trel = database[tr-1]->serializeAsXML();
	  trel->SetAttribute("tr",tr);
	  roiResponse->LinkEndChild(trel);
	  // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	}
      }      
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
  errEl->SetAttribute("name","error");
  errEl->SetAttribute("type",error);
  return errEl; 
}

// get the version
//  out: char array that represents the cvs version
char *RtInfoServer::getVersionString() {
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


