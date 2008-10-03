/******************************************************************************
 * declaration of a class that holds information about the loaded experiment 
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-02 
 * 
 *****************************************************************************/

#ifndef RT_EXPERIMENT_H
#define RT_EXPERIMENT_H

#include"RtConductor.h"

class RtExperiment {
public:
  //* static methods for retreiving info about the the experiment *//
  
  // get the unique ID number for this site
  static unsigned int getExperimentSiteID();
  
  // get the unique ID number for this study
  static unsigned int getExperimentStudyID();
  
  // get the unique ID number for this image series UID
  static unsigned int getSeriesNumFromUID(char *uid);
  
  // get the current experiment elapsed time in ms
  static time_t getExperimentElapsedTime();
  
  // get the current time in ms since 
  static time_t getNow();

private:

  static map<string, unsigned int> uids;
  static unsigned int studyID;
  static time_t startTime;

  // set studyID
  // SHOULD ONLY BE CALLED FROM THE CONSTRUCTOR FOR THIS CLASS AND FROM
  // RTCONDUCTOR IF AN EXISTING STUDY IS BEING LOADED
  static void setExperimentStudyID(string s);
  friend class RtConductor;

  RtExperiment();
  ~RtExperiment();

};

#endif
