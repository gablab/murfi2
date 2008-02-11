/******************************************************************************
 * RtInfoServer.cpp declares a class for serving info about real to clients
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-02-11
 * 
 *****************************************************************************/

#include"RtInfoServer.h"

#include"RtDataIDs.h"
#include"tinyxml/tinyxml.h"

static char *VERSION = "$Id$";

// default constructor
RtInfoServer::RtInfoServer() : RtOutput(), RtServerSocket() {
  
} 
  
// constructor with port and host
RtInfoServer::RtInfoServer(unsigned short portNum, string hostName) 
  : RtOutput(), RtServerSocket(portNum, hostName) {
  
}

// destructor
RtInfoServer::~RtInfoServer() {

}

// set some data
// NOTE: at the moment this only takes activation sums and stores them for 
// each tr in an xml document 
void RtInfoServer::setData(RtData *data) {
  if(data->getID() == ID_ACTIVATIONSUM) {
    cout << "adding an activation sum" << endl;
    database.push_back(data);
  }
  else {
    cout << "ignoring a " << data->getID() << endl;
  }
}

// receive an XML message
// in 
//  string recieved
//  stream recieved on
// out XML string response
string RtInfoServer::recieveMessage(string &message, ACE_SOCK_Stream &stream) {
  // parse the string
  TiXmlDocument request;
  request.Parse(message.c_str());
  if(request.Error()) {
    cerr << "couldn't parse request XML" << endl;
  }

  // set up the response
  TiXmlDocument response;
  TiXmlDeclaration decl( "1.0", "", "" );
  response.LinkEndChild(&decl);

  // search for info tags
  for(TiXmlNode *info = 0; (info = request.IterateChildren("info", info)); ) {
    // create an info node for this request
    TiXmlElement infoResponse("info");
    response.LinkEndChild(&infoResponse);

    // find roi tags
    for(TiXmlElement *roi = 0; 
	(roi = (TiXmlElement*) info->IterateChildren("roi", roi)); ) {
      // create an roi node
      TiXmlElement roiResponse("roi");
      response.LinkEndChild(&infoResponse);
      
      // set the id
      if(roi->Attribute("id")) {
	roiResponse.SetAttribute("id", roi->Attribute("id"));
      }      

      // find get tags
      for(TiXmlElement *get = 0; 
	  (get = (TiXmlElement*) roi->IterateChildren("get", get)); ) {
	// check name
	if(get->Attribute("name") != "activation") {
	  // put an unsupported msg in XML 
	  TiXmlElement *unsupported = new TiXmlElement("data");
	  unsupported->SetAttribute("name","unsupported");
	  roi->LinkEndChild(unsupported);
	  // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
	  continue;
	}

	//// get tr range
	unsigned int trStart, trEnd;
	
	// check for single tr
	if(get->Attribute("tr")) {
	  // check for proper format and get tr
	  if(!RtConfigVal::convert<unsigned int>(trStart,
						 get->Attribute("tr"))) {
	    cerr << "error converting tr attribute to integer" << endl;
	    continue;	   
	  }
	  
	  // just set trEnd to the same as start
	  trEnd = trStart;
	}
	else if(get->Attribute("trstart")) {
	  // check for proper format and get trStart
	  if(!RtConfigVal::convert<unsigned int>(trStart,
						 get->Attribute("trStart"))) {
	    cerr << "error converting trStart attribute to integer" << endl;
	    continue;	   
	  }

	  // check for proper format and get trEnd
	  if(!RtConfigVal::convert<unsigned int>(trEnd,
						 get->Attribute("trEnd"))) {
	    cerr << "error converting trEnd attribute to integer" << endl;
	    continue;	   
	  }

	}
	  
	// check range (fix this when database is better)
	if(trStart < 1 || trEnd > database.size()) {
	  cerr << "tr range requested (" << trStart << "-" << trEnd 
	       << ") is invalid" << endl;
	  continue;
	}
	  
	// get the data as xml (zero vs one based)
	for(unsigned int tr=trStart; tr <= trEnd; tr++) {
	  TiXmlElement *trel = database[tr-1]->serializeAsXML();
	  trel->SetAttribute("tr",tr);
	  roi->LinkEndChild(trel);
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


