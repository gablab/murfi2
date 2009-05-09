/******************************************************************************
 * RtConfigFmriExperiment.cpp defines a class that controls configuration of
 * an entire real-time fMRI experimental session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-11
 *
 *****************************************************************************/

static char *VERSION = "$Id: RtConfig.cpp 268 2008-10-13 19:54:35Z ohinds $";

#include"RtConfigFmriExperiment.h"

#include"RtExperiment.h"
#include"RtLimit.h"

#include"site_config.h"

#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "ace/Trace.h"

#include<cstdlib>

using namespace boost::filesystem;

// defaults

// dirs
static const string DEFAULT_SOFTWAREDIR(   "/sw/festr/trunk");
static const string DEFAULT_VOLUMEDIR(     "img/");
static const string DEFAULT_MASKDIR(       "mask/");
static const string DEFAULT_XFMDIR(        "xfm/");
static const string DEFAULT_CONFDIR(       "scripts/");
static const string DEFAULT_LOGDIR(        "log/");

// formats
static const string DEFAULT_VOLUMEFORMAT(  "nii");

// file names
static const string DEFAULT_VOLUMEFILESTEM("img");
static const string DEFAULT_LOGNAME(       "log");
static const string DEFAULT_LOGFILEEXT(    "rtl");
static const string DEFAULT_STUDYREFNAME(  "study_ref");
static const string DEFAULT_XFMFILEEXT(    "xfm");
static const string DEFAULT_SERIESXFMMOD(  "series");

// ports
static const unsigned int DEFAULT_INFOSERVERPORT(15001);

// copy constructor (called often)
RtConfigFmriExperiment::RtConfigFmriExperiment(const RtConfigFmriExperiment &other) 
  : RtConfig(other) {
}

// set default config info
// doesn't work
void RtConfigFmriExperiment::setDefaults() {
  //set("study:softwareDir", DEFAULT_SOFTWAREDIR);
  //set("study:siteID",      SITE_ID_NUMBER);
  //set("study:id",          getExperimentStudyID());
  //set("gui:disabled",      true);
  //set("oldgui:disabled",   true);
}

// validate the configuration
// checks for valid setup of different parts of the program
// returns true for success
bool RtConfigFmriExperiment::validateConfig() {
  ACE_TRACE(("RtConfigFmriExperiment::validateConfig"));

  // return value
  bool valid = true;

  // tmp storage vars
  path p;

  // check subject
  if(!isSet("study:subject:name")) {
    cerr << "ERROR: no subject name specified" << endl;
    valid = false;
  }

  // check subjects directory
  if(!isSet("study:subjectsDir")) {
    cerr << "ERROR: no subjects directory specified" << endl;
    valid = false;
  }

  // check for existance of studyDir
  p.operator=(get("study:subjectsDir").filepath() 
	      / get("study:subject:name").str());
  set("study:directory",p.string());
  path studyDir(p); // for local use

  if(!( exists(p) && is_directory(p) )) { 
   cerr << "ERROR: study directory " << p.string() << " is bad" << endl;
    valid = false;
  }

  // check or set the paths for all kinds of data

  // software directory
  if(!isSet("study:softwareDir")) { // not set, make default
    set("study:softwareDir",DEFAULT_SOFTWAREDIR);
  }

  // study id
  if(!isSet("study:id")) { // not set, make default
    set("study:id", getExperimentStudyID());
  }

  // site id
  if(!isSet("study:siteID")) { // not set, make default
    set("study:siteID", SITE_ID_NUMBER);
  }

  // gui 
  if(!isSet("gui:disabled")) { // not set, make default
    set("gui:disabled", true);
  }

  // old gui 
  if(!isSet("oldgui:disabled")) { // not set, make default
    set("oldgui:disabled", true);
  }

  // volume directory
  if(!isSet("study:volumeDir")) { // not set, make default
    p.operator=(studyDir / DEFAULT_VOLUMEDIR);
    set("study:volumeDir",p.string());
  }      
  else {
    p.operator=(get("study:volumeDir").filepath());
  }

  // create volume directory if it doesn't exist
  if(!( exists(p) && is_directory(p) )) { 
    cout << "creating volume directory: " << p.string() << endl;
    create_directory(p);
  }


  // mask directory
  if(!isSet("study:maskDir")) { // not set, make default
    p.operator=(studyDir / DEFAULT_MASKDIR);
    set("study:maskDir",p.string());
  }      
  else {
    p.operator=(get("study:maskDir").filepath());
  }
  
  // create mask directory if it doesn't exist
  if(!( exists(p) && is_directory(p) )) { 
    cout << "creating mask directory: " << p.string() << endl;
    create_directory(p);
  }


  // xfm directory
  if(!isSet("study:xfm:directory")) { // not set, make default
    p.operator=(studyDir / DEFAULT_XFMDIR);
    set("study:xfm:directory",p.string());
  }      
  else {
    p.operator=(get("study:xfm:directory").filepath());
  }
  
  // create xfm directory if it doesn't exist
  if(!( exists(p) && is_directory(p) )) { 
    cout << "creating xfm directory: " << p.string() << endl;
    create_directory(p);
  }


  // conf directory
  if(!isSet("study:confDir")) { // not set, make default
    p.operator=(studyDir / DEFAULT_CONFDIR);
    set("study:confDir",p.string());
  }      
  else {
    p.operator=(get("study:confDir").filepath());
  }
  
  // create conf directory if it doesn't exist
  if(!( exists(p) && is_directory(p) )) { 
    cout << "creating conf directory: " << p.string() << endl;
    create_directory(p);
  }


  // log directory
  if(!isSet("study:log:directory")) { // not set, make default
    p.operator=(studyDir / DEFAULT_LOGDIR);
    set("study:log:directory",p.string());
  }      
  else {
    p.operator=(get("study:log:directory").filepath());
  }
  
  // create log directory if it doesn't exist
  if(!( exists(p) && is_directory(p) )) { 
    cout << "creating log directory: " << p.string() << endl;
    create_directory(p);
  }


  // formats
  
  // set the default volume format if its not been set
  string volumeFormat;
  if(!isSet("study:volumeFormat")) { // not set, make default
    set("study:volumeFormat",DEFAULT_VOLUMEFORMAT);
  }
  else { // check that the secified format is valid
    if(get("study:volumeFormat").str() != DEFAULT_VOLUMEFORMAT) {
      cerr << "ERROR: unsupported volume format " 
	   << get("study:volumeFormat") << endl;
      valid = false;
    }
  }

  // set the default volume fileStem if its not been set
  string volumeFileStem;
  if(!isSet("study:volumeFileStem")) { // not set, make default
    set("study:volumeFileStem",DEFAULT_VOLUMEFILESTEM);
  }
  
  // filenames

  // check logfile name
  if(get("study:log:disabled")==false) {
    if(!isSet("study:log:filename")) {
      p.operator=(get("study:log:directory").filepath()
		  / (DEFAULT_LOGNAME + "." + DEFAULT_LOGFILEEXT));
      cout << "using default logfile name " << p.string() << endl;
      set("study:log:filename",p.string());
    }
  }

  // study reference volume
  if(!isSet("study:xfm:referenceVol")) {
    p.operator=(get("study:xfm:directory").filepath()
		/ (DEFAULT_STUDYREFNAME + "." 
		   + get("study:volumeFormat").str()));
    cout << "using default study reference volume name " << p.string() << endl;
    set("study:xfm:referenceVol",p.string());
  }


  // general setup

  // check info server
  if(get("infoserver:disabled")==false) {
    if(!isSet("infoserver:port")) {
      set("infoserver:port",DEFAULT_INFOSERVERPORT);
    }
    if((unsigned int) get("infoserver:port") < 1 
       || (unsigned int) get("infoserver:port") > MAX_TCPIP_PORT_NUM) {
      cerr << "WARNING: invalid port number for infoserver, disabling"
	   << endl;
      set("infoserver:disabled",true);
    }
  }


  return valid;
}

//******* computed values must have special functions

// get the filename for the series reference volume
string RtConfigFmriExperiment::getSeriesRefVolFilename(unsigned int series) {
  stringstream ss;
  ss << "series" << series << "_ref." << get("study:volumeFormat").str();

  path seriesFile(get("info:xfm:directory").filepath() / ss.str());
  return seriesFile.string();
}

// get the filename for the transformation file that takes the
// experiment reference volume into the space of the current series
string RtConfigFmriExperiment::getSeriesXfmFilename(unsigned int series) {
  stringstream ss;
  ss << "series" << series << "." << get("info:xfm:fileExt").str();

  path xfmFile(get("info:xfm:directory").filepath() / ss.str());
  return xfmFile.string();
}

// get the filename of a file transformed by using the series xfm on
// an input file
string RtConfigFmriExperiment::getSeriesXfmOutputFilename(unsigned int series,
						string input) {
  size_t ext = input.rfind('.');
  if(ext == string::npos) {
    ext = input.size();
  }

  stringstream sins;
  sins << "_series" << series;

  input.insert(ext, sins.str());

  return input;
}

// get the filename for the mask volume for a single series
string RtConfigFmriExperiment::getSeriesMaskFilename(unsigned int series,
						     string roiID) {
  stringstream ss;
  ss << "series" << series << "_" << roiID << "." 
     << get("study:volumeFormat").str();
  
  path p(get("study:maskDir").filepath() / ss.str());
  return p.string();
}

// determine if the reference volume already exists
// true if reference volume exists
bool RtConfigFmriExperiment::getStudyRefVolExists() {
  path p(get("study:xfm:referenceVol").filepath());
  return exists(p) && is_regular(p);
}

// build a filename for a volume in this experiment
//  in
//   series number
//   timepoint
//  out
//   absolute file string
string RtConfigFmriExperiment::getVolFilename(int _seriesNum,int _timepoint) {
  // five digit filename assumption here!
  char srnum[6];
  char acnum[6];
  sprintf(srnum,"%05d",_seriesNum);
  sprintf(acnum,"%05d",_timepoint);

  path p(get("study:volumeDir").filepath() 
	 / (get("study:volumeFileStem").str()
	    + "-" + srnum 
	    + "-" + acnum 
	    + "." + get("study:volumeFormat").str()));

  return p.string();
}

// gets the version
//  out:
//   cvs version string for this class
char *RtConfigFmriExperiment::getVersionString() {
  return VERSION;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


