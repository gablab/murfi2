/*=========================================================================
 *  definition of a class that holds information about the loaded experiment
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

#include"RtExperiment.h"

#include"RtConductor.h"
#include"RtConfigFmriExperiment.h"
#include"RtConfigFmriRun.h"

// site specific defines (siteID, etc)
#include"site_config.h"

#include"ace/Get_Opt.h"

// limit
#include"RtLimit.h"

// platform independent filesystem access
#include<boost/filesystem.hpp>
#include<boost/filesystem/operations.hpp>

using namespace boost::filesystem;

#include"util/timer/timer.h"

//////////////////////////////////////////////////////////////////////////////
// experiment data
//////////////////////////////////////////////////////////////////////////////

namespace {
// experiment configuration
RtConfigFmriExperiment config;

// one or the other of these should be set to read the configuration
string confFilename;
string confXmlStr;

string defaultConfigXML(
  "<?xml version='1.0' encoding='UTF-8'?>"
  "<scanner>"
  "  <option name='disabled'>      false </option>"
  "  <option name='receiveImages'> true </option>"
  "  <option name='saveImages'>    true </option>"
  "  <option name='port'>          15000 </option>"
  "  <option name='onlyReadMoCo'>  false </option>"
  "  <option name='unmosaic'>      true </option>"
  "</scanner>"
  "<infoserver>"
  "    <option name='disabled'> false </option>"
  "    <option name='port'> 15001 </option>"
  "</infoserver>"
);

// store and allow access to collected and computed data
RtDataStore dataStore;

// tcpip interface to experiment information
RtInfoServer *infoServer = NULL;

// tcpip interface to scanner input
RtInputScannerImages scannerInput;

// conductor to execute fmri runs
RtConductor *conductor = NULL;

// raw data identifiers that we've seen (need for knowing which data
// streams are novel)
map<string, unsigned int> uids;

// number of unique input data series we've seen
unsigned int numExistingSeries;

// unique id for this study (date and time of first initialization)
unsigned int studyID;

// list of mask filenames to align
vector<string> masksToAlign;

// timers
timer experimentTimer;
timer computeTimer;

string execName;

string subjectName;
string subjectsDir;

} // anonymous namespace

//* methods for retreiving info about the the experiment *//

// get the unique ID number for this study
unsigned int getExperimentStudyID() {
  return studyID;
}

// get the unique ID number for this image series UID
// checks if this uid exists
//  if so returns its index+1
//  if not adds it to the end and returns the length for the list
unsigned int getSeriesNumFromUID(char *uid) {
  // serach for the uid
  map<string, unsigned int>::iterator i = uids.find(uid);

  unsigned int seriesNum = 0;

  if(i == uids.end()) { // not found
    string s(uid);
    uids[s] = uids.size()+1+numExistingSeries;
    seriesNum = uids.size()+numExistingSeries;
  }
  else {
    seriesNum = (*i).second-1;
  }

  if(DEBUG_LEVEL & ADVANCED) {
    cerr << "getsernumfromuid: uid=" << uid << " found=" << (i!=uids.end())
         << " series num=" << seriesNum << endl;
  }

  return seriesNum;
}

// get a new unique series number
unsigned int getNextUniqueSeriesNum() {

  string max = "A";
  map<string, unsigned int>::iterator i = uids.begin();
  for(; i != uids.end(); i++) {
    if((*i).first >= max) {
      max = (*i).first;
    }
  }

  // add "A" to the max string
  string s(max + "A");
  uids[s] = uids.size()+1+numExistingSeries;
  return uids.size()+numExistingSeries;
}

// reset and start the compute timer
bool startComputeTimer() {
  if(computeTimer.is_running()) {
    return false;
  }

  computeTimer.restart();
  return true;
}

// stop the compute timer and return the elapsed time
double stopComputeTimer() {
  if(!computeTimer.is_running()) {
    return -1;
  }

  computeTimer.stop();
  return computeTimer.elapsed_time();
}

// get the current experiment elapsed time in ms
double getExperimentElapsedTime() {
  return 1000*experimentTimer.elapsed_time();
}

// get the configuration for this experiment
RtConfigFmriExperiment &getExperimentConfig() {
  return config;
}

// get the configuration filename for this experiment
string getExperimentConfigFile() {
  return confFilename;
}

// get the data store for this experiment
RtDataStore &getDataStore() {
  return dataStore;
}

// get the conductor for this experiment
RtConductor &getConductor() {
  return *conductor;
}

// initialize the experiment (call before the first run is prepared)
// returns true for success
bool initExperiment() {

  cout << "intializing experiment" << endl;

  // set the start time of the experiment
  experimentTimer.restart();

  // make the study id
  ACE_Date_Time t;
  char str[] = "yyyyhhmmss";
  sprintf(str, "%04ld%02ld%02ld%02ld",
          t.year(), t.hour(), t.minute(), t.second());
  RtConfigVal::convert<unsigned int>(studyID,str);

  // parse the configuration
  if(!confFilename.empty()) { // look first to a file
    cout << "parsing config file...";

    if(!config.parseConfigFile(confFilename)) {
      cout << "failed" << endl;
      cerr << "ERROR: failed to parse config file: " << confFilename << endl;
      return false;
    }

    // TODO: what to do when subject or subjects dir is set in the config file?
    if (config.isSet("study:subjectsDir")) {
      cout << "WARNING!!!! A subjects dir was provided in the config file."
           << " It will be IGNORED" << endl;
    }
    config.set("study:subjectsDir", subjectsDir);

    cout << "done" << endl;
  }
  else { // next look to a string
    if(confXmlStr.empty()) {
      confXmlStr = defaultConfigXML;
    }

    // TODO: what to do when subject or subjects dir is set in the
    // config string?
    config.set("study:subject:name", subjectName);
    config.set("study:subjectsDir", subjectsDir);

    cout << "parsing config xml string...";

    if(!config.parseConfigStr(confXmlStr)) {
      cout << "failed" << endl;
      cerr << "ERROR: failed to parse config string" << confXmlStr << endl;
      return false;
    }

    cout << "done" << endl;
  }

  // count existing series for this experiment
  cout << "searching for existing series... " << endl;
  numExistingSeries = -1;
  path p;
  do {
    numExistingSeries++;
    p.operator=(config.getVolFilename(numExistingSeries+1,1));
  } while(exists(p) && is_regular(p)
          && numExistingSeries < TOO_MANY_EXISTING_SERIES);
  cout << "found " << numExistingSeries << " existing series" << endl;


  // start info server
  if(config.isSet("infoserver:disabled")
     && config.get("infoserver:disabled")==false) {
    infoServer = new RtInfoServer();
    if(!infoServer->open(config)) {
      cerr << "ERROR: could not initialize info server" << endl;
    }
    else {
      infoServer->activate(); // start the info server thread
    }
  }

  // start scanner listener
  if(config.isSet("scanner:disabled")
     && config.get("scanner:disabled")==false) {
    if(!scannerInput.open(config)) {
      cerr << "ERROR: could not add scanner input" << endl;
    }
    else {
      scannerInput.activate(); // start the scanner input thread
    }
  }

  return true;
}


// deinitialize the experiment (call after last run is complete)
// returns true for success
bool deinitExperiment() {

  cout << "deintializing experiment" << endl;

  // set the start time of the experiment
  experimentTimer.stop();

  // stop info server
  if(NULL != infoServer && !infoServer->close()) {
    cerr << "ERROR: could not deinitialize info server" << endl;
  }
  else {
    cout << "stopped the infoserver" << endl;
  }

  // stop info server
  if(!scannerInput.close()) {
    cerr << "ERROR: could not deinitialize scanner input" << endl;
  }
  else {
    cout << "stopped the scanner image listener" << endl;
  }

  // delete stuff
  delete conductor;
  delete infoServer;

  return true;
}

// initialize and run the backend computation system
int executeRun(RtConfigFmriRun &conf) {

  // make sure only one backend is running at a time
  if(conductor != NULL && conductor->isRunning()) {
    cerr << "an instance of the backend computation is already running. "
         << "can't start another" << endl;
    return 1;
  }
  else if(conductor != NULL) {
    delete conductor;
    conductor = NULL;
  }

  // reinitialize scanner listener
  scannerInput.init(conf);

  // clear align mask names
  masksToAlign.clear();

  conductor = new RtConductor();

  // add the inputs and outputs to the conductor
  conductor->addExistingInput(&scannerInput);

  if(NULL != infoServer) {
    conductor->addExistingOutput(infoServer);
  }

  conductor->configure(conf);

  conductor->activate();

  return 0;
}

// initialize and run the backend computation system and block
int executeRunBlocking(RtConfigFmriRun &conf) {

  // make sure only one backend is running at a time
  if(conductor != NULL && conductor->isRunning()) {
    cerr << "an instance of the backend computation is already running. "
         << "can't start another" << endl;
    return 1;
  }
  else if(conductor != NULL) {
    delete conductor;
    conductor = NULL;
  }

  // reinitialize scanner listener
  scannerInput.init(conf);

  conductor = new RtConductor();

  // add the inputs and outputs to the conductor
  conductor->addExistingInput(&scannerInput);

  if(NULL != infoServer) {
    conductor->addExistingOutput(infoServer);
  }

  conductor->configure(conf);

  conductor->svc();

  return 0;
}

// prints the usage info for the realtime system
void printUsage() {
  cout << "usage: " << endl << execName
       << " [-f conffile | -s confxmlstr]"
       << endl;
}

// parse command line args
bool parseArgs(int argc, char **args) {
  ACE_TRACE(("parseArgs"));

  execName = args[0];

  // set up short options
  static const ACE_TCHAR options[] = ACE_TEXT (":f:c:s:h?");
  ACE_Get_Opt cmdOpts(argc, args, options, 1, 0,
                      ACE_Get_Opt::PERMUTE_ARGS, 1);

  // set up long options
  cmdOpts.long_option("conffile",   'f', ACE_Get_Opt::ARG_REQUIRED);
  cmdOpts.long_option("conffile",   'c', ACE_Get_Opt::ARG_REQUIRED);
  cmdOpts.long_option("confxmlstr", 's', ACE_Get_Opt::ARG_REQUIRED);
  cmdOpts.long_option("help",       'h', ACE_Get_Opt::NO_ARG);
  cmdOpts.long_option("help",       '?', ACE_Get_Opt::NO_ARG);

  // handle options
  for(int option; (option = cmdOpts ()) != EOF; ) {
    if(DEBUG_LEVEL & MODERATE) {
      cout << "-" << (char) option << " " << cmdOpts.opt_arg() << endl;
    }

    switch(option) {

      case 'f':
      case 'c':
        confFilename = cmdOpts.opt_arg();
        break;

      case 's':
        confXmlStr = cmdOpts.opt_arg();
        break;

      case 'h':
      case '?':
        return false; // automatically prints usage

      case ':':
        cerr << "ERROR: -" << cmdOpts.opt_opt()
             << " requires an argument" << endl;
        return false;

      default:
        cerr << "ERROR: unknown command line parameter: " << option << endl;
        return false;
    }
  }

  // // parse the xml config (file or string)
  // if(confFilename.empty() && confXmlStr.empty()) {
  //   cerr << "ERROR: no configuration specified" << endl;
  //   return false;
  // }
  // else if(DEBUG_LEVEL & BASIC) {
  //   cout << "config file is: " << confFilename << endl
  //        << "config str is: " << confXmlStr << endl;
  // }

  return true;
}

bool initializeSystem(int argc, char** args) {
  // get subject's directory
  if (getenv("MURFI_SUBJECT_NAME") == NULL ||
      getenv("MURFI_SUBJECTS_DIR") == NULL) {
    cout << "ERROR: The environment variables MURFI_SUBJECT_NAME and "
         << "MURFI_SUBJECTS_DIR must be set."
         << endl;
    return false;
  }

  subjectName = getenv("MURFI_SUBJECT_NAME");
  subjectsDir = getenv("MURFI_SUBJECTS_DIR");

  // setup arguments
  if(!parseArgs(argc, args)) {
    printUsage();
    return false;
  }

  return true;
}

int murfi(int argc, char** args) {
  ACE_TRACE(("ACE_TMAIN"));

  // initialize the system
  if (!initializeSystem(argc, args)) {
    return 1;
  }

  // initialize experiment
  if(!initExperiment()) {
    cerr << "ERROR: experiment initialization failed. check your config"
         << endl;
    return false;
  }

  int result = 0;

  // if the gui frontend is being used, give it control (confused yet?)
  if((config.isSet("gui:disabled") &&
      config.get("gui:disabled")==false) ||
     (config.isSet("oldgui:disabled") && // "backward" compatibility
      config.get("oldgui:disabled")==false)) {

#ifndef USE_FRONTEND
    cerr << "ERROR: this build does not support the gui frontend" << endl;
    return 1;
#endif

    return 0;
  }
  else { // just run once in the terminal with the passed config
    RtConfigFmriRun runConf;
    runConf.parseConfigFile(confFilename);


    conductor = new RtConductor();

    scannerInput.init(runConf);

    conductor->addExistingInput(&scannerInput);

    if(NULL != infoServer) {
      conductor->addExistingOutput(infoServer);
    }

    conductor->configure(runConf);
    result = conductor->svc();
  }

  // deinitialize experiment
  if(!deinitExperiment()) {
    cerr << "ERROR: experiment deinitialization failed."
         << endl;
    result = 1;
  }

  return result;
}

// main entry function
#ifndef USE_FRONTEND
int ACE_TMAIN(int argc, char **args) {
  return murfi(argc, args);
}
#endif
