/*=========================================================================
 *  RtData.h declares a base class for data classes
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

#ifndef RTDATA_H
#define RTDATA_H

#include<dirent.h>
#include"ace/Date_Time.h"
#include "ace/Log_Msg.h"

#include"RtDataID.h"

#include<string>
#include"tinyxml/tinyxml.h"

#include"RtTypes.h"

//#include"RtLocker.h"

using namespace std;

// class declaration

class RtData {
 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtData();

  // destructor
  ~RtData();

  // gets and sets

  // gets the id string for this data
  //  out
  //   id string
  RtDataID &getDataID();

  // gets the id string for this data without modification
  //  out
  //   id string
  RtDataID getDataID() const;

  // set the id string
  void setDataID(const RtDataID &id);

  // get the number of elements in this datatype
  unsigned int getNumEl() const;

  // get the underlying datatype this data stores

  RtElementType getElementType() const {
    return elType;
  }

  // serialize the data as xml for transmission or saving to a file
  virtual TiXmlElement *serializeAsXML(TiXmlElement *requestElement) = 0;

  // unserialize the xml data
  virtual void unserializeXML(TiXmlElement* element) = 0;

  // get the creation time
  //ACE_Date_Time getCreationTime() const;

 protected:

  //*** data members  ***//

  RtDataID dataID;

  // number of distinct elements of data
  unsigned int numEl;

  RtElementType elType;

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
