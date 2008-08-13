/******************************************************************************
 * RtDataStore.h declares a class for data storage and retreival
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 * 
 *****************************************************************************/
// not ready for prime time
#ifdef 0

#ifndef RTDATASTORE_H
#define RTDATASTORE_H

#include"RtOutput.h"
#include<vector>
using namespace std;

// class declaration
class RtDataStore : public RtOutput  {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtDataStore(); 
  
  // destructor
  virtual ~RtDataStore();

  // hand off some data to be output
  virtual void setData(RtData *data);

  // add an output to be notified when new data arrives
  virtual void addOutputForNotify(const RtOutput *out);

  //*** methods for retreiving data ***//
  
  // get data by id
  virtual RtData *getData(RtDataID id);

  // gets the id for a data object
  virtual RtDataID getDataID(const RtData &data); 

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // list of outputs to be notified when new data arrives
  vector<RtOutput*> outputNotifyList;
};

#endif

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


