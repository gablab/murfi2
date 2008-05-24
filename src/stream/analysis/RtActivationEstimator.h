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
#include"RtMaskImage.h"

#include<fstream>
using namespace std;

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

  // set the desired probability threshold
  void setProbThreshold(double p);

  // get the desired probability threshold
  double  getProbThreshold();

  // set whether stat thresholds should reflect multiple comparisons corrections
  void setCorrectMultipleComparisons(bool correct);

  // get whether stat thresholds should reflect multiple comparisons corrections
  bool getCorrectMultipleComparisons();

  // get the desired t statistic threshold
  // in
  //  dof: number of degrees of freedom
  // out
  //  t statistic threshold for the current probability threshold and
  //  mulitple comparisons correction state
  double getTStatThreshold(unsigned int dof);

protected:

  // builds an hrf vector
  //
  // in
  //  sampleRate: temporal precision in milliseconds
  //  length:     length of the HRF in milliseconds
  // out
  //  vnl_vector HRF
  void buildHRF(vnl_vector<double> &hrf, double tr,
		double sampleRate, double length);

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

  // build the condition regressors
  virtual void buildConditions();

  // build the trend regressors
  virtual void buildTrends();

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  virtual void initEstimation(RtMRIImage &image);

  // start a logfile 
  virtual void startDumpAlgoVarsFile();  

  // sets the latest result of processing
  //  in
  //   data result
  virtual void setResult(RtStreamMessage *msg, RtData *data);

  bool needsInit;

  double tr;

  // number of timepoints
  unsigned int numMeas;        // total expected
  unsigned int numTimepoints;  // so far

  // condition regressors
  unsigned int numConditions;
  vnl_matrix<double> conditions;
  unsigned int conditionShift;  // shift all condition regressors
  bool modelEachBlock;
  unsigned int blockLen;

  // trend regressors
  unsigned int numTrends;
  vnl_matrix<double> trends;

  // probability of false positive at which to threshold activations
  double probThreshold;

  // whether the probability threshold should be corrected for mulitple
  // comparisons
  bool correctForMultiComps;

  // number of statistical comparisons
  unsigned int numComparisons;

  // id of roi this represents
  string roiID;

  /// mask ///
  RtMaskImage mask;
  enum MaskSource {
    THRESHOLD_FIRST_IMAGE_INTENSITY,
    LOAD_FROM_FILE,
    PASSED_FROM_INSIDE
  } maskSource;

  // parameters for different mask sources
  double maskIntensityThreshold;
  string maskFilename;
  bool mosaicMask;
  bool flipLRMask;
  bool putMaskOnMessage;

  // whether to save the resulting t map
  bool saveTVol;
  bool unmosaicTVol;
  string saveTVolFilename;

  // whether to save the result as a mask
  bool savePosResultAsMask;
  bool unmosaicPosMask;

  bool saveNegResultAsMask;
  bool unmosaicNegMask;

  // filename to save mask to
  string savePosAsMaskFilename;
  string saveNegAsMaskFilename;

  // for logging the analysis process
  bool dumpAlgoVars;
  string dumpAlgoVarsFilename;
  ofstream dumpFile;
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


