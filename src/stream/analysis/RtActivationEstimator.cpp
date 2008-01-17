/******************************************************************************
 * RtActivationEstimator.cpp is the implementation of a base class for
 * any method for estimation of activation
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#include"RtActivationEstimator.h"

#include<gnuplot_i_vxl.h>

string RtActivationEstimator::moduleString("voxel-accumcor");

// default constructor
RtActivationEstimator::RtActivationEstimator() : RtStreamComponent(), 
					         numMeas(0), numConditions(0), 
                                                 numTrends(0) {
  id = moduleString;

  trends = conditions = NULL;
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
  if(name == "trends") {
    return RtConfigVal::convert<unsigned int>(numTrends,text);
  }

  return RtStreamComponent::processOption(name, text);
}  


// process gloabl config info or config from other modules
bool RtActivationEstimator::processConfig(RtConfig &config) {
  
  if(config.isSet("scanner:measurements")) {
    numMeas = config.get("scanner:measurements");
  } 
  else {
    cerr << "error: number of measurements has not been set" << endl;
    return false;
  }

  return true;
}

// finish initialization and prepare to run
bool RtActivationEstimator::finishInit() {
  buildTrends();

  // convolve the conditions with hrf (cannonical from SPM)

  // FOR NOW ASSUME A TR OF 2 SECONDS 
  double hrf_da[] = {
    0,
    0.08656608099364,
    0.37488823647169,
    0.38492338174546,
    0.21611731564656,
    0.07686956525508,
    0.00162017719800,
    -0.03060781173404,
    -0.03730607813300,
    -0.03083737159887,
    -0.02051613335212,
    -0.01164416374906,
    -0.00582063147183,
    -0.00261854249819,
    -0.00107732374409,
    -0.00041044352236,
    -0.00014625750688
  };

  vnl_vector<double> hrf(17);
  hrf.copy_in(hrf_da);

  //Gnuplot g1, g2;
  //g1 = Gnuplot("lines");
  //g1.plot_x(hrf,"hrf");

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

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


