/******************************************************************************
 * RtDataID.cpp defines a class for identifying data uniquely
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/

#include"RtDataID.h"

#include"RtExperiment.h"
#include"RtStreamComponent.h"
#include"RtData.h"
#include"site_config.h" // for SITE_ID_NUMBER

#include<climits>

// default constructor
RtDataID::RtDataID() {
    siteIDNum = SITE_ID_NUMBER;
    studyNum =  DATAID_UNSET_VALUE;
    seriesNum = DATAID_UNSET_VALUE;
    timePoint = DATAID_UNSET_VALUE;
    history = "";
    moduleID = "";
    dataName = "";
    roiID = "";

    instantiationTime = getExperimentElapsedTime();
}

// constructor with known fields
RtDataID::RtDataID(unsigned int _studyNum,
                   unsigned int _seriesNum,
                   unsigned int _timePoint,
                   const string &_history,
                   const string &_moduleID,
                   const string &_dataName,
                   const string &_roiID) {
    siteIDNum = SITE_ID_NUMBER;
    studyNum = _studyNum;
    seriesNum = _seriesNum;
    timePoint = _timePoint;
    history = _history;
    moduleID = _moduleID;
    dataName = _dataName;
    roiID = _roiID;

    instantiationTime = getExperimentElapsedTime();
}

// copy constructor
RtDataID::RtDataID(const RtDataID &other) {
  copyFromOtherDataID(other);
}


// constructor from a data id from a previous module
RtDataID::RtDataID(RtDataID &prevModuleData,
                   const RtStreamComponent &thisModule) {
    setFromInputDataID(prevModuleData, thisModule);
}


// constructor from another data id
//RtDataID::RtDataID(const RtDataID &other) {
//
//}

RtDataID::~RtDataID() {}

//*** operators ***//

bool RtDataID::operator!=(const RtDataID &other) const {
    return !(*this==other);
}
// comparison
// if either data id has a field blank ("") it is ignored in the comparison
bool RtDataID::operator==(const RtDataID &other) const {
    return
        (siteIDNum          == other.siteIDNum
         || siteIDNum       == DATAID_UNSET_VALUE
         || other.siteIDNum == DATAID_UNSET_VALUE)

        && (studyNum           == other.studyNum
            || studyNum        == DATAID_UNSET_VALUE
            || other.studyNum  == DATAID_UNSET_VALUE)

        && (seriesNum          == other.seriesNum
            || seriesNum       == DATAID_UNSET_VALUE
            || other.seriesNum == DATAID_UNSET_VALUE)

        && (timePoint          == other.timePoint
            || timePoint       == DATAID_UNSET_VALUE
            || other.timePoint == DATAID_UNSET_VALUE)

        && (history            == other.history
            || history         == ""
            || other.history   == "")

        && (moduleID           == other.moduleID
            || moduleID        == ""
            || other.moduleID  == "")

        && (dataName           == other.dataName
            || dataName        == ""
            || other.dataName  == "")

        && (roiID              == other.roiID
            || roiID           == ""
            || other.roiID     == "");
}

// less
bool RtDataID::operator<(const RtDataID &other) const {
    /* Logic:
     * if a != b, return whether or not it's a < b or a > b
     * if a == b, continue to next field
     * NOTE: If fields are added to the DataID in the future, another logic block needs to be added*/

    // siteIDNum
    if (siteIDNum != other.siteIDNum) {
        return (siteIDNum < other.siteIDNum);
    };

    // studyNum
    if (studyNum != other.studyNum) {
        return (studyNum < other.studyNum);
    };

    // seriesNum
    if (seriesNum != other.seriesNum) {
        return (seriesNum < other.seriesNum);
    };

    // timePoint
    if (timePoint != other.timePoint) {
        return (timePoint < other.timePoint);
    };

    // history
    if (history != other.history) {
        return (history < other.history);
    };

    // moduleID
    if (moduleID != other.moduleID) {
        return (moduleID < other.moduleID);
    };

    // dataName
    if (dataName != other.dataName) {
        return (dataName < other.dataName);
    };

    // roiID
    if (roiID != other.roiID) {
        return (roiID < other.roiID);
    };

    // If reached, all a == b, so return false (a !< b)
    return false;
}

// partial less
bool RtDataID::partless(const RtDataID &other) const {
    /* Logic:
     * if a != b, return whether or not it's a < b or a > b
     * if a == b, continue to next field
     * NOTE: If fields are added to the DataID in the future, another logic block needs to be added*/

    // siteIDNum
    if (siteIDNum != other.siteIDNum 
	&& !(siteIDNum == DATAID_UNSET_VALUE
	     || other.siteIDNum == DATAID_UNSET_VALUE)) {
        return (siteIDNum < other.siteIDNum);
    };

    // studyNum
    if (studyNum != other.studyNum
	&& !(studyNum == DATAID_UNSET_VALUE
	     || other.studyNum == DATAID_UNSET_VALUE)) {
        return (studyNum < other.studyNum);
    };

    // seriesNum
    if (seriesNum != other.seriesNum 
	&& !(seriesNum == DATAID_UNSET_VALUE
	     || other.seriesNum == DATAID_UNSET_VALUE)) {
        return (seriesNum < other.seriesNum);
    };

    // timePoint
    if (timePoint != other.timePoint 
	&& !(timePoint == DATAID_UNSET_VALUE
	     || other.timePoint == DATAID_UNSET_VALUE)) {
        return (timePoint < other.timePoint);
    };

    // history
    if (history != other.history 
	&& !(history.empty() || other.history.empty())) {
        return (history < other.history);
    };

    // moduleID
    if (moduleID != other.moduleID 
	&& !(moduleID.empty() || other.moduleID.empty())) {
        return (moduleID < other.moduleID);
    };

    // dataName
    if (dataName != other.dataName 
	&& !(dataName.empty() || other.dataName.empty())) {
        return (dataName < other.dataName);
    };

    // roiID
    if (roiID != other.roiID 
	&& !(roiID.empty() || other.roiID.empty())) {
        return (roiID < other.roiID);
    };

    // If reached, all a == b, so return false (a !< b)
    return false;
}

// output to a stream
// ostream &RtDataID::operator<<(ostream &out) const {
//   out
//     << ":" << siteIDNum
//     << ":" << studyNum
//     << ":" << seriesNum
//     << ":" << timePoint
//     << ":" << history
//     << ":" << moduleID
//     << ":" << dataName
//     << ":" << roiID
//     << ":";
//   return out;
// }

// get a string version of the ID
string RtDataID::toString() const {
    stringstream sstr("");

    sstr
    << ":" << siteIDNum
    << ":" << studyNum
    << ":" << seriesNum
    << ":" << timePoint
    << ":" << history
    << ":" << moduleID
    << ":" << dataName
    << ":" << roiID
    << ":";
    return sstr.str();
}

// set the ID from a string
string RtDataID::setFromString(const string &id) {
    return "not yet implemented";
}

// set from input data
void RtDataID::setFromInputData(RtData &prevModuleData,
                                const RtStreamComponent &thisModule) {
    setFromInputDataID(prevModuleData.getDataID(), thisModule);
}

// set from input data
void RtDataID::setFromInputDataID(RtDataID &prevModuleDataID,
                                  const RtStreamComponent &thisModule) {
  copyFromOtherDataID(prevModuleDataID);
  history = history + moduleID;
  moduleID = thisModule.getID();
  
  instantiationTime = getExperimentElapsedTime();
}

// copy from other data
void RtDataID::copyFromOtherDataID(const RtDataID &otherDataID) {
    (*this) = otherDataID;
    siteIDNum = SITE_ID_NUMBER;
}

// output to stream
ostream &operator<<(ostream &out, const RtDataID &id)  {
    out << id.toString();
    return out;
}
