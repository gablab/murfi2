/*=========================================================================
 *  header file for the top level and
 *  main function for a real-time
 *  experiment
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef RT_EXPERIMENT_H
#define RT_EXPERIMENT_H

#include"RtConfigFmriExperiment.h"
#include"RtConfigFmriRun.h"
#include"RtDataStore.h"
#include"RtInfoServer.h"

// initialize the entire murfi system
bool initializeSystem(int argc, char** args);

// initialize the experiment (call before the first run is prepared)
// returns true for success
bool initExperiment();

// deinitialize the experiment (call after the last run is complete)
// returns true for success
bool deinitExperiment();

// execute a single run of the backend computation system
int executeRun(RtConfigFmriRun &conf);

// execute a single run of the backend computation system and block
int executeRunBlocking(RtConfigFmriRun &conf);

// get the unique ID number for this study
unsigned int getExperimentStudyID();

// start the compute timer
bool startComputeTimer();

// stop the compute timer and return the time
double stopComputeTimer();

// get the current experiment elapsed time in ms
double getExperimentElapsedTime();

// get the configuration for this experiment
RtConfigFmriExperiment &getExperimentConfig();

// get the configuration filename for this experiment
string getExperimentConfigFile();

// get the data store for this experiment
RtDataStore &getDataStore();

// get the conductor for the experiment
RtConductor &getConductor();

// get the unique ID number for this image series UID
unsigned int getSeriesNumFromUID(char *uid);

// get a new unique series number
unsigned int getNextUniqueSeriesNum();

#endif
