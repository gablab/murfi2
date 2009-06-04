/******************************************************************************
 * RtInfoClient.h declares a class that sends new data to a remote server
 * 
 * Oliver Hinds <ohinds@mit.edu> 2009-05-05
 * 
 *****************************************************************************/

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


