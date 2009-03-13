/******************************************************************************
 * header file for the top level and main function for a real-time
 * experiment
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-02
 *
 *****************************************************************************/

#ifndef RT_EXPERIMENT_H
#define RT_EXPERIMENT_H

#include"RtConfigFmriExperiment.h"
#include"RtConfigFmriRun.h"
#include"RtDataStore.h"
#include"RtInfoServer.h"

// initialize the experiment (call before the first run is prepared)
// returns true for success
bool initExperiment();

// execute a single run of the backend computation system
int executeRun(const RtConfigFmriRun &conf);

// execute a single run of the backend computation system and block
int executeRunBlocking(const RtConfigFmriRun &conf);

// get the unique ID number for this study
unsigned int getExperimentStudyID();

// get the current experiment elapsed time in ms
time_t getExperimentElapsedTime();

// get the current time in ms since
time_t getNow();

// get the configuration for this experiment
RtConfigFmriExperiment &getConfig();

// get the data store for this experiment
RtDataStore &getDataStore();

// get the unique ID number for this image series UID
unsigned int getSeriesNumFromUID(char *uid);

// get a new unique series number
unsigned int getNextUniqueSeriesNum();

#endif
