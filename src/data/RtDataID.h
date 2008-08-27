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
  virtual ~RtDataID();
  
  //*** operators ***//
  
  // comparison
  bool operator==(const RtDataID &other);

  // less
  bool operator<(const RtDataID &other);

  // get a string version of the ID
  string toString();

  // set the ID from a string
  string setFromString(const string &id);

protected:

  // fields
  unsigned int siteIDNum;        // id of study site
  unsigned int studyNum;         // study number (date and time study started)
  unsigned int seriesNum;        // image series (count up from 1)
  unsigned int timePoint;        // image number (count up from 1)
  RtDataID     inputDataID;      // id of data that served as input here
  string       processModuleID;  // id of this processing module
  string       dataID;           // id representing the particular data kind 
  string       roiID;            // if this was processed on a particular ROI

};

// comparison class for storing ids in a map (see RtDataStore.h) 
class RtDataIDCompare {
public:
  bool operator()(const RtDataID* one, const RtDataID* two) const {
    return (*one) < (*two);
  }
};

#endif

#endif
