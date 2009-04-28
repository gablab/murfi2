/******************************************************************************
 * RtDataListener.h declares a class that provides an interface for
 * data availability notification
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-18
 *
 *****************************************************************************/
#ifndef RTDATALISTENER_H
#define RTDATALISTENER_H

#include"RtDataID.h"

// pure virtual listener class 
// TODO: impement mechanism to selectively notify (only certain data)
class RtDataListener {

public:

  RtDataListener() {};
  virtual ~RtDataListener() {};

  // entry function for notification that new data is ready
  virtual void notify(const RtDataID &dataID) = 0;
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

