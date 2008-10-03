/******************************************************************************
 * RtDataID.h declares a class for identifying data uniquely
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/

#ifndef RTDATAID_H
#define RTDATAID_H

#include<ctime>
#include<climits>
#define DATAID_UNSET_VALUE UINT_MAX


#include<ostream>
#include<string>
using namespace std;

class RtStreamComponent;
class RtData;

class RtDataID {

public:

  // default constructor
  RtDataID();

  // constructor from known fields
  RtDataID(unsigned int _studyNum,
	   unsigned int _seriesNum = DATAID_UNSET_VALUE,
	   unsigned int _timePoint = DATAID_UNSET_VALUE,
	   const string &_history = "",
	   const string &_moduleID = "",
	   const string &_dataName = "",
	   const string &_roiID = "");

  // constructor from a data id from a previous module
  RtDataID(const RtDataID &prevModuleData,
	   const RtStreamComponent &thisModule);


  virtual ~RtDataID();

  //*** operators ***//

  // comparison
  bool operator==(const RtDataID &other) const;
  bool operator!=(const RtDataID &other) const { return ! (*this == other); } ;

  // less
  bool operator<(const RtDataID &other) const;

  // get a string version of the ID
  string toString() const;

  // set info from a data id of the input data
  void setFromInputDataID(const RtDataID &prevModuleDataID,
			  const RtStreamComponent &thisModule);

  // set info from input data
  void setFromInputData(const RtData &prevModuleData,
			const RtStreamComponent &thisModule);

  // set the ID from a string
  string setFromString(const string &id);

  //*** gets ***//
  unsigned int getSiteIDNum()         const { return siteIDNum; }
  unsigned int getStudyNum()          const { return studyNum; }
  unsigned int getSeriesNum()         const { return seriesNum; }
  unsigned int getTimePoint()         const { return timePoint; }
  string       getHistory()           const { return history; }
  string       getModuleID()          const { return moduleID; }
  string       getDataName()          const { return dataName; }
  string       getRoiID()             const { return roiID; }
  time_t       getInstantiationTime() const { return instantiationTime; }

  //*** sets ***//
  void setSiteIDNum(unsigned int n) { siteIDNum = n; }
  void setStudyNum (unsigned int n) { studyNum  = n; }
  void setSeriesNum(unsigned int n) { seriesNum = n; }
  void setTimePoint(unsigned int n) { timePoint = n; }
  void setHistory(string s)         { history = s; }
  void setModuleID(string s)        { moduleID = s; }
  void setDataName(string s)        { dataName = s; }
  void setRoiID(string s)           { roiID = s; }

protected:

  // fields
  unsigned int  siteIDNum;        // id of study site
  unsigned int  studyNum;         // study number (date and time study started)
  unsigned int  seriesNum;        // image series (count up from 1)
  unsigned int  timePoint;        // image number (count up from 1)
  string        history;          // ids of modules already used on this data
  string        moduleID;         // id of this processing module
  string        dataName;         // name indicating the particular data kind
  string        roiID;            // if this was processed on a particular ROI
  time_t        instantiationTime;// time this data was first constructed
};

// comparison class for storing ids in a map (see RtDataStore.h)
class RtDataIDCompare {
public:
  bool operator()(const RtDataID* one, const RtDataID* two) const {
    return (*one) < (*two);
  }
};


// output to stream
ostream &operator<<(ostream &out, const RtDataID &id) {
  out << id.toString();
  return out;
}


#endif
