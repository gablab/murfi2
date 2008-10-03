/******************************************************************************
 * definition of a class that holds information about the loaded experiment 
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-02 
 * 
 *****************************************************************************/

#include"RtExperiment.h"

#include"site_config.h"

#include<ctime>

//* init static vars *//
time_t RtExperiment::startTime;
unsigned int RtExperiment::studyID;
map<string, unsigned int> RtExperiment::uids;

//* methods for retreiving info about the the experiment *//

// get the unique ID number for this site
unsigned int RtExperiment::getExperimentSiteID() {
  return SITE_ID_NUMBER;
}

// get the unique ID number for this study
unsigned int RtExperiment::getExperimentStudyID() {
  return studyID;
}

// get the unique ID number for this image series UID
// checks if this uid exists 
//  if so returns its index+1
//  if not adds it to the end and returns the length for the list
unsigned int RtExperiment::getSeriesNumFromUID(char *uid) {
  // serach for the uid
  map<string, unsigned int>::iterator i = uids.find(uid);
  
  if(i == uids.end()) { // not found 
    string s(uid);
    uids[s] = uids.size()+1;
    return  uids.size();
  }

  return (*i).second;
}

// get the current experiment elapsed time in ms
time_t RtExperiment::getExperimentElapsedTime() {
  return getNow()-startTime;
}
  
// get the current time in ms since 
time_t RtExperiment::getNow() {
  return time(NULL);
}
  
// set studyID
// SHOULD ONLY BE CALLED FROM THE CONSTRUCTOR FOR THIS CLASS AND FROM
// RTCONDUCTOR IF AN EXISTING STUDY IS BEING LOADED
void RtExperiment::setExperimentStudyID(string s) {
  stringstream ss(s);
  ss >> studyID;
}

// constructor (private)
RtExperiment::RtExperiment() {
  startTime = getNow();

  // make the study id
  ACE_Date_Time t;
  char str[] = "yyyyhhmmss";
  sprintf(str, "%04ld%02ld%02ld%02ld",
	  t.year(), t.hour(), t.minute(), t.second());
  RtExperiment::setExperimentStudyID(str);
};

RtExperiment::~RtExperiment() {};

