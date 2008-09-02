/******************************************************************************
 * RtDataID.cpp defines a class for identifying data uniquely
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/

// not ready for prime time
#if 0

#include"RtDataID.h"
#include"site_config.h"

// default constructor
RtDataID::RtDataID() {
  siteIDNum = MY_SITE_ID_NUMBER;
  studyNum = 0;
  seriesNum = 0;
  timePoint = 0;
  history = "";
  processModuleID = "";
  dataID = "";
  roiID = "";
}

// constructor with known fields
RtDataID::RtDataID(unsigned int _studyNum,
		   unsigned int _seriesNum,
		   unsigned int timePoint,
		   const string &_history,
		   const string &_processModuleID,
		   const string &_dataID,
		   const string &_roiID) {
  siteIDNum = MY_SITE_ID_NUMBER;
  studyNum = _studyNum;
  seriesNum = _seriesNum;
  timePoint = _timePoint;
  history = _history;
  processModuleID = _processModuleID;
  dataID = _dataID;
  roiID = _roiID;
}

// constructor from a data id from a previous module
RtDataID::RtDataID(const RtDataID &prevModuleData,
		   const RtStreamComponent &_thisModule) {
  siteIDNum = MY_SITE_ID_NUMBER;
  studyNum = _studyNum;
  seriesNum = _seriesNum;
  timePoint = _timePoint;
  history = _history;
  processModuleID = _processModuleID;
  dataID = _dataID;
  roiID = _roiID;
}

virtual RtDataID::~RtDataID() {}

//*** operators ***//

// comparison
bool RtDataID::operator==(const RtDataID &other) {
  return
       siteIDNum       == other.siteIDNum
    && studyNum        == other.studyNum
    && seriesNum       == other.seriesNum
    && timePoint       == other.timePoint
    && history         == other.history
    && processModuleID == other.processModuleID
    && dataID          == other.dataID
    && roiID           == other.roiID
    ;
}

// less
bool RtDataID::operator<(const RtDataID &other) {
  return
       siteIDNum       < other.siteIDNum
    || studyNum        < other.studyNum
    || seriesNum       < other.seriesNum
    || timePoint       < other.timePoint
    || history     < other.history
    || processModuleID < other.processModuleID
    || dataID          < other.dataID
    || roiID           < other.roiID
    ;

}

// get a string version of the ID
string RtDataID::toString() {
  stringstream sstr("");

  sstr 
    << ":" << siteIDNum
    << ":" << studyNum
    << ":" << seriesNum
    << ":" << timePoint
    << ":" << history
    << ":" << processModuleID
    << ":" << dataID
    << ":" << roiID
    << ":";
  return sstr.str();
}

// set the ID from a string
string RtDataID::setFromString(const string &id) {
  return "not yet implemented";
}

#endif
