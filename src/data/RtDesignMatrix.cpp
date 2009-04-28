/******************************************************************************
 * RtDesignMatrix.cpp defines a class that represents a model's basis
 * functions
 *
 * NOTE: much of the funcitonality is inhereted from vnl_matrix, so see this
 * help page for usage of this class:
 * http://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__matrix.html
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-21
 *
 *****************************************************************************/

#include"RtDesignMatrix.h"

#include"RtExperiment.h"
#include"RtMotion.h"
#include"RtDataIDs.h"
#include"RtLimit.h"

#include<vcl_istream.h>
#include<fstream>
#include<limits>

#include<vnl/algo/vnl_convolve.h>
#include"gsl/gsl_cdf.h"
#include<vnl/vnl_gamma.h>
#include<vnl/algo/vnl_svd.h>

#include"debug_levels.h"

RtDesignMatrix::RtDesignMatrix() : RtData(), vnl_matrix<double>() {

  vnl_matrix<double>::set_size(0,0);

  isBuilt = false;

  numInputConditions = 0;
  numAddedColumns = 0;
  numMeas = 0;

  // neural signal bases
  conditionShift = 20;
  modelEachBlock = false;
  blockLen = 0;

  // hrf parms
  hrfIsBuilt = false;

  hrfSamplePeriod = 1/16.0;
  hrfLength = 32;
  hrfTimeToPeakPos = 6;
  hrfTimeToPeakNeg = 16;
  hrfPosToNegRatio = 6;

  loadHrf = false;
  hrfFilename = "unset";


  // nuisance bases
  maxTrendOrder = 1;

  modelMotionParameters = false;
  modelTemporalDerivatives = false;

  modelEvents = false;
  maxNumEvents = 0;
  eventDuration = 0;
  numEvents = 0;

  modelArtifacts = false;
  maxNumArtifacts = 0;
  numArtifacts = 0;

  dataID.setDataName(NAME_DESIGN);
}


// configure the design matrix from an xml element
bool RtDesignMatrix::configure(TiXmlElement *designEle,
			       const RtConfig &config) {
  string name;
  TiXmlElement *optionElmt;

  bool ret = true;

  // iterate over options
  for(TiXmlNode *option = 0;
      (option = designEle->IterateChildren("option", option)); ) {
    if(option->Type() != TiXmlNode::ELEMENT)
      continue;

    optionElmt = (TiXmlElement*) option;
    if(TIXML_SUCCESS != optionElmt->QueryValueAttribute("name", &name)) {
      continue;
    }

    // build the map between atrribute names and values
    map<string,string> attr = RtConfig::getAttributeMap(*optionElmt);

    // figure out which option we have and process it
    if(!processOption(name, optionElmt->GetText(), attr)) {
      ret = false;      
    }
  }

  return ret;
}


// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtDesignMatrix::processOption(const string &name,
			       const string &text,
			       const map<string,string> &attrMap) {

  if(name == "disabled") {
    return true;
  }
  // look for known options
  if(name == "condition") { // load the condition vector
    numInputConditions++;

    if(numInputConditions == 1) { // allocate condition matrix
      inputConditions.clear();
      inputConditions.set_size(numMeas,MAX_CONDITIONS);
      inputConditions.fill(0);
      conditionNames.reserve(MAX_CONDITIONS);
    }
    else if(numInputConditions > MAX_CONDITIONS) {
      cerr << "warning: max number of conditions exceeded." << endl;
      return false;
    }

    // find the name, or just name it a number
    map<string,string>::const_iterator condName
      = attrMap.find("conditionName");
    if(condName == attrMap.end()) {
      string nameNum = "condition";
      nameNum+=numInputConditions;
      conditionNames.push_back(nameNum);
    }
    else {
      conditionNames.push_back((*condName).second);
    }

    // parse the text string into a condition vector
    double el;
    size_t i = 0;
    for(size_t i1 = 0, i2 = text.find(" "); 1;
	i++, i1 = i2+1, i2 = text.find(" ", i1)) {

      if(!RtConfigVal::convert<double>(el,
	  text.substr(i1,i2 == string::npos ? text.size()-i1 : i2-i1))) {
	continue;
      }
      inputConditions.put(i,numInputConditions-1,el);

      if(i2 == string::npos) { // test if we are on the last one
	// set the blockLen if its not been set
	if(blockLen == 0) {
	  blockLen = i+1;
	  cout << "auto setting blockLen to " << blockLen << endl;
	}

	break;
      }
    }

    // fill the rest of the measurements as periodic stim
    for(; i < numMeas; i++) {
      inputConditions.put(i,numInputConditions-1,
			  inputConditions.get(i%blockLen,
					      numInputConditions-1));
    }

    return true;
  }
  if(name == "conditionShift") {
    return RtConfigVal::convert<unsigned int>(conditionShift,text);
  }
  if(name == "modelEachBlock") {
    return RtConfigVal::convert<bool>(modelEachBlock,text);
  }
  if(name == "hrfSamplePeriod") {
    return RtConfigVal::convert<double>(hrfSamplePeriod,text);
  }
  if(name == "hrfLength") {
    return RtConfigVal::convert<double>(hrfLength,text);
  }
  if(name == "hrfTimeToPeakPos") {
    return RtConfigVal::convert<double>(hrfTimeToPeakPos,text);
  }
  if(name == "hrfTimeToPeakNeg") {
    return RtConfigVal::convert<double>(hrfTimeToPeakNeg,text);
  }
  if(name == "hrfPosToNegRatio") {
    return RtConfigVal::convert<double>(hrfPosToNegRatio,text);
  }
  if(name == "hrfFilename") {
    hrfFilename = text;
    loadHrf = true;
    return true;
  }
  if(name == "modelMotionParameters") {
    return RtConfigVal::convert<bool>(modelMotionParameters,text);
  }
  if(name == "modelTemporalDerivatives") {
    return RtConfigVal::convert<bool>(modelTemporalDerivatives,text);
  }
  if(name == "modelEvents") {
    return RtConfigVal::convert<bool>(modelEvents,text);
  }
  if(name == "eventDuration") {
    return RtConfigVal::convert<unsigned int>(eventDuration,text);
  }
  if(name == "maxNumEvents") {
    return RtConfigVal::convert<unsigned int>(maxNumEvents,text);
  }
  if(name == "modelArtifacts") {
    return RtConfigVal::convert<bool>(modelArtifacts,text);
  }
  if(name == "maxNumArtifacts") {
    return RtConfigVal::convert<unsigned int>(maxNumArtifacts,text);
  }
  if(name == "maxTrendOrder") {
    return RtConfigVal::convert<unsigned int>(maxTrendOrder,text);
  }

  cout << "WARNING: option " << name << " unrecognized and ignored" << endl;
  return true;
}

// build the design
bool RtDesignMatrix::build() {

  if(numMeas == 0) {
    cerr << "ERROR: refusing to build() design matrix with zero rows. did you forget to call setNumMeas()?" << endl;
    return false;
  }

  // setup the size
  unsigned int numConditions = getNumConditionBases();
  unsigned int numNuisance = getNumNuisanceBases();

  columnNames.clear();
  columnNames.reserve(numConditions + numNuisance);
  
  set_size(numMeas, numConditions + numNuisance);
  
  // set neural condition regressors
  buildHrf();

  // store the new condition names as we go along
  vector<string> inputConditionNames = conditionNames;

  // build shifted delta for potential condition shifting
  vnl_vector<double> shiftdelta(numMeas,0.0);
  if(conditionShift > 0 && conditionShift < numMeas) {
    shiftdelta.put(conditionShift,1);
  }
  else {
    shiftdelta.put(0,1);
  }


  // consider each input condition individually
  for(unsigned int cond = 0; cond < numConditions; cond++) {
    vnl_vector<double> basis = inputConditions.get_column(cond);

    // incorporate condition shift via convolution with shifted delta
    if(conditionShift > 0 && conditionShift < numMeas) {
      vnl_vector<double> shiftcol = vnl_convolve(basis,shiftdelta);
      basis.update(shiftcol.extract(basis.size()));
    }


    // build block-wise condition vectors if required
    if(modelEachBlock) {
      unsigned int numBlocks = ceil((numMeas-conditionShift)/blockLen);

      // create a design matrix column per block
      for(unsigned int block = 0; block <= numBlocks; block++) {
	vnl_vector<double> blockCol(numMeas,0);

	// copy the entire block from the input condition vector
	for(unsigned int meas = conditionShift + block*blockLen;
	    meas < conditionShift + (block+1)*blockLen && meas < numMeas;
	    meas++) {
	  blockCol.put(meas, basis[meas]);
	}

	string blockStr;
	RtConfigVal::convertToString<unsigned int>(blockStr,block);

	addColumn(convolveVecWithHrf(blockCol),
		  inputConditionNames[cond] + "_block" + blockStr,
		  true);

	// add columns for temporal derivatives if required
	if(modelTemporalDerivatives) {
	  addColumn(convolveVecWithTemporalDerivative(blockCol),
		    inputConditionNames[cond] + "_block" + 
		    blockStr + "_deriv",
		    true);
	}
      }
    }
    else { // no block splitting necessary
      addColumn(convolveVecWithHrf(basis),
		inputConditionNames[cond],
		true);

      // add columns for temporal derivatives if required
      if(modelTemporalDerivatives) {
	addColumn(convolveVecWithTemporalDerivative(basis),
		   inputConditionNames[cond] + "_deriv",
		   true);
      }
    }
  }

  // build the nuisance columns

  // fill the trend colums
  for(unsigned int i = 0; i <= maxTrendOrder; i++) {
    vnl_vector<double> trend(numMeas,0);
    for(unsigned int j = 0; j < numMeas; j++) {
      switch(i) {
      case 0: // mean
	trend.put(j, 1.0);
	break;
      case 1: // linear
	trend.put(j, j+1);
	break;
      default: // higher
	trend.put(j, pow((double)j-numMeas/2,(int)i));
	break;
      }
    }

    string order;
    RtConfigVal::convertToString(order,i);

    addColumn(trend,string("trend_order_") + order, false);
  }

  // name the motion columns
  vnl_vector<double> motStub(numMeas,0);
  if(modelMotionParameters) {
    addColumn(motStub, "motion_tx", false);
    addColumn(motStub, "motion_ty", false);
    addColumn(motStub, "motion_tz", false);
    addColumn(motStub, "motion_rx", false);
    addColumn(motStub, "motion_ry", false);
    addColumn(motStub, "motion_rz", false);
  }


  // event and artifact columns will be left blank until they are received

  // check number of conditions against filled columns
  if(numAddedColumns != numNuisance + numConditions) {
    cerr << "WARNING: number of columns generated (" 
	 << numAddedColumns << ") is different from that expected ("
	 << numNuisance + numConditions 
	 << "). further calculations are suspect." 
	 << endl;
      
  }

  isBuilt = true;

  if(DEBUG_LEVEL & MODERATE) {
    print();
  }

  return true;
}

// sets the size (old data lost)
bool RtDesignMatrix::set_size(unsigned int row, unsigned int col) {
  vnl_matrix<double>::set_size(row,col);
  columnOfInterestIndicator.assign(col, false);
  columnNames.assign(col, "");

  return true;
}

// get the name of a design matrix column
string RtDesignMatrix::getColumnName(unsigned int col) {
  if(col >= columnNames.size()) {
    return "error: out of bounds";
  }

  return columnNames[col];
}

// get a design matrix column by index
vnl_vector<double> RtDesignMatrix::getColumn(unsigned int index) {
  return get_column(index);
}

// get a design matrix column by name
vnl_vector<double> RtDesignMatrix::getColumn(const string &name) {
  return get_column(getColumnIndex(name));
}

// get a design matrix column by name
unsigned int RtDesignMatrix::getColumnIndex(const string &name) {
  unsigned int index = 0;
  for(vector<string>::iterator i = columnNames.begin(); 
      i != columnNames.end(); i++, index++) {
    if((*i) == name) {
      return index;
    }
  }
  
  // not found
  return std::numeric_limits<unsigned int>::max();
}

// compute the number of neural signal bases in this model
unsigned int RtDesignMatrix::getNumConditionBases() {

  // take the number of conditions read from config as the base number
  unsigned int numConditions = numInputConditions;

  // expand this number based on optional parms

  if(modelEachBlock) {
    int numBlocks = (numMeas-conditionShift)/blockLen;
    numConditions *= numBlocks;
  }

  if(modelTemporalDerivatives) {
    numConditions *= 2;
  }

  return numConditions;
}


// compute the number of nuisance bases in this model
unsigned int RtDesignMatrix::getNumNuisanceBases() {

  unsigned int numNuisance = maxTrendOrder+1;

  // accumulate based on parms

  if(modelMotionParameters) {
    numNuisance += 6;
  }

  if(modelEvents) {
    numNuisance += maxNumEvents;
  }

  if(modelArtifacts) {
    numNuisance += maxNumArtifacts;
  }

  return numNuisance;
}


// retreive a vector of the indices of columns identified as representing
// neural signals of interest
vnl_vector<unsigned int> RtDesignMatrix::getColumnOfInterestIndices() {
  vnl_vector<unsigned int> inds;
  int ind = 0;
  for(vector<bool>::iterator i = columnOfInterestIndicator.begin();
      i != columnOfInterestIndicator.end(); ind++, i++) {
    if(*i) {
      inds.put(inds.size(),ind);
    }
  }

  return inds;
}

// determine whether a column is a basis function representing neural
// signals of interest or not
bool RtDesignMatrix::isColumnOfInterest(unsigned int col) {
  return col >= 0 && col < columnOfInterestIndicator.size()
    && columnOfInterestIndicator[col];
}



// get the stored hrf vector for this experiment (or build it)
vnl_vector<double> &RtDesignMatrix::getHrf() {
  // build it if not built
  if(!hrfIsBuilt) {
    buildHrf();
  }

  return hrf;
}

// get the stored high temporal resoltion hrf vector for this experiment (or
// build it)
vnl_vector<double> &RtDesignMatrix::getHiresHrf() {
  // build it if not built
  if(!hrfIsBuilt) {
    buildHrf();
  }

  return hiresHrf;
}


// get the stored temporalDerivative vector for this experiment
vnl_vector<double> &RtDesignMatrix::getTemporalDerivativeBasis() {
  // build it if not built
  if(!hrfIsBuilt) {
    buildHrf();
  }

  return temporalDerivative;
}

// get the stored high temporal resoltion temporalDerivative vector
vnl_vector<double> &RtDesignMatrix::getHiresTemporalDerivativeBasis() {
  // build it if not built
  if(!hrfIsBuilt) {
    buildHrf();
  }

  return hiresTemporalDerivative;
}

// loads an hrf vector from a file
bool RtDesignMatrix::loadHrfFile(vnl_vector<double> &hrf,
				 string filename) {
  vcl_ifstream in(filename.c_str(),ios::in);
  if(in.fail()) {
    return false;
  }

  if(!hrf.read_ascii(in)) {
    return false;
  }

  // compute other parms
  hrfSamplePeriod = tr;
  hiresHrf = hrf;

  // compute the temporal derivative basis
  temporalDerivative.set_size(hrf.size());
  temporalDerivative.fill(0);
  temporalDerivative.update(hrf.extract(hrf.size()-1,1),1);
  temporalDerivative = hrf - temporalDerivative;
  hiresTemporalDerivative = temporalDerivative;

  return true;
}

// build a gamma pdf with shape and scale parms
// see http://www.itl.nist.gov/div898/handbook/apr/section1/apr165.htm
double gammaPDF(double t, double a, double b) {
  return pow(b,a)/vnl_gamma(a) * pow(t,a-1) * exp(-b*t);
}

// builds an hrf vector 
// NOTE: first file loading is attempted, then if that fails a double gamma
// cannonical hrf is built
bool RtDesignMatrix::buildHrf() {

  // try to load from file
  if(loadHrf) {
    if(!loadHrfFile(hrf, hrfFilename)) { // try to load
      cerr << "WARNING: couldn't load hrf from "
	   << hrfFilename << ". using cannonical instead" << endl;
      loadHrf = false;
    }
    else {
      hrfIsBuilt = true;
      return hrfIsBuilt;
    }
  }


  // build a high temporal resolution version of the hrf
  double dt = tr*hrfSamplePeriod;
  hiresHrf.set_size(static_cast<unsigned int>(ceil(hrfLength/dt)));
  hiresHrf.fill(0);
  vnl_vector<double> shiftedHrf(hiresHrf.size(), 0);
  for(unsigned int i = 0; i < hiresHrf.size(); i++) {
    hiresHrf.put(i, gammaPDF(i,hrfTimeToPeakPos,dt)
		 - gammaPDF(i,hrfTimeToPeakNeg,dt)/hrfPosToNegRatio);
    shiftedHrf.put(i, gammaPDF(i-1/dt,hrfTimeToPeakPos,dt)
		 - gammaPDF(i-1/dt,hrfTimeToPeakNeg,dt)/hrfPosToNegRatio);
  }
  hiresHrf /= hiresHrf.sum();
  shiftedHrf /= shiftedHrf.sum();

  // build a temporal derivative basis function
  hiresTemporalDerivative = hiresHrf - shiftedHrf;


  // subsample hrf and td at the tr
  hrf.set_size((int)floor(hrfLength/tr+1));
  hrf.fill(0);
  temporalDerivative.set_size((int)floor(hrfLength/tr+1));
  temporalDerivative.fill(0);
  for(unsigned int i = 0; i < hrf.size(); i++) {
    hrf.put(i, hiresHrf[static_cast<unsigned int>(rint(i/hrfSamplePeriod))]);
    temporalDerivative.put(i, 
	       hiresTemporalDerivative[static_cast<unsigned int>(rint(i/hrfSamplePeriod))]);
  }

  if(DEBUG_LEVEL & MODERATE) {
    cerr << "hires hrf: ";
    for(unsigned int i = 0; i < hiresHrf.size(); i++) {
      cerr << hiresHrf[i] << " ";
    }
    cerr << endl;

    cerr << "hrf: ";
    for(unsigned int i = 0; i < hrf.size(); i++) {
      cerr << hrf[i] << " ";
    }
    cerr << endl;

    cerr << "hires temporal derivative basis: ";
    for(unsigned int i = 0; i < hiresTemporalDerivative.size(); i++) {
      cerr << hiresTemporalDerivative[i] << " ";
    }
    cerr << endl;

    cerr << "temporal derivative basis: ";
    for(unsigned int i = 0; i < temporalDerivative.size(); i++) {
      cerr << temporalDerivative[i] << " ";
    }
    cerr << endl;
  }

  hrfIsBuilt = true;
  return hrfIsBuilt;
}

// convolves a vector with the hrf for this design
vnl_vector<double> RtDesignMatrix::convolveVecWithHrf(const vnl_vector<double> &v) {
  vnl_vector<double> conHiresV(v.size()/hrfSamplePeriod);

  // upsample
  double curTr = 0;
  for(unsigned int i = 0; i < conHiresV.size(); 
      i++, curTr += hrfSamplePeriod) {
    conHiresV.put(i,v[(int) floor(curTr)]);
  }

  // convolve
  vnl_vector<double> tmp = vnl_convolve(conHiresV,getHiresHrf());
  conHiresV.update(tmp.extract(conHiresV.size()));

  // downsample
  vnl_vector<double> conV(v.size(),0);
  for(unsigned int i = 0; i < conV.size(); i++) {
    conV.put(i,conHiresV[i/hrfSamplePeriod]);
  }

  return conV;
}

// convolves a vector with the temporal derivative response vector
vnl_vector<double> RtDesignMatrix::convolveVecWithTemporalDerivative(const vnl_vector<double> &v) {
  vnl_vector<double> conHiresV(v.size()/hrfSamplePeriod);

  // upsample
  double curTr = 0;
  for(unsigned int i = 0; i < conHiresV.size();
      i++, curTr += hrfSamplePeriod) {
    conHiresV.put(i,v[(int) floor(curTr)]);
  }

  // convolve
  vnl_vector<double> tmp = vnl_convolve(conHiresV,getHiresTemporalDerivativeBasis());
  conHiresV.update(tmp.extract(conHiresV.size()));

  // downsample
  vnl_vector<double> conV(v.size(),0);
  for(unsigned int i = 0; i < conV.size(); i++) {
    conV.put(i,conHiresV[i/hrfSamplePeriod]);
  }

  return conV;
}

// add event to be modeled
// in
//   onset (trs) of the event
//   duration (trs) of the event
bool RtDesignMatrix::addEvent(double onset, double duration) {
  // check bounds
  if(onset >= numMeas) {
    cerr << "WARNING: RtDesignMatrix::addEvent trying to add out of bounds tr (" 
	 << onset << ")" << endl;
    return false;
  }

  if(numEvents >= maxNumEvents) {
    cerr << "WARNING: RtDesignMatrix::addEvent trying to add too many events (" 
	 << numEvents << ")" << endl;
    return false;
  }

  // build a vector to convolve with the hrf
  unsigned int hiresEventLen =  numMeas*tr/hrfSamplePeriod;
  vnl_vector<double> hiresEvent(hiresEventLen, 0);
  unsigned int hiresOnset = (unsigned int) rint(onset*tr/hrfSamplePeriod);
  unsigned int hiresDuration = (unsigned int) rint(duration*tr/hrfSamplePeriod);
  
  // fill with ones at on times
  hiresEvent.put(0,1);
  for(unsigned int i = hiresOnset; i < hiresDuration; i++) {
    hiresEvent.put(i,1);
  }

  // scale if duration is less than a tr
  if(duration < 1.0) {
    hiresEvent /= (hiresEvent.sum() * hrfSamplePeriod);
  }

  // convolve
  vnl_vector<double> tmp = vnl_convolve(hiresEvent, getHiresHrf());
  hiresEvent.update(tmp.extract(hiresEvent.size()));

  // make a design matrix column
  vnl_vector<double> column(numMeas,0);
  for(unsigned int i = 0; i < numMeas; i++) {
    column.put(i, hiresEvent[i*(tr/hrfSamplePeriod)]);
  }

  addColumn(column, "event" + numEvents, false);
  numEvents++;

  return true;
}

// add artifact to be modeled
// in
//   tr of the artifact
bool RtDesignMatrix::addArtifact(unsigned int thisTr) {
  // check bounds
  if(thisTr >= numMeas) {
    cerr << "WARNING: RtDesignMatrix::addArtifact trying to add out of bounds tr (" 
	 << thisTr << ")" << endl;
    return false;
  }

  if(numArtifacts >= maxNumArtifacts) {
    cerr << "WARNING: RtDesignMatrix::addArtifact trying to add too many artifacts (" 
	 << numArtifacts << ")" << endl;
    return false;
  }

  // add the artifact and update the names
  vnl_vector<double> artCol(numMeas,0);
  artCol.put(thisTr,1);
  addColumn(artCol,"artifact" + numArtifacts, false);
  numArtifacts++;

  return true;
}

// update the design matrix for a new timepoint
// NOTE: this should be called after all events and  artifacts have been
// added (through addEvent and addArtifact) and motion has been added to the
// datastore for this timepoint. usually this function will be called from
// RtModelFit::process()
//  in 
//   tr of the timepoint to update for
bool RtDesignMatrix::updateAtTr(unsigned int thisTr) {

  if(!isBuilt) {
    cerr << "WARNING: can't update() a design matrix that hasn't been build()t."
	 << endl;
    return false;
  }

  // check bounds
  if(thisTr >= numMeas) {
    cerr 
      << "WARNING: RtDesignMatrix::update trying to process tr out of range ("
      << thisTr << ")" << endl;
    return false;
  }

  // copy the motion parameters
  if(modelMotionParameters) {
    RtDataID motID(templateDataID);
    motID.setModuleID("motion");

    RtMotion *mot = static_cast<RtMotion*>(getDataStore().getData(motID));

    if(mot != NULL) {
      unsigned int motionCol = getNumConditionBases() + maxTrendOrder;
      vnl_matrix<double> motionMat(mot->getMotion(),1,NUM_MOTION_DIMENSIONS);
      update(motionMat, thisTr, motionCol);
    }
  }

  return true;
}
 
// build a row of the GLM deign matrix
// in
//  current image
vnl_vector<double> RtDesignMatrix::getRow(unsigned int timepoint) {

  // bounds check
  if(timepoint >= numMeas) {
    return vnl_vector<double>(0);
  }

  return get_row(timepoint);
}

// get the covariance of a contrast given a contrast vector
// if this matrix is X, this function computes c' pinv(X'X) c
double RtDesignMatrix::computeContrastCovariance(vnl_vector<double> &contrastVector) {
  if(contrastVector.size() != columns()) {
    cerr << "ERROR: number of elements in contrast vector does not match the number of columns in the design matrix" << endl;
    return std::numeric_limits<double>::quiet_NaN();
  }

  // compute the contrast covariance based on the currently known regressors
  // NOTE: this will not be the same as computing it at the end of the
  // experiment when all regressors are known. it would be nice to compute
  // final values using the known design.
  vnl_matrix<double> convec(contrastVector.data_block(), 
			    contrastVector.size(), 1);
  vnl_svd<double> pinv(transpose() * (*this));
  vnl_matrix<double> result = convec.transpose() * pinv.pinverse() * convec;
  return result.get(0,0);
}

// add a column to the design
//  in
//   vals:     desired values
//   interest: whether this column is a basis function representing neural
//             activation of interest
//  out
//   true if assigned
//   false if index out of bounds
unsigned int RtDesignMatrix::addColumn(const vnl_vector<double> &vals,
				       string name,
				       bool interest) {
  unsigned int col = numAddedColumns;

  if(col >= columns()) { // check bounds
    return UINT_MAX;
  }

  // assign
  set_column(col,vals);
  columnNames[col] = name;
  columnOfInterestIndicator[col] = interest;

  numAddedColumns++;

  return numAddedColumns-1;
}

// print
void RtDesignMatrix::print() {
  for(unsigned int r = 0; r < rows(); r++) {
    for(unsigned int c = 0; c < cols(); c++) {
      cout << data[r][c] << " ";
    }
    cout << endl;
  }
}

// serialize the data as xml for transmission or saving to a file
TiXmlElement *RtDesignMatrix::serializeAsXML() {
  cout << "RtDesignMatrix::serializeAsXML() not implemented" << endl;
  return NULL;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


