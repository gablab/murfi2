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
RtConfig *RtExperiment::config;
string RtExperiment::softwareDir;
time_t RtExperiment::startTime;
unsigned int RtExperiment::studyID;
string RtExperiment::studyDir;
map<string, unsigned int> RtExperiment::uids;
unsigned int RtExperiment::numExistingSeries;


//* methods for retreiving info about the the experiment *//

// get the subject directory this study
string RtExperiment::getExperimentSoftwareDir() {
  return softwareDir;
}

// get the unique ID number for this site
unsigned int RtExperiment::getExperimentSiteID() {
  return SITE_ID_NUMBER;
}

// get the unique ID number for this study
unsigned int RtExperiment::getExperimentStudyID() {
  return studyID;
}

// get the subject directory this study
string RtExperiment::getExperimentStudyDir() {
  return studyDir;
}

// get the unique ID number for this image series UID
// checks if this uid exists 
//  if so returns its index+1
//  if not adds it to the end and returns the length for the list
unsigned int RtExperiment::getSeriesNumFromUID(char *uid) {
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
time_t RtExperiment::getExperimentElapsedTime() {
  return getNow()-startTime;
}
  
// get the current time in ms since 
time_t RtExperiment::getNow() {
  return time(NULL);
}

// get the configuration for this experiment
RtConfig *RtExperiment::getConfig() {
  return config;
}

// get the data store for this experiment
RtDataStore *RtExperiment::getDataStore() {
  if(config == NULL || config->getConductor() == NULL) {
    return NULL;
  }

  return config->getConductor()->getDataStore();
}

  
// set studyID and set studyDir
// SHOULD ONLY BE CALLED FROM THE CONSTRUCTOR FOR THIS CLASS AND FROM
// RTCONDUCTOR IF AN EXISTING STUDY IS BEING LOADED
void RtExperiment::setExperimentStudyID(string s) {
  stringstream ss(s);
  ss >> studyID;
}

void RtExperiment::setExperimentStudyDir(string s) {
  stringstream ss(s);
  ss >> studyDir;
}

void RtExperiment::setConfig(RtConfig *_config) {
  config = _config;

  softwareDir = config->get("study:festrDir").str();

  // initialize the experiment
  // 1) makes sure the subject directory and all required subdirectories exist
  // 2) ??

  setExperimentStudyDir(
	 config->get("study:subjectsDir").str()
       + "/" 
       + config->get("study:subject").str()
       + "/" 
			);

  // make the directories (ugly plaform specific code)
#ifndef WIN32
  stringstream command;

  command.str("");
  command << "mkdir -p " 
	  << RtExperiment::getExperimentStudyDir()
	  << VOLUME_DIR;
  system(command.str().c_str());

  // count existing series for this experiment
  cout << "searching for existing series... " << endl;
  numExistingSeries = -1;
  do {
    numExistingSeries++;
    command.str("");
    command << "ls "
	    << RtExperiment::getExperimentVolFilename(numExistingSeries+1,1)
	    << " 2> /dev/null ";
  } while(!system(command.str().c_str()) 
	  && numExistingSeries < TOO_MANY_EXISTING_SERIES);
  cout << "found " << numExistingSeries << " existing series" << endl;

  command.str("");
  command << "mkdir -p " 
	  << RtExperiment::getExperimentStudyDir()
	  << MASK_DIR;
  system(command.str().c_str());

  command.str("");
  command << "mkdir -p " 
	  << RtExperiment::getExperimentStudyDir()
	  << XFM_DIR;
  system(command.str().c_str());

  command.str("");
  command << "mkdir -p " 
	  << RtExperiment::getExperimentStudyDir()
	  << CONF_DIR;
  system(command.str().c_str());

  command.str("");
  command << "mkdir -p " 
	  << RtExperiment::getExperimentStudyDir()
	  << LOG_DIR;
  system(command.str().c_str());
#endif
	 
}

//* static methods for getting info about registrations *//

// get the filename for the epi volume that serves as the reference
// space to relate all acquired images during the experiment.
//
// note:
// for registrations done during the realtime portion this
// image will be the movable image, not the target image, because an
// inverse warp is usually more efficient if the later acquired
// images are in the same space as the first of the series.
string RtExperiment::getExperimentRefVolFilename() {
  return studyDir + XFM_DIR + "study_ref.nii";
}

// determine if the reference volume already exists
bool RtExperiment::getExperimentRefVolExist() {
  RtMRIImage refImg;
  return refImg.readHeader(RtExperiment::getExperimentRefVolFilename());
}

// get the filename for the epi volume that is the reference space
// for a single series
string RtExperiment::getSeriesRefVolFilename(unsigned int series) {
  stringstream ss;
  ss << studyDir << XFM_DIR << "series" << series << "_ref.nii";
  return ss.str();
}

// build a filename for a volume in this experiment
//  in
//   series number
//   timepoint
//  out
//   absolute file string
string RtExperiment::getExperimentVolFilename(int _seriesNum,int _timepoint) {
  // five digit filename assumption here!
  char srnum[6];
  char acnum[6];
  sprintf(srnum,"%05d",_seriesNum);
  sprintf(acnum,"%05d",_timepoint);

  stringstream s;
  s << config->get("study:dir").str() 
    << VOLUME_DIR 
    << "/img"
    << "-" 
    << srnum 
    << "-" 
    << acnum 
    << "."
    << VOLUME_FORMAT;

  return s.str();
}

// get the filename of a file transformed by using the series xfm on
// an input file
string RtExperiment::getSeriesXfmOutputFilename(unsigned int series,
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

// get the filename for the transformation file that takes the
// experiment reference volume into the space of the current series
string RtExperiment::getSeriesXfmFilename(unsigned int series) {
  stringstream ss;
  ss << studyDir << XFM_DIR << "series" << series << ".xfm";
  return ss.str();
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

  config = NULL;
};

RtExperiment::~RtExperiment() {};

