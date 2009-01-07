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
#include"RtExperiment.h"
#include"RtDataImage.h"
#include"RtDataIDs.h"
#include"RtMaskImage.h"

#include<vector>
#include<fstream>
using namespace std;

#define MAX_CONDITIONS 1024

typedef enum {
  TREND = 0,
  MOTION,
  EVENT
} Nuisance;

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
  double getProbThreshold();

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

  // get the hrf vector
  virtual vnl_vector<double> &getHrf();

  // loads an hrf vector from a file
  virtual bool loadHrfFile(vnl_vector<double> &hrf, string filename);

  // builds an hrf vector
  //
  // in
  //  sampleRate: temporal precision in milliseconds
  //  length:     length of the HRF in milliseconds
  // out
  //  vnl_vector HRF
  virtual void buildHrf(vnl_vector<double> &hrf, double tr,
			double sampleRate, double length,
			double timeToPeakPos, double timeToPeakNeg,
			double posToNegRatio);

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
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
			     const map<string,string> &attr);

  // finish initialization tasks for run
  virtual bool finishInit();

  // build the condition regressors
  virtual void buildConditions();

  // test if a condition index is a derivative (index ignores trend regressors)
  virtual bool conditionIsDerivative(unsigned int index);

  // build the trend regressors
  virtual void buildNuisance();

  // retreive the number of nuisance regressors in this model
  virtual unsigned int getNumNuisanceRegressors();

  // retreive the column index of a nuisance regressor
  virtual unsigned int getNuisanceColumn(Nuisance, unsigned int index);

  // build the event regressor
  virtual void buildEventRegressor(unsigned int length);

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  virtual void initEstimation(RtMRIImage &image);

  // common processing for a single acquisition
  // in
  //  current image
  virtual double *getDesignMatrixRow(unsigned int timepoint);

  // start a logfile
  virtual void startDumpAlgoVarsFile();

  // sets the latest result of processing
  //  in
  //   data result
  virtual void setResult(RtStreamMessage *msg, RtData *data);

  bool needsInit;

  // template data id of the data volumes 
  RtDataID templateDataID;

  // repetition time of the MR acquisitions
  double tr;

  // number of timepoints
  unsigned int numMeas;        // total expected
  unsigned int numTimepoints;  // so far

  // hrf spec
  vnl_vector<double> hrf;
  bool hrfIsBuilt;

  // cannonical hrf parameters
  double hrfSamplePeriod;      // temporal precision for hrf sampling (s)
  double hrfLength;            // seconds
  double hrfTimeToPeakPos;
  double hrfTimeToPeakNeg;
  double hrfPosToNegRatio;

  // load hrf from file
  bool loadHrf;
  string hrfFilename;

  // condition regressors
  unsigned int numConditions;
  vnl_matrix<double> conditions;
  vector<string> conditionNames;
  unsigned int conditionShift;

  bool modelEachBlock;
  unsigned int blockLen;

  // nuisance regressors
  vnl_matrix<double> nuisance;

  unsigned int numTrends;

  bool modelMotionParameters;
  bool modelTemporalDerivatives;

  // single event regressors
  bool modelEvents;
  unsigned int eventDuration;
  bool convolveEventsWithHrf;
  vnl_vector<double> eventRegressor;
  unsigned int eventRegressorLength;
  unsigned int maxNumEvents;
  unsigned int numEvents;




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
    NO_MASK,
    THRESHOLD_FIRST_IMAGE_INTENSITY,
    LOAD_FROM_FILE,
    PASSED_FROM_INSIDE
  } maskSource;

  // parameters for different mask sources
  double maskIntensityThreshold;
  string maskFilename;
  bool alignMask;
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


