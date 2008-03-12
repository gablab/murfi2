/******************************************************************************
 * RtData.h declares a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTDATA_H
#define RTDATA_H

#include"ace/Date_Time.h"
#include<string>
#include"tinyxml/tinyxml.h"

#include"RtLocker.h"

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
  const string &getID();

  // get the roiID
  string getRoiID();

  // append to the id string for this data
  //  in
  //   string to append to the id
  void addToID(string &s);

  // append to the id string for this data
  //  in
  //   string to append to the id
  void addToID(char *s);

  // set the id string
  void setID(const string &s);

  // set the roiID
  void setRoiID(const string &id);

  // get if we should delete this data after stream processing (not persistent)
  bool getPersistent();

  // get if we should delete this data after stream processing (not persistent)
  void setPersistent(bool p);

  // get the number of elements in this datatype
  unsigned int getNumEl();

  // try to obtain a lock for this data
  //  out
  //   if lock was obtained
  bool lockData(RtLocker *locker);

  // release a lock on this data
  //  out
  //   if lock was released or if data was not locked
  bool unlockData(RtLocker *locker);

  // serialize the data as xml for transmission or saving to a file
  virtual TiXmlElement *serializeAsXML() = 0;

  // get the creation time
  ACE_Date_Time getCreationTime();

protected:

  //*** data members  ***//

  // ids
  string id;     // data type id
  string roiID;  // roi id
  
  ACE_Date_Time creationTime;

  bool persistent;

  // pointer to a member with a lock on this data
  // NOTE: locks only prevent write and delete operations. 
  // reads are still possible
  RtLocker *lock; 

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


