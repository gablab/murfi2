/******************************************************************************
 * RtSlideWinCor.h is the header for a class that estimates the
 * instantaneous activation at each voxel in an image using a sliding
 * window correlation analysis from Gembris 2000
 *
 *****************************************************************************/

#ifndef RTSLIDEWINCOR_
#define RTSLIDEWINCOR_

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtActivationEstimator.h"

// class declaration
class RtSlideWinCor : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtSlideWinCor();

  // destructor
  ~RtSlideWinCor();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option in name of the option to process val  text of the option node
  virtual bool processOption(const string &name, const string &text, const map<string,string> &attr);

  // start a logfile 
  virtual void startDumpAlgoVarsFile();

  // if we should reinit
  bool needsInit;

  // condition column
  unsigned int conditionColumn;

  // length of window
  unsigned int windowLen;
  
  // number of detrending vectors
  unsigned int L;

  // number of data points to process
  unsigned int numData;
  
  // subsidiary variables
  double b, d, rOld, rNew, xOld, xNew;
  vnl_vector<double> *a;
  vnl_vector<double> *c;
  vnl_vector<double> *e;

  // store x values over timesteps for beta calculations
  vnl_vector<double> *rWindow;
  vnl_matrix<double> *xWindow; 
  
  // store value for stat calculation
  vnl_vector<double> *rs;
};

#endif /*RTSLIDEWINCOR_*/

/*****************************************************************************
 * $Source:  $
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
