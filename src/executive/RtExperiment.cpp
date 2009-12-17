/******************************************************************************
 * definition of a class that holds information about the loaded experiment 
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-02 
 * 
 *****************************************************************************/

#include"RtExperiment.h"

#include"RtConductor.h"
#include"RtConfigFmriExperiment.h"
#include"RtConfigFmriRun.h"

// frontend gui includes
#ifdef USE_FRONTEND
 #include"FrApplication.h"
 #include"FrMainWindow.h"
 #include"FrMainDocument.h"
 #include"FrMainController.h"
#endif

// old gui includes
#ifdef USE_OLDFRONTEND
    #include"RtDisplayImage.h"
#endif

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
// experiment data (all at static file scope)
//////////////////////////////////////////////////////////////////////////////

// experiment configuration
static RtConfigFmriExperiment config;

// one or the other of these should be set to read the configuration
static string confFilename;
static string confXmlStr;

// store and allow access to collected and computed data
static RtDataStore dataStore;

// tcpip interface to experiment information
static RtInfoServer infoServer;

// tcpip interface to scanner input
static RtInputScannerImages scannerInput;

// conductor to execute fmri runs
static RtConductor *conductor = NULL;

// raw data identifiers that we've seen (need for knowing which data
// streams are novel)
static map<string, unsigned int> uids;  

// number of unique input data series we've seen
static unsigned int numExistingSeries;

// unique id for this study (date and time of first initialization)
static unsigned int studyID;

// list of mask filenames to align
static vector<string> masksToAlign;

// timers
static timer experimentTimer;
static timer computeTimer;

static string execName;


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
    cerr << "getsernumfromuid: uid=" << uid << " found=" << (i!=uids.end()) << " series num=" << seriesNum << endl; 
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

// get the data store for this experiment
RtDataStore &getDataStore() {
  return dataStore;
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

    cout << "done" << endl;
  }
  else if(!confXmlStr.empty()) { // next look to a string
    cout << "parsing config xml string...";

    if(!config.parseConfigStr(confXmlStr)) {
      cout << "failed" << endl;
      cerr << "ERROR: failed to parse config string" << confXmlStr << endl;
      return false;
    }

    cout << "done" << endl;
  }
  else { // error, no config
    cerr << "ERROR: no experiment configuration provided" << endl;
    return false;
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
    if(!infoServer.open(config)) {
      cerr << "ERROR: could not initialize info server" << endl;
    }
    else {
      infoServer.activate(); // start the info server thread
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
  if(!infoServer.close()) {
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

  return true;
}

// initialize and run the backend computation system
int executeRun(RtConfigFmriRun &conf) {

  // make sure only one backend is running at a time
  if(conductor != NULL && conductor->isRunning()) {
    cerr << "an instance of the backend computation is already running. can't start another" << endl;
    return 1;
  }
  else if(conductor != NULL) {
    delete conductor;
  }

  // reinitialize scanner listener
  scannerInput.init(conf);

  // clear align mask names
  masksToAlign.clear();

  conductor = new RtConductor();

  // add the inputs and outputs to the conductor
  conductor->addExistingInput(&scannerInput);
  conductor->addExistingOutput(&infoServer);
  conductor->configure(conf);

  conductor->activate();

  return 0;
}

// initialize and run the backend computation system and block
int executeRunBlocking(RtConfigFmriRun &conf) {

  // make sure only one backend is running at a time
  if(conductor != NULL && conductor->isRunning()) {
    cerr << "an instance of the backend computation is already running. can't start another" << endl;
    return 1;
  }
  else if(conductor != NULL) {
    delete conductor;
  }

  // reinitialize scanner listener
  scannerInput.init(conf);

  conductor = new RtConductor();

  // add the inputs and outputs to the conductor
  conductor->addExistingInput(&scannerInput);
  conductor->addExistingOutput(&infoServer);
  conductor->configure(conf);

  conductor->svc();

  return 0;
}

// prints the usage info for the realtime system
void printUsage() {
  int w = 15;

  cout << "usage: " << endl << execName
       << " [-f conffile | -s confxmlstr]"
       << endl;
//       << "---------------------------------------------" << endl
//       << "some useful flags:" << endl
//       << setiosflags(ios::left)
//       << setw(w) << endl
//       << "---------------------------------------------" << endl;
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

// scopic Alan: for MRI testers
#ifdef MRI_TEST
    confFilename = "./test_config.xml";
#else
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
      cout << "here" << endl;
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
#endif

  // parse the xml config (file or string)
  if(confFilename.empty() && confXmlStr.empty()) {
    cerr << "ERROR: no configuration specified" << endl;
    return false;
  }
  else if(DEBUG_LEVEL & BASIC) {
    cout << "config file is: " << confFilename << endl
	 << "config str is: " << confXmlStr << endl;
  }

  return true;
}

// main entry function 
int ACE_TMAIN(int argc, char **args) {
  ACE_TRACE(("ACE_TMAIN"));

  // setup arguments
  if(!parseArgs(argc,args)) {
    printUsage();
    return 1;
  }

  // initialize experiment
  if(!initExperiment()) {
    cerr << "ERROR: experiment initialization failed. check your config" 
	 << endl;
    return 1;
  }

  int result = 0;

  // if the gui frontend is being used, give it control
  if(config.isSet("gui:disabled") &&
     config.get("gui:disabled")==false) { 

#ifdef USE_FRONTEND
    FrApplication application(argc, args);

    // Create main view and document of app
    FrMainWindow* mainWindow = new FrMainWindow();
    FrMainDocument* document = new FrMainDocument();

    // Create main controller.
    FrMainController controller(mainWindow, document);
    controller.Initialize();

    int result = 0;
    if(!application.exec()){
      result = 1;
    }
#else
    cerr << "ERROR: this build does not support the gui frontend" << endl;
#endif
  }
  else if(config.isSet("oldgui:disabled")
	  && config.get("oldgui:disabled")==false) { 
    
// scopic Alan: old image display was removed from Frontend project on windows
// ohinds: 2009-05-10 used a makefile flag instead of the win32 define
#ifdef USE_OLDFRONTEND
    // start a display
    RtDisplayImage dispImg;
    if(!dispImg.open(config)) {
      cerr << "ERROR: could not open display" << endl;
      result = 1;
    }
    result = dispImg.svc();
#else
    cerr << "ERROR: this build does not support the old gui frontend" << endl;
#endif

  }
  else { // just run once in the terminal with the passed config
    RtConfigFmriRun runConf;
    runConf.parseConfigFile(confFilename);


    conductor = new RtConductor();

    scannerInput.init(runConf);

    conductor->addExistingInput(&scannerInput);
    conductor->addExistingOutput(&infoServer);

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


