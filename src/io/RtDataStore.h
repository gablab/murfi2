/******************************************************************************
 * RtDataStore.h declares a class for data storage and retreival
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/
// not ready for prime time
#if 0

#ifndef RTDATASTORE_H
#define RTDATASTORE_H

#include"RtDataID.h"
#include"RtData.h"

#include"RtOutput.h"
#include<vector>
#include<map>
using namespace std;

// class declaration
class RtDataStore : public RtOutput  {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtDataStore();

  // destructor
  virtual ~RtDataStore();

  // add an output to be notified when new data arrives
  virtual void addOutputForNotify(const RtOutput *out);

  //*** data methods ***//

  // hand off some data to be output
  virtual void setData(RtData *data);

  // get data by id
  virtual RtData *getData(const RtDataID &id);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // list of outputs to be notified when new data arrives
  vector<RtOutput*> outputNotifyList;

  // hash map to store pointers to acquired data
  map<RtDataID*,RtData*,RtDataIDCompare> store;
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


