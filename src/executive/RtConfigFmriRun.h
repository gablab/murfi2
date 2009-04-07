/******************************************************************************
 * RtConfigFmriRun.h is the header for class that controls xml configuration
 * reading specific to a single run of an fmri experiment.
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-11 
 * 
 *****************************************************************************/

#ifndef RTCONFIGFMRIRUN_H
#define RTCONFIGFMRIRUN_H


#include"RtConfig.h"

// class declaration
class RtConfigFmriRun : public RtConfig {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConfigFmriRun() : RtConfig() {};

  // copy constructor (called often)
  RtConfigFmriRun(const RtConfigFmriRun &other) : RtConfig(other) {};

  // assignment operator (called often)
  void operator=(const RtConfigFmriRun &them) {
    parms = them.parms;
  };

  // destructor
  virtual ~RtConfigFmriRun() {};

  // sets some default configuration info
  virtual void setDefaults();

  // validate the configuration
  // checks for valid setup of different parts of the program
  // returns true for success
  virtual bool validateConfig();

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


