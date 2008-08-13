/******************************************************************************
 * RtDataID.h declares a class for identifying data uniquely
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 * 
 *****************************************************************************/

// not ready for prime time
#ifdef 0

#ifndef RTDATAID_H
#define RTDATAID_H

class RtDataID {
  
public:

  RtDataID();
  ~RtDataID();
  
  //*** operators ***//
  
  // comparison
  bool operator==(const RtDataID &other);

  // get a string version of the ID
  string toString();

protected:

  // fields
  unsigned int timePoint;
  RtDataID     inputDataID;
  string       roiID;
  string       processModuleID;

};

#endif

#endif
