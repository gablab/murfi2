/*=========================================================================
 *  RtInfoSever.h declares a class for serving info about real to clients
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

#ifndef RTINFOSERVER_H
#define RTINFOSERVER_H

#include"RtServerSocket.h"
#include<vector>
using namespace std;

// class declaration

class RtInfoServer : public RtServerSocket {
public:

    //*** constructors/destructors  ***//

    // default constructor
    RtInfoServer();

    // constructor with port and host
    RtInfoServer(unsigned short portNum);

    // destructor
    virtual ~RtInfoServer();

    // open and start accepting
    virtual bool open(RtConfig &config);

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

    // build an error element
    // in
    //  name of the error
    // out
    //  XML element containing error info
    TiXmlElement *createErrorElement(string error);

    // database
    // NOTE: now this only stores activation sums by tr.
    // TODO: reconfigure this to access the data store
    RtData** posAct;
    RtData** negAct;

    unsigned int numPos;
    unsigned int numNeg;

    // the last good trigger time
    int lastGoodTriggerTR;

    // the last bad trigger time
    int lastBadTriggerTR;

private:

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


