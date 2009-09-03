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
    studyNum  = DATAID_NUM_UNSET_VALUE;
    seriesNum = DATAID_NUM_UNSET_VALUE;
    timePoint = DATAID_NUM_UNSET_VALUE;
    history   = DATAID_STRING_UNSET_VALUE;
    moduleID  = DATAID_STRING_UNSET_VALUE;
    dataName  = DATAID_STRING_UNSET_VALUE;
    roiID     = DATAID_STRING_UNSET_VALUE;

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

RtDataID::~RtDataID() {
}

//*** operators ***//

bool RtDataID::operator!=(const RtDataID &other) const {
    return !(*this == other);
}
// comparison
// if either data id has a field blank ("") it is ignored in the comparison

bool RtDataID::operator==(const RtDataID &other) const {
    return
    (
               siteIDNum       == other.siteIDNum
            || siteIDNum       == DATAID_NUM_WILDCARD_VALUE
            || other.siteIDNum == DATAID_NUM_WILDCARD_VALUE)

            && (studyNum       == other.studyNum
            || studyNum        == DATAID_NUM_WILDCARD_VALUE
            || other.studyNum  == DATAID_NUM_WILDCARD_VALUE)

            && (seriesNum      == other.seriesNum
            || seriesNum       == DATAID_NUM_WILDCARD_VALUE
            || other.seriesNum == DATAID_NUM_WILDCARD_VALUE)

            && (timePoint      == other.timePoint
            || timePoint       == DATAID_NUM_WILDCARD_VALUE
            || other.timePoint == DATAID_NUM_WILDCARD_VALUE)

            && (history        == other.history
            || history         == DATAID_STRING_WILDCARD_VALUE
            || other.history   == DATAID_STRING_WILDCARD_VALUE)

            && (moduleID       == other.moduleID
            || moduleID        == DATAID_STRING_WILDCARD_VALUE
            || other.moduleID  == DATAID_STRING_WILDCARD_VALUE)

            && (dataName       == other.dataName
            || dataName        == DATAID_STRING_WILDCARD_VALUE
            || other.dataName  == DATAID_STRING_WILDCARD_VALUE)

            && (roiID          == other.roiID
            || roiID           == DATAID_STRING_WILDCARD_VALUE
            || other.roiID     == DATAID_STRING_WILDCARD_VALUE);
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
	&& !(siteIDNum == DATAID_NUM_WILDCARD_VALUE
	     || other.siteIDNum == DATAID_NUM_WILDCARD_VALUE)) {
        return (siteIDNum < other.siteIDNum);
    };

    // studyNum
    if (studyNum != other.studyNum
	&& !(studyNum == DATAID_NUM_WILDCARD_VALUE
	     || other.studyNum == DATAID_NUM_WILDCARD_VALUE)) {
        return (studyNum < other.studyNum);
    };

    // seriesNum
    if (seriesNum != other.seriesNum 
	&& !(seriesNum == DATAID_NUM_WILDCARD_VALUE
	     || other.seriesNum == DATAID_NUM_WILDCARD_VALUE)) {
        return (seriesNum < other.seriesNum);
    };

    // timePoint
    if (timePoint != other.timePoint 
	&& !(timePoint == DATAID_NUM_WILDCARD_VALUE
	     || other.timePoint == DATAID_NUM_WILDCARD_VALUE)) {
        return (timePoint < other.timePoint);
    };

    // history
    if (history != other.history 
	&& !(history == DATAID_STRING_WILDCARD_VALUE 
	     || other.history == DATAID_STRING_WILDCARD_VALUE)) {
        return (history < other.history);
    };

    // moduleID
    if (moduleID != other.moduleID 
	&& !(moduleID == DATAID_STRING_WILDCARD_VALUE 
	     || other.moduleID == DATAID_STRING_WILDCARD_VALUE)) {
        return (moduleID < other.moduleID);
    };

    // dataName
    if (dataName != other.dataName 
	&& !(dataName == DATAID_STRING_WILDCARD_VALUE 
	     || other.dataName == DATAID_STRING_WILDCARD_VALUE)) {
        return (dataName < other.dataName);
    };

    // roiID
    if (roiID != other.roiID 
	&& !(roiID == DATAID_STRING_WILDCARD_VALUE || other.roiID == DATAID_STRING_WILDCARD_VALUE)) {
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

void RtDataID::setFromString(const string &id) {

    string delim = ":";
    size_t startAt;
    size_t foundAt;

    // find and set siteIDNum
    startAt = 1; // data id string has a : at the beginning
    foundAt = id.find(delim, startAt);
    bool fail;
    if (foundAt != string::npos && startAt != foundAt) {
      if(id.substr(startAt, foundAt - startAt) == DATAID_STRING_WILDCARD_VALUE) {
	this->setSiteIDNum(DATAID_NUM_WILDCARD_VALUE);
	fail = false;
      }
      else {
        unsigned int siteIDNum;
        fail = !RtConfigVal::convert<unsigned int>(siteIDNum, id.substr(startAt, foundAt - startAt));
        this->setSiteIDNum(siteIDNum);
      }
    } else { fail = true; }
    if(fail) {
      this->setSiteIDNum(DATAID_NUM_UNSET_VALUE);
    }
    

    // find and set studyNum
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
      if(id.substr(startAt, foundAt - startAt) == DATAID_STRING_WILDCARD_VALUE) {
	this->setStudyNum(DATAID_NUM_WILDCARD_VALUE);
	fail = false;
      }
      else {
        unsigned int studyNum;
        fail = !RtConfigVal::convert<unsigned int>(studyNum, id.substr(startAt, foundAt - startAt));
        this->setStudyNum(studyNum);
      }
    } else { fail = true; }
    if(fail) {
      this->setStudyNum(DATAID_NUM_UNSET_VALUE);
    }

    // find and set seriesNum
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
      if(id.substr(startAt, foundAt - startAt) == DATAID_STRING_WILDCARD_VALUE) {
	this->setSeriesNum(DATAID_NUM_WILDCARD_VALUE);
	fail = false;
      }
      else {
        unsigned int seriesNum;
        fail = !RtConfigVal::convert<unsigned int>(seriesNum, id.substr(startAt, foundAt - startAt));
        this->setSeriesNum(seriesNum);
      }
    } else { fail = true; }
    if(fail) {
      this->setSeriesNum(DATAID_NUM_UNSET_VALUE);
    }

    // find and set timePoint
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
      if(id.substr(startAt, foundAt - startAt) == DATAID_STRING_WILDCARD_VALUE) {
	this->setTimePoint(DATAID_NUM_WILDCARD_VALUE);
	fail = false;
      }
      else {
        unsigned int timePoint;
        fail = !RtConfigVal::convert<unsigned int>(timePoint, id.substr(startAt, foundAt - startAt));
        this->setTimePoint(timePoint);
      }
    } else { fail = true; }
    if(fail) {
      this->setTimePoint(DATAID_NUM_UNSET_VALUE);
    }

    // find and set history
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
        this->setHistory(id.substr(startAt, foundAt - startAt));
	fail = false;
    } else { fail = true; }
    if(fail) {
      this->setHistory(DATAID_STRING_UNSET_VALUE);
    }

    // find and set moduleID
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
        this->setModuleID(id.substr(startAt, foundAt - startAt));
	fail = false;
    } else { fail = true; }
    if(fail) {
      this->setModuleID(DATAID_STRING_UNSET_VALUE);
    }

    // find and set dataName
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
        this->setDataName(id.substr(startAt, foundAt - startAt));
	fail = false;
    } else { fail = true; }
    if(fail) {
      this->setDataName(DATAID_STRING_UNSET_VALUE);
    }

    // find and set roiID
    startAt = foundAt + 1;
    foundAt = id.find(delim, startAt);
    if (foundAt != string::npos && startAt != foundAt) {
        this->setRoiID(id.substr(startAt, foundAt - startAt));
	fail = false;
    } else { fail = true; }
    if(fail) {
      this->setRoiID(DATAID_STRING_UNSET_VALUE);
    }
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
    //siteIDNum = SITE_ID_NUMBER;
}

// output to stream

ostream & operator<<(ostream &out, const RtDataID &id) {
    out << id.toString();
    return out;
}
