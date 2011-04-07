/******************************************************************************
 * RtIncrementalGLM.h is the header for a class that estimates the
 * activation at each voxel incrementally using Gentleman's method
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-01
 *
 *****************************************************************************/

#ifndef RTINCREMENTALGLM_H
#define RTINCREMENTALGLM_H

#include"RtModelFit.h"
#include"RtLeastSquaresSolve.h"
#include"util/timer/timer.h"

// class declaration
class RtIncrementalGLM : public RtModelFit {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtIncrementalGLM();

  // destructor
  ~RtIncrementalGLM();

 protected:

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  virtual void initEstimation(const RtData &dat, RtMaskImage *mask);

  // make sure we are configured properly
  virtual bool validateComponentConfig();

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

  // one solver for each voxel 
  unsigned int numSolvers;
  RtLeastSquaresSolve **solvers;

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


