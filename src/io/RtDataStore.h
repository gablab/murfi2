/******************************************************************************
 * RtDataStore.h declares a class for data storage and retreival
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/
#ifndef RTDATASTORE_H
#define RTDATASTORE_H

#include"RtDataID.h"
#include"RtData.h"
#include"ace/Mutex.h"

#include"RtOutput.h"
#include<vector>
#include<map>
#include<set>
using namespace std;

// class declaration
class RtDataStore : public RtOutput  {
#if USE_FRONTEND // Need to access stuff from frontend
  friend class FrDataStore;
#endif

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtDataStore();

  // destructor
  virtual ~RtDataStore();

  // add an output to be notified when new data arrives
  virtual void addOutputForNotify(RtOutput *out);

  //*** data methods ***//

  // hand off some data to be output
  virtual void setData(RtData *data);

  // get data by id
  virtual RtData *getData(RtDataID &id);

  // get data by indexing
  virtual RtData *getDataFast(RtDataID &id);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

  // get a list of the available data in the data store
  // note: this returns a list of the unique data ids excepting timepoint. 
  set<RtDataID>::const_iterator getAvailableData();

  protected:

  // list of outputs to be notified when new data arrives
  vector<RtOutput*> outputNotifyList;

  // hash map to store pointers to acquired data
  map<RtDataID,RtData*,RtDataIDCompare> store;

  // list of the data ids we currently have in the data 
  // store (excepting timepoiint)
  set<RtDataID> availableData;

  // set data ids to availableData
  void setAvailableData(RtDataID);

  // create mutex for datastore lockdown
  ACE_Mutex mut;

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


