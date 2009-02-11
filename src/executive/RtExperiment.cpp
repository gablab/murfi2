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
#include"FrApplication.h"
#include"FrMainWindow.h"
#include"FrMainDocument.h"
#include"FrMainController.h"

// old gui includes
#include"RtDisplayImage.h"

// site specific defines (siteID, etc)
#include"site_config.h"

#include"ace/Get_Opt.h"

// limit
#include"RtLimit.h"

// platform independent filesystem access
#include<boost/filesystem.hpp>
#include<boost/filesystem/operations.hpp>

using namespace boost::filesystem;

#include<ctime>

//////////////////////////////////////////////////////////////////////////////
// experiment data (all at static file scope)
//////////////////////////////////////////////////////////////////////////////

// experiment configuration
static RtConfigFmriExperiment config;

// one or the other of these should be set to read the configuration
static string confFilename;
static string confXmlStr;

// to conduct a single run
static RtConductor conductor;

// store and allow access to collected and computed data
static RtDataStore dataStore;

// tcpip interface to experiment information
static RtInfoServer infoServer;

// raw data identifiers that we've seen (need for knowing which data
// streams are novel)
static map<string, unsigned int> uids;

// number of unique input data series we've seen
static unsigned int numExistingSeries;

// unique id for this study (date and time of first initialization)
static unsigned int studyID;

// experiment initialization time
static time_t startTime;

// experiment initialization time
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

// get the current experiment elapsed time in ms
time_t getExperimentElapsedTime() {
  return getNow()-startTime;
}

// get the current time in ms
time_t getNow() {
  return time(NULL);
}

// get the configuration for this experiment
RtConfigFmriExperiment &getConfig() {
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
  startTime = getNow();

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
    infoServer.activate(); // start the info server thread
  }

}

// initialize and run the backend computation system
int executeRun(const RtConfigFmriRun &conf) {

  // make sure only one backend is running at a time
  if(conductor.isRunning()) {
    cerr << "an instance of the backend computation is already running. can't start another" << endl;
    return 1;
  }

  conductor.configure(conf);
  conductor.activate();

  return 0;
}

// initialize and run the backend computation system
int executeRunBlocking(const RtConfigFmriRun &conf) {

  // make sure only one backend is running at a time
  if(conductor.isRunning()) {
    cerr << "an instance of the backend computation is already running. can't start another" << endl;
    return 1;
  }

  conductor.configure(conf);
  conductor.svc();

  return 0;
}

// prints the usage info for the realtime system
void printUsage() {
  int w = 15;

  cout << "usage: " << endl <<execName
       << " [-f conffile | -s confxmlstr]" << endl
       << endl
       << "---------------------------------------------" << endl
       << "some useful flags:" << endl
       << setiosflags(ios::left)
       << setw(w) << endl
       << "---------------------------------------------" << endl;
}

// parse command line args
bool parseArgs(int argc, char **args) {
  ACE_TRACE(("parseArgs"));

  execName = args[0];

  // set up short options
  static const ACE_TCHAR options[] = ACE_TEXT (":f:s:h");
  ACE_Get_Opt cmdOpts(argc, args, options, 1, 0,
		      ACE_Get_Opt::PERMUTE_ARGS, 1);

  // set up long options
  cmdOpts.long_option("conffile",   'f', ACE_Get_Opt::ARG_REQUIRED);
  cmdOpts.long_option("confxmlstr", 's', ACE_Get_Opt::ARG_REQUIRED);
  cmdOpts.long_option("help",       'h', ACE_Get_Opt::NO_ARG);

  // handle options
  for(int option; (option = cmdOpts ()) != EOF; ) {
    switch(option) {

    case 'f':
      confFilename = cmdOpts.opt_arg();
      break;

    case 's':
      confXmlStr = cmdOpts.opt_arg();
      break;

    case 'h':
      printUsage();
      exit(1);

    case ':':
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
          cmdOpts.opt_opt()), -1);

    default:
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
    }
  }

  // parse the xml config (file or string)
  if(confFilename.empty() && confXmlStr.empty()) {
    cerr << "ERROR: no configuration specified" << endl;
    return false;
  }

  return true;
}

/* ohinds 2009-02-02
 * added this temporarily. we'll use this main soon....
 */
#ifndef USE_FRONTEND

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
  if(config.get("gui:disabled")==false) {

    FrApplication application(argc, args);

    // Create main view and document of app
    FrMainWindow* mainWindow = new FrMainWindow();
    FrMainDocument* document = new FrMainDocument();

    // Create main controller.
    // It takes care about all the stuff (i.e. init, manage and delete).
    FrMainController controller(mainWindow, document);
    controller.Initialize();

    if(!application.exec()){
      result = 1;
    }
  }
  else if(config.get("oldgui:disabled")==false) {

    /* ohinds 2009-02-02
     * commented because of the #ifdef USE_FRONTEND in
     * RtdisplayImage.cpp, which we should get rid of
    RtDisplayImage dispImg;
    if(!dispImg.open(config)) {
      cerr << "ERROR: could not open display" << endl;
      result = 1;
    }

    result = dispImg.svc();
    */
  }
  else { // just run once in the terminal with the passed config
    RtConfigFmriRun runConf;
    runConf.parseConfigFile(confFilename);
    conductor.configure(runConf);
    result = conductor.svc();
  }

  return result;
}

#endif
