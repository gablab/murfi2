/******************************************************************************
 * RtData.h declares a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

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
  virtual ~RtData();

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
  RtElementType getElementType() const { return elType; }

  // serialize the data as xml for transmission or saving to a file
  virtual TiXmlElement *serializeAsXML() = 0;

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


