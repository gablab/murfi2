/******************************************************************************
 * RtActivationEstimator.h is the header for a base class for any
 * method for estimation of activation
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#ifndef RTACTIVATIONESTIMATOR_H
#define RTACTIVATIONESTIMATOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>
#include<vnl/algo/vnl_convolve.h>

#include"RtStreamComponent.h"
#include"RtDataImage.h"

#define MAX_CONDITIONS 1

// class declaration
class RtActivationEstimator : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtActivationEstimator();

  // destructor
  ~RtActivationEstimator();


protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb) = 0;

  // process the configuration: only use this for cross module or global config
  // that is not available in the xml node for this stream component
  //  in 
  //   config class
  virtual bool processConfig(RtConfig &config);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // finish initialization tasks for run
  virtual bool finishInit();

  // build the trend regressors
  virtual void buildTrends();

  // number of timepoints
  unsigned int numMeas;        // total expected
  unsigned int numTimepoints;  // so far

  // condition regressors
  unsigned int numConditions; 
  vnl_matrix<double> *conditions;

  // trend regressors
  unsigned int numTrends; 
  vnl_matrix<double> *trends;

  // mask
  //RtMRIImage mask;
   
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


