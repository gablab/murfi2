/******************************************************************************
 * RtActivationEstimator.cpp is the implementation of a base class for
 * any method for estimation of activation
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#include"RtActivationEstimator.h"

#include<gnuplot_i_vxl.h>
#include"gsl/gsl_cdf.h"

// for gamma functions
#include<vnl/vnl_gamma.h>

string RtActivationEstimator::moduleString("voxel-accumcor");


// default constructor
RtActivationEstimator::RtActivationEstimator() : RtStreamComponent() {

  // standard init
  id = moduleString;
  trends = conditions = NULL;
  numTrends = numConditions = numMeas = 0;
  numTimepoints = 0;
  conditionShift = 0;

  // default values for probability thresholding
  probThreshold = 0.05;
  numComparisons = 0;
  correctForMultiComps = true;

  // default values for mask
  maskSource = THRESHOLD_FIRST_IMAGE_INTENSITY;
  maskIntensityThreshold = 0.5;

  needsInit = true;
}

// destructor
RtActivationEstimator::~RtActivationEstimator() {
  if(trends != NULL) {
    delete trends;
  }

  if(conditions != NULL) {
    delete conditions;
  }
}


// set the desired probability threshold
void RtActivationEstimator::setProbThreshold(double p) {
  probThreshold = p;
}

// get the desired probability threshold
double RtActivationEstimator::getProbThreshold() {
  return probThreshold;
}

// set whether stat thresholds should reflect multiple comparisons corrections
void RtActivationEstimator:: setCorrectMultipleComparisons(bool correct) {
  correctForMultiComps = correct;
}

// get whether stat thresholds should reflect multiple comparisons corrections
bool RtActivationEstimator::getCorrectMultipleComparisons() {
  return correctForMultiComps;
}

// get the desired t statistic threshold
double RtActivationEstimator::getTStatThreshold(unsigned int dof) {
  return fabs(gsl_cdf_tdist_Pinv(probThreshold 
		    / (correctForMultiComps ? numComparisons : 1.0),dof));
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationEstimator::processOption(const string &name, const string &text) {

  // look for known options
  if(name == "condition") { // load the condition vector
    numConditions++;

    if(numConditions == 1) { // allocate condition matrix
      conditions = new vnl_matrix<double>(numMeas,MAX_CONDITIONS);
    }
    else if(numConditions > MAX_CONDITIONS) {
      cerr << "warning: max number of conditions exceeded." << endl;
      return false;
    }
    
    double el;
    unsigned int i = 0;
    for(unsigned int i1 = 0, i2 = text.find(" "); 1; 
	i++, i1 = i2+1, i2 = text.find(" ", i1)) {

      if(!RtConfigVal::convert<double>(el, 
				       text.substr(i1, 
						   i2 == string::npos ? text.size()-i1 : i2-i1))) {
	continue;
      }
      conditions->put(i,numConditions-1,el);

      if(i2 == string::npos) { // test if we are on the last one
	break;
      }
    }

    // fill the rest of the measurements as periodic stim
    for(unsigned int startind = i+1; i < numMeas; i++) {
      conditions->put(i,numConditions-1,conditions->get(i%startind,0));
    }

    return true;
  }
  if(name == "conditionshift") {
    return RtConfigVal::convert<unsigned int>(conditionShift,text);
  }
  if(name == "trends") {
    return RtConfigVal::convert<unsigned int>(numTrends,text);
  }
  if(name == "probThreshold") {
    return RtConfigVal::convert<double>(probThreshold,text);
  }  
  if(name == "correctForMultiComps") {
    return RtConfigVal::convert<bool>(correctForMultiComps,text);
  }  
  if(name == "maskSource") {
    // match type string
    if(text == "thresholdFirstImageIntensity") {
      maskSource = THRESHOLD_FIRST_IMAGE_INTENSITY;
    }
    else if(text == "loadFromFile") {
      maskSource = LOAD_FROM_FILE;
    }
    else if(text == "passedFromInside") {
      maskSource = PASSED_FROM_INSIDE;
    }
  }
  if(name == "maskFilename") {
    mask.setFilename(text);
    return true;
  }
  if(name == "roiID") {
    roiID = text;
    return true;
  }  
  if(name == "maskIntensityThreshold") {
    return RtConfigVal::convert<double>(maskIntensityThreshold,text);
  }  
  if(name == "savePosAsMask") {
    return RtConfigVal::convert<bool>(savePosResultAsMask,text);
  }
  if(name == "saveNegAsMask") {
    return RtConfigVal::convert<bool>(saveNegResultAsMask,text);
  }
  if(name == "savePosAsMaskFilename") {
    savePosAsMaskFilename = text;
    return true;
  }
  if(name == "saveNegAsMaskFilename") {
    saveNegAsMaskFilename = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text);
}  


// process gloabl config info or config from other modules
bool RtActivationEstimator::processConfig(RtConfig &config) {
  
  if(config.isSet("scanner:measurements")) {
    numMeas = config.get("scanner:measurements");
  } 
  if(config.isSet("scanner:tr")) {
    tr= config.get("scanner:tr");
  } 
  else {
    cerr << "error: number of measurements has not been set" << endl;
    return false;
  }

  return true;
}

// build a gamma pdf with shape and scale parms
// see http://www.itl.nist.gov/div898/handbook/apr/section1/apr165.htm
double gammaPDF(double t, double a, double b) {
  return pow(b,a)/vnl_gamma(a) * pow(t,a-1) * exp(-b*t);
}


// builds an hrf vector from the difference of two gammas
//
// in
//  tr:           repetition time in seconds
//  samplePeriod: temporal precision in seconds
//  length:       length of the HRF in seconds
// out
//  vnl_vector HRF
void RtActivationEstimator::buildHRF(vnl_vector<double> &hrf,
				     double tr,
				     double samplePeriod, 
				     double length) {

  double timeToPeakPos = 6;
  double timeToPeakNeg = 16;
  double posToNegRatio = 6;

  double dt = tr*samplePeriod;
  hrf.set_size((int)ceil(length/tr+1));
  //double *hrf_da = new double[(int)ceil(length/tr+1)];
  for(unsigned int i = 0, t = 0; t <= (unsigned int) ceil(length/samplePeriod); 
      t += (unsigned int) ceil(tr/samplePeriod), i++) {
//    hrf_da[i] = gammaPDF(t/tr,timeToPeakPos,dt)
//      - gammaPDF(t/tr,timeToPeakNeg,dt)/posToNegRatio;
    hrf.put(i, gammaPDF(t/tr,timeToPeakPos,dt)
  	    - gammaPDF(t/tr,timeToPeakNeg,dt)/posToNegRatio);
      
//    cout << i << " " << t << " " << hrf[i] << endl;
  }
  //hrf.set_size((int)ceil(length/tr+1));
  //hrf.copy_in(hrf_da);

  //cout << "jdsgljd" << endl;
  hrf.normalize();

  // sum
  for(unsigned int i = 0; i < hrf.size(); i++) {
    cout << hrf[i] << endl;
  }

//  // FOR NOW ASSUME 
//  // sampleRate = 2000 milliseconds
//  // length = 32000 milliseconds
//  double hrf_da[] = {
//    0,
//    0.08656608099364,
//    0.37488823647169,
//    0.38492338174546,
//    0.21611731564656,
//    0.07686956525508,
//    0.00162017719800,
//    -0.03060781173404,
//    -0.03730607813300,
//    -0.03083737159887,
//    -0.02051613335212,
//    -0.01164416374906,
//    -0.00582063147183,
//    -0.00261854249819,
//    -0.00107732374409,
//    -0.00041044352236,
//    -0.00014625750688
//  };
//
//  hrf.set_size(17);
//  hrf.copy_in(hrf_da);
//
//  for(unsigned int i = 0; i < hrf.size(); i++) {
//    cout << hrf_da[i] << endl;
//  }
}

// finish initialization and prepare to run
bool RtActivationEstimator::finishInit() {
  buildTrends();

  // mask from file
  if(maskSource == LOAD_FROM_FILE) {
    mask.load();
  }

  // convolve the conditions with hrf (cannonical from SPM)
  vnl_vector<double> hrf;
  buildHRF(hrf, tr, 1/16.0, 32);

  // debugging
//static Gnuplot g1;
//g1 = Gnuplot("lines");
//g1.plot_x(hrf,"hrf");

  // incorporate condition shift
  if(conditionShift > 0 && conditionShift < numMeas) {
    vnl_vector<double> shiftdelta(numMeas,0.0);
    shiftdelta.put(conditionShift,1);

    for(unsigned int i = 0; i < numConditions; i++) {
      vnl_vector<double> col = conditions->get_column(i);
      vnl_vector<double> shiftcol = vnl_convolve(col,shiftdelta);
      col.update(shiftcol.extract(col.size()));
      conditions->set_column(i,col);
    }  
  }

  // convolve each condition with the hrf
  for(unsigned int i = 0; i < numConditions; i++) {
    vnl_vector<double> col = conditions->get_column(i);
    vnl_vector<double> convhrfcol = vnl_convolve(col,hrf);
    col.update(convhrfcol.extract(col.size()));
    conditions->set_column(i,col);
  }

  return true;
}

// build the trend regressors
void RtActivationEstimator::buildTrends() {
  trends = new vnl_matrix<double>(numMeas, numTrends);

  for(unsigned int i = 0; i < numTrends; i++) {
    for(unsigned int j = 0; j < numMeas; j++) {
      switch(i) {
      case 0: // mean
	trends->put(j,i,1.0);
	break;
      case 1: // linear
	trends->put(j,i,j+1);
	break;
      default:
	trends->put(j,i,0.0);
	break;
      }
    }
  }

}

// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtActivationEstimator::initEstimation(RtMRIImage &image) {
  // mask from intensity threshold
  if(maskSource == THRESHOLD_FIRST_IMAGE_INTENSITY) {
    numComparisons 
      = mask.initByMeanIntensityThreshold(image, maskIntensityThreshold);
  }

  needsInit = false;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


