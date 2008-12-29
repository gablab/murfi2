/******************************************************************************
 * declaration of a class that holds information about the loaded experiment
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-02
 *
 *****************************************************************************/

#ifndef RT_EXPERIMENT_H
#define RT_EXPERIMENT_H

#include"RtConductor.h"
#include"RtDataStore.h"

// set the paths relative to the subject directory for kinds of data
#define VOLUME_DIR    "img/"
#define VOLUME_FORMAT "nii"
#define MASK_DIR      "mask/"
#define XFM_DIR       "xfm/"
#define CONF_DIR      "scripts/"
#define LOG_DIR       "log/"

#define TOO_MANY_EXISTING_SERIES 10000

class RtExperiment {
public:

  //* static methods for retreiving info about the the experiment *//

  // get the software directory for this experiment
  static string getExperimentSoftwareDir();

  // get the unique ID number for this site
  static unsigned int getExperimentSiteID();

  // get the unique ID number for this study
  static unsigned int getExperimentStudyID();

  // get the subject directory this study
  static string getExperimentStudyDir();

  // get the unique ID number for this image series UID
  static unsigned int getSeriesNumFromUID(char *uid);

  // get the current experiment elapsed time in ms
  static time_t getExperimentElapsedTime();

  // get the current time in ms since
  static time_t getNow();

  // get the configuration for this experiment
  static RtConfig *getConfig();

  // get the data store for this experiment
  static RtDataStore *getDataStore();

  //* static methods for getting info about registrations *//

  // get the filename for the epi volume that serves as the reference
  // space to relate all acquired images during the experiment.
  //
  // note:
  // for registrations done during the realtime portion this
  // image will be the movable image, not the target image, sine an
  // inverse warp is usually more efficient if the later acquired
  // images are in the same space as the first of the series.
  static string getExperimentRefVolFilename();


  // build a filename for a volume in this experiment
  //  in
  //   series number
  //   timepoint
  //  out
  //   absolute file string
  static string getExperimentVolFilename(int _seriesNum,int _timepoint);

  // determine if the reference volume already exists
  static bool getExperimentRefVolExist();

  // get the filename for the epi volume that is the reference space
  // for a single series
  static string getSeriesRefVolFilename(unsigned int series);

  // get the filename of a file transformed by using the series xfm on
  // an input file
  static string getSeriesXfmOutputFilename(unsigned int series, string input);

  // get the filename for the transformation file that takes the
  // experiment reference volume into the space of the current series
  static string getSeriesXfmFilename(unsigned int series);

private:

  static string softwareDir;

  // private static study info members
  static map<string, unsigned int> uids;
  static unsigned int numExistingSeries;
  static unsigned int studyID;
  static string studyDir;
  static RtConfig *config;
  static time_t startTime;

  // setConfig, setstudyID, and setstudyDir
  // SHOULD ONLY BE CALLED FROM THE CONSTRUCTOR FOR THIS CLASS AND FROM
  // RTCONDUCTOR IF AN EXISTING STUDY IS BEING LOADED
  static void setConfig(RtConfig *config);
  static void setExperimentStudyID(string s);
  static void setExperimentStudyDir(string s);
  friend class RtConductor;


  RtExperiment();
  ~RtExperiment();

};

#endif
