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

  // get the roiID
  //  string getRoiID() const;

  // append to the id string for this data
  //  in
  //   string to append to the id
//  void addToID(string &s);
//
//  // append to the id string for this data
//  //  in
//  //   string to append to the id
//  void addToID(char *s);

  // set the id string
  void setDataID(const RtDataID &id);

  // set the roiID
  //  void setRoiID(const string &id);

  // get if we should delete this data after stream processing (not persistent)
  bool getPersistent() const;

  // get if we should delete this data after stream processing (not persistent)
  void setPersistent(bool p);

  // get the number of elements in this datatype
  unsigned int getNumEl() const;

  // serialize the data as xml for transmission or saving to a file
  virtual TiXmlElement *serializeAsXML() = 0;

  // get the creation time
  //ACE_Date_Time getCreationTime() const;

protected:

  //*** data members  ***//

  // old id stuff
  //string dataID;     // data type id
  //string roiID;  // roi id
  //
  //ACE_Date_Time creationTime;

  RtDataID dataID;

  bool persistent;

  // number of distinct elements of data 
  unsigned int numEl;

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


