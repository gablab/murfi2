/******************************************************************************
 * RtDataID.cpp defines a class for identifying data uniquely
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/

// not ready for prime time
#ifdef 0

#include"RtDataID.h"
#include"site_config.h"

RtDataID::RtDataID() {
  siteIDNum = MY_SITE_ID_NUMBER;
  studyNum = 0;
  seriesNum = 0;
  timePoint = 0;
  inputDataID = "";
  processModuleID = "";
  dataID = "";
  roiID = "";
}

RtDataID::RtDataID(unsigned int _studyNum,
		   unsigned int _seriesNum,
		   unsigned int timePoint,
		   const RtDataID &_inputID,
		   const string &_processModuleID,
		   const string &_dataID,
		   const string &_roiID) {
  siteIDNum = MY_SITE_ID_NUMBER;
  studyNum = _studyNum;
  seriesNum = _seriesNum;
  timePoint = _timePoint;
  inputDataID = _inputID;
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
    && inputDataID     == other.inputDataID
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
    || inputDataID     < other.inputDataID
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
    << ":" << inputDataID
    << ":" << processModuleID
    << ":" << dataID
    << ":" << roiID;
  return sstr.str();
}

// set the ID from a string
string RtDataID::setFromString(const string &id) {
  return "not yet implemented";
}

#endif
