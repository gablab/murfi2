/******************************************************************************
 * RtInfoSever.h declares a class for serving info about real to clients
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-02-11
 * 
 *****************************************************************************/

#ifndef RTINFOSERVER_H
#define RTINFOSERVER_H

#include"RtServerSocket.h"
#include<vector>
using namespace std;

// class declaration
class RtInfoServer : public RtServerSocket  {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInfoServer(); 
  
  // constructor with port and host
  RtInfoServer(unsigned short portNum); 

  // destructor
  virtual ~RtInfoServer();

  // hand of some data to be output
  virtual void setData(RtData *data);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // receive a message
  // in 
  //  string recieved
  //  stream recieved on
  // out  success or failure 
  virtual string recieveMessage(string &message, ACE_SOCK_Stream &stream);

  // build a string from an XML document
  // in
  //  XML document 
  // out
  //  string representation
  string buildXMLString(TiXmlDocument &doc);

  // build an error element
  // in
  //  name of the error
  // out
  //  XML element containing error info
  TiXmlElement *createErrorElement(string error);

  // database
  // NOTE: now this only stores activation sums by tr.
  // TODO: reconsider how to store old data and manage access to it
  vector<RtData*> database;

  // the last good trigger time
  int lastGoodTriggerTR;

  // the last bad trigger time
  int lastBadTriggerTR;
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


