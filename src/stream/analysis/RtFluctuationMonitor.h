/******************************************************************************
 * RtFluctuationMonitor.h is the header for a class that estimates the
 * activation at each voxel in a single image using a GLM-based
 * correlation analysis based on Hids, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#ifndef RTFLUCTUATIONMONITOR_H
#define RTFLUCTUATIONMONITOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtActivationEstimator.h"
#include"RtOutput.h"

#include"RtLeastSquaresSolve.h"

#include<vector>
using namespace std;

// class declaration
class RtFluctuationMonitor : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtFluctuationMonitor();

  // destructor
  ~RtFluctuationMonitor();

  // receive a message that stimulus has been triggered
  void receiveStimTriggered();

  // look for triggered events so we can stop estimation during task
  virtual void setData(RtData *data);

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // start a logfile 
  virtual void startDumpAlgoVarsFile();

  //// parameters of the activation estimation algorithm

  // if we should reinit
  bool needsInit;

  // number of data points to process
  unsigned int numData;

  // one solver for each voxel 
  RtLeastSquaresSolve **solvers;

  // store the per pixel sum of squared error
  RtActivation *absEstErrSum;
  int numDataInErrSum;

  // the amount of data to use in the estimation of the error
  int numDataPointsForErrEst;

  // for task triggering based on activation sum
  bool triggerStim;
  bool isTriggered;  
  int afterTriggerSkip;
  int numImagesSinceTrigger;

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


