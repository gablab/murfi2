/******************************************************************************
 * RtConfigFmri.h is the header for class that controls xml configuration
 * reading specific to an fmri experiment.
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-11 
 * 
 *****************************************************************************/

#ifndef RTCONFIGFMRIEXPERIMENT_H
#define RTCONFIGFMRIEXPERIMENT_H


#include"RtConfig.h"

// class declaration
class RtConfigFmriExperiment : public RtConfig {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConfigFmriExperiment() : RtConfig() {}; 

  // copy constructor (called often)
  RtConfigFmriExperiment(const RtConfigFmriExperiment &other);

  // destructor
  virtual ~RtConfigFmriExperiment() {};

  // sets some default configuration info
  virtual void setDefaults();

  // validate the configuration
  // checks for valid setup of different parts of the program
  // returns true for success
  virtual bool validateConfig();


//******* computed values must have special functions

// get the filename for the transformation file that takes the
// experiment reference volume into the space of the current series
  string getSeriesXfmFilename(unsigned int series);

  // get the filename of a file transformed by using the series xfm on
  // an input file
  string getSeriesXfmOutputFilename(unsigned int series, string input);

  // get the filename for the epi volume that is the reference space
  // for a single series
  string getSeriesRefVolFilename(unsigned int series);

  // determine if the reference volume already exists
  // true if reference volume exists
  bool getStudyRefVolExists();

  // build a filename for a volume in this experiment
  //  in
  //   series number
  //   timepoint
  //  out
  //   absolute file string
  string getVolFilename(int _seriesNum,int _timepoint);  

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();  

protected:
  
};


#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


