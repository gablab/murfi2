/******************************************************************************
 * RtActivationEstimator.cpp is the implementation of a base class for
 * any method for estimation of activation
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#include"RtActivationEstimator.h"

string RtActivationEstimator::moduleString("voxel-accumcor");

// default constructor
RtActivationEstimator::RtActivationEstimator() : RtStreamComponent(), 
                                                 numConditions(0), 
                                                 numTrends(0), numMeas(0) {
  id = moduleString;
}

// constructor
RtActivationEstimator::RtActivationEstimator(unsigned int _numMeas) 
                                               : RtStreamComponent(), 
                                                 numConditions(0), 
                                                 numTrends(0) {
  id = moduleString;
  numMeas = _numMeas;
}

// destructor
RtActivationEstimator::~RtActivationEstimator() {
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationEstimator::processOption(const string &name, const string &text) {
  // look for known options
  if(name == "condition") {
    // expand conditions matrix
    conditions.set_size(numMeas,numConditions++);

    double el;
    for(unsigned int i = 0; unsigned int i1 = 0, i2 = text.find(" "); 1; 
	i++, i1 = i2+1, i2 = text.find(" ", i1)) {

      if(!RtConfigVal::convert<double>(el, 
		text.substr(i1,i2 == string::npos ? text.size()-i1 : i2-i1))) {
	continue;
      }
      conditions.put(i,numConditions,el);

      if(i2 == string::npos) {
	break;
      }
    }

    // fill the rest of the measurements as periodic stim
    for(unsigned int startind = i; i < numMeas; i++) {
      conditions.put(i,numConditions,conditions.get(i%startind,j));
    }

    return true;
  }
  if(name == "trends") {
    return RtConfigVal::convert<unsigned int>(numTrends,text);
  }
  else if(name == "measurements") {
    return RtConfigVal::convert<unsigned int>(numMeas,text);
  }

  return RtStreamComponent::processOption(name, text);
}  


// initialize
void RtActivationEstimator::init() {
  buildTrends();
}


// build the trend regressors
void RtActivationEstimator::buildTrends() {
  
  trends.set_size(numMeas, numTrends);
  for(i = 0; i < numTrends; i++) {
    for(j = 0; j < numMeas; j++) {
      switch(i) {
      case 0: // mean
	trends.put(i,j,1.0);
	break;
      case 1: // linear
	trends.put(i,j,j);
	break;
      default:
	trends.put(i,j,0.0);
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


