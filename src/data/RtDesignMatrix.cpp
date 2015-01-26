/*=========================================================================
 *  RtDesignMatrix.cpp defines a class that represents a model's basis
 *  functions
 *
 *  NOTE: much of the funcitonality is inhereted from vnl_matrix, so see this
 *  help page for usage of this class:
 *  http://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__matrix.html
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include"RtDesignMatrix.h"

#include"RtExperiment.h"
#include"RtMotion.h"
#include"RtDataIDs.h"
#include"RtLimit.h"

#include<vcl_istream.h>

#include<algorithm>
#include<fstream>
#include<limits>
#include<iostream>
#include<iomanip>
#include<string>

#include<vnl/algo/vnl_convolve.h>
#include<vnl/vnl_gamma.h>
#include<vnl/algo/vnl_svd.h>

#include"debug_levels.h"
RtDesignMatrix::RtDesignMatrix() : RtData(), vnl_matrix<double>() {

  vnl_matrix<double>::set_size(0, 0);

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

  hrfSamplePeriod = 1 / 16.0;
  hrfLength = 32;
  hrfTimeToPeakPos = 6;
  hrfTimeToPeakNeg = 16;
  hrfPosToNegRatio = 6;

  loadHrf = false;
  hrfFilename = "unset";

  loadDesignMatrixFromFile = false;
  designMatrixFilename = "unset";
  padConditionWithZeros = false;

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
  for (TiXmlNode *option = 0;
       (option = designEle->IterateChildren("option", option));) {
    if (option->Type() != TiXmlNode::ELEMENT)
      continue;

    optionElmt = (TiXmlElement*) option;
    if (TIXML_SUCCESS != optionElmt->QueryValueAttribute("name", &name)) {
      continue;
    }

    // build the map between atrribute names and values
    map<string, string> attr = RtConfig::getAttributeMap(*optionElmt);

    // figure out which option we have and process it
    if (!processOption(name, optionElmt->GetText(), attr)) {
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
                                   const map<string, string> &attrMap) {

  if (name == "disabled") {
    return true;
  }
  if (name == "condition") { // load the condition vector
    numInputConditions++;

    if (numInputConditions == 1) { // allocate condition matrix
      inputConditions.clear();
      inputConditions.set_size(numMeas, MAX_CONDITIONS);
      inputConditions.fill(0);
      conditionNames.reserve(MAX_CONDITIONS);
      conditionInterestIndicator.reserve(MAX_CONDITIONS);
    }
    else if (numInputConditions > MAX_CONDITIONS) {
      cerr << "warning: max number of conditions exceeded." << endl;
      return false;
    }

    // find the name, or just name it a number
    map<string, string>::const_iterator condName
        = attrMap.find("conditionName");
    if (condName == attrMap.end()) {
      string nameNum = "condition";
      nameNum += numInputConditions;
      conditionNames.push_back(nameNum);
    }
    else {
      conditionNames.push_back((*condName).second);
    }

    // find if this condition is of interest for feedback
    map<string, string>::const_iterator condType
        = attrMap.find("conditionType");

    // if condition type missing, assume it's of interest
    if(condType == attrMap.end()) {
      conditionInterestIndicator.push_back(true);
    }
    else {
      string typeVal = (*condType).second;
      transform(typeVal.begin(), typeVal.end(),
                typeVal.begin(), ::tolower);
      conditionInterestIndicator.push_back(
          (*condType).second != "nuisance");
    }

    // parse the text string into a condition vector
    double el;
    size_t i = 0;
    for (size_t i1 = 0, i2 = text.find(" "); 1;
         i++, i1 = i2 + 1, i2 = text.find(" ", i1)) {

      if (!RtConfigVal::convert<double>(
              el, text.substr(
                  i1, i2 == string::npos ? text.size() - i1 : i2 - i1))) {
        continue;
      }
      inputConditions.put(i, numInputConditions - 1, el);

      if (i2 == string::npos) { // test if we are on the last one
        // set the blockLen if its not been set
        if (blockLen == 0) {
          blockLen = i + 1;
          cout << "auto setting blockLen to " << blockLen << endl;
        }

        break;
      }
    }

    // fill the rest of the measurements as periodic stim
    for (; i < numMeas; i++) {
      inputConditions.put(i, numInputConditions - 1,
                          inputConditions.get(i % blockLen,
                                              numInputConditions - 1));
    }

    return true;
  }
  if (name == "conditionShift") {
    return RtConfigVal::convert<unsigned int>(conditionShift, text);
  }
  if (name == "loadDesignMatrixFromFile") {
    RtConfigVal::convert<bool>(loadDesignMatrixFromFile, text);
    return true;
  }
  if (name == "designMatrixFilename") {
    designMatrixFilename = text;
    return true;
  }
  if (name == "padConditionWithZeros") {
    return RtConfigVal::convert<bool>(padConditionWithZeros, text);
  }
  if (name == "modelEachBlock") {
    return RtConfigVal::convert<bool>(modelEachBlock, text);
  }
  if (name == "hrfSamplePeriod") {
    return RtConfigVal::convert<double>(hrfSamplePeriod, text);
  }
  if (name == "hrfLength") {
    return RtConfigVal::convert<double>(hrfLength, text);
  }
  if (name == "hrfTimeToPeakPos") {
    return RtConfigVal::convert<double>(hrfTimeToPeakPos, text);
  }
  if (name == "hrfTimeToPeakNeg") {
    return RtConfigVal::convert<double>(hrfTimeToPeakNeg, text);
  }
  if (name == "hrfPosToNegRatio") {
    return RtConfigVal::convert<double>(hrfPosToNegRatio, text);
  }
  if (name == "hrfFilename") {
    hrfFilename = text;
    loadHrf = true;
    return true;
  }
  if (name == "modelMotionParameters") {
    return RtConfigVal::convert<bool>(modelMotionParameters, text);
  }
  if (name == "modelTemporalDerivatives") {
    return RtConfigVal::convert<bool>(modelTemporalDerivatives, text);
  }
  if (name == "modelEvents") {
    return RtConfigVal::convert<bool>(modelEvents, text);
  }
  if (name == "eventDuration") {
    return RtConfigVal::convert<unsigned int>(eventDuration, text);
  }
  if (name == "maxNumEvents") {
    return RtConfigVal::convert<unsigned int>(maxNumEvents, text);
  }
  if (name == "modelArtifacts") {
    return RtConfigVal::convert<bool>(modelArtifacts, text);
  }
  if (name == "maxNumArtifacts") {
    return RtConfigVal::convert<unsigned int>(maxNumArtifacts, text);
  }
  if (name == "maxTrendOrder") {
    return RtConfigVal::convert<unsigned int>(maxTrendOrder, text);
  }

  cout << "WARNING: option " << name << " unrecognized and ignored" << endl;
  return true;
}

void RtDesignMatrix::addCondition(const string &name, bool of_interest) {
  inputConditions.set_size(numMeas, inputConditions.columns() + 1);
  for (size_t i = 0; i < numMeas; i++) {
    inputConditions(i, inputConditions.columns() - 1) = 0.0;
  }
  numInputConditions++;

  conditionNames.push_back(name);
  conditionInterestIndicator.push_back(of_interest);
}

void RtDesignMatrix::setConditionValueAtTR(size_t row, size_t col, double val) {
  inputConditions(row, col) = val;
}

double RtDesignMatrix::getConditionValueAtTR(size_t row, size_t col) {
  return inputConditions(row, col);
}

bool RtDesignMatrix::buildDesignMatrix() {

  // TODO this will need to get called by unserializeXML as well

  // fill the rest of the measurements as periodic stim
  if (loadDesignMatrixFromFile) {
    if (padConditionWithZeros) {
      // TODO
    }
    else {
      unsigned int i = blockLen - 1;
      for (; i < numMeas; i++) {
        inputConditions.put(
            i, numInputConditions - 1, inputConditions.get(
                i % blockLen, numInputConditions - 1));
      }
    }
  }

  // the rest was originally in build()

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
  vnl_vector<double> shiftdelta(numMeas, 0.0);
  if (conditionShift > 0 && conditionShift < numMeas) {
    shiftdelta.put(conditionShift, 1);
  }
  else {
    shiftdelta.put(0, 1);
  }

  // consider each input condition individually
  for (unsigned int cond = 0; cond < numConditions; cond++) {
    vnl_vector<double> basis = inputConditions.get_column(cond);

    // incorporate condition shift via convolution with shifted delta
    if (conditionShift > 0 && conditionShift < numMeas) {
      vnl_vector<double> shiftcol = vnl_convolve(basis, shiftdelta);
      basis.update(shiftcol.extract(basis.size()));
    }


    // build block-wise condition vectors if required
    if (modelEachBlock) {
      unsigned int numBlocks =
          ceil((double) ((numMeas - conditionShift) / blockLen));

      // create a design matrix column per block
      for (unsigned int block = 0; block <= numBlocks; block++) {
        vnl_vector<double> blockCol(numMeas, 0);

        // copy the entire block from the input condition vector
        for (unsigned int meas = conditionShift + block * blockLen;
             meas < conditionShift + (block + 1) * blockLen && meas < numMeas;
             meas++) {
          blockCol.put(meas, basis[meas]);
        }

        string blockStr;
        RtConfigVal::convertToString<unsigned int>(blockStr, block);

        addColumn(convolveVecWithHrf(blockCol),
                  inputConditionNames[cond] + "_block" + blockStr,
                  conditionInterestIndicator[cond]);

        // add columns for temporal derivatives if required
        if (modelTemporalDerivatives) {
          addColumn(convolveVecWithTemporalDerivative(blockCol),
                    inputConditionNames[cond] + "_block" +
                    blockStr + "_deriv",
                    conditionInterestIndicator[cond]);
        }
      }
    }
    else { // no block splitting necessary
      addColumn(convolveVecWithHrf(basis),
                inputConditionNames[cond],
                conditionInterestIndicator[cond]);

      // add columns for temporal derivatives if required
      if (modelTemporalDerivatives) {
        addColumn(convolveVecWithTemporalDerivative(basis),
                  inputConditionNames[cond] + "_deriv",
                  conditionInterestIndicator[cond]);
      }
    }
  }

  // build the nuisance columns

  // fill the trend colums
  for (unsigned int i = 0; i <= maxTrendOrder; i++) {
    vnl_vector<double> trend(numMeas, 0);
    for (unsigned int j = 0; j < numMeas; j++) {
      switch (i) {
        case 0: // mean
          trend.put(j, 1.0);
          break;
        case 1: // linear
          trend.put(j, j + 1);
          break;
        default: // higher
          trend.put(j, pow((double) j - numMeas / 2, (int) i));
          break;
      }
    }

    string order;
    RtConfigVal::convertToString(order, i);

    addColumn(trend, string("trend_order_") + order, false);
  }

  // name the motion columns
  vnl_vector<double> motStub(numMeas, 0);
  if (modelMotionParameters) {
    addColumn(motStub, "motion_tx", false);
    addColumn(motStub, "motion_ty", false);
    addColumn(motStub, "motion_tz", false);
    addColumn(motStub, "motion_rx", false);
    addColumn(motStub, "motion_ry", false);
    addColumn(motStub, "motion_rz", false);
  }


  // event and artifact columns will be left blank until they are received

  // check number of conditions against filled columns
  if (numAddedColumns != numNuisance + numConditions) {
    cerr << "WARNING: number of columns generated ("
         << numAddedColumns << ") is different from that expected ("
         << numNuisance + numConditions
         << "). further calculations are suspect."
         << endl;

  }

  isBuilt = true;

  if (DEBUG_LEVEL & MODERATE) {
    print();
  }

  return true;
}

// build the design
bool RtDesignMatrix::build() {

  if (numMeas == 0) {
    cerr << "ERROR: refusing to build() design matrix with zero rows. "
         << "did you forget to call setNumMeas()?" << endl;
    return false;
  }

  // try to load design matrix from file
  if (loadDesignMatrixFromFile) {
    if (!loadDesignMatrixFile(designMatrixFilename)) {
      cerr << "WARNING: couldn't load design matrix from "
           << designMatrixFilename << ". Will wait for infoserver instead."
           << endl;
      loadDesignMatrixFromFile = false;
    }
    else {
      // will get here if loading from file was successful
      buildDesignMatrix();

      // TODO will maybe need call to data store if it's not put into
      // buildDesignMatrix
    }
  }
  // check that condition is filled and then call buildDesignMatrix (for
  // temp. backwards compatibility until all condition stuff is removed from
  // conf file)
  else if (numInputConditions > 0) {

    buildDesignMatrix();
  }
  else {
    cerr << "WARNING: setFromStimulus is true. "
         << "Design matrix must come from infoserver." << endl;
  }

  // the stuff that was here is now in buildDesignMatrix()

  return true;
}

// sets the size (old data lost)
bool RtDesignMatrix::set_size(unsigned int row, unsigned int col) {
  vnl_matrix<double>::set_size(row, col);
  columnOfInterestIndicator.assign(col, false);
  columnNames.assign(col, "");

  return true;
}

// get the name of a design matrix column
string RtDesignMatrix::getColumnName(unsigned int col) {
  if (col >= columnNames.size()) {
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
  for (vector<string>::iterator i = columnNames.begin();
       i != columnNames.end(); i++, index++) {
    if ((*i) == name) {
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

  if (modelEachBlock) {
    int numBlocks = (numMeas - conditionShift) / blockLen;
    numConditions *= numBlocks;
  }

  if (modelTemporalDerivatives) {
    numConditions *= 2;
  }

  return numConditions;
}


// compute the number of nuisance bases in this model
unsigned int RtDesignMatrix::getNumNuisanceBases() {

  unsigned int numNuisance = maxTrendOrder + 1;

  // accumulate based on parms

  if (modelMotionParameters) {
    numNuisance += 6;
  }

  if (modelEvents) {
    numNuisance += maxNumEvents;
  }

  if (modelArtifacts) {
    numNuisance += maxNumArtifacts;
  }

  return numNuisance;
}


// retreive a vector of the indices of columns identified as representing
// neural signals of interest
vnl_vector<unsigned int> RtDesignMatrix::getColumnOfInterestIndices() {
  vnl_vector<unsigned int> inds(columnOfInterestIndicator.size() -
                                getNumNuisanceBases());

  int filled = 0;
  int ind = 0;
  for (vector<bool>::iterator i = columnOfInterestIndicator.begin();
       i != columnOfInterestIndicator.end(); ind++, i++) {
    if (*i) {
      inds.put(filled, ind);
      filled++;
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
  if (!hrfIsBuilt) {
    buildHrf();
  }

  return hrf;
}

// get the stored high temporal resoltion hrf vector for this experiment (or
// build it)
vnl_vector<double> &RtDesignMatrix::getHiresHrf() {
  // build it if not built
  if (!hrfIsBuilt) {
    buildHrf();
  }

  return hiresHrf;
}

// get the stored temporalDerivative vector for this experiment
vnl_vector<double> &RtDesignMatrix::getTemporalDerivativeBasis() {
  // build it if not built
  if (!hrfIsBuilt) {
    buildHrf();
  }

  return temporalDerivative;
}

// get the stored high temporal resoltion temporalDerivative vector
vnl_vector<double> &RtDesignMatrix::getHiresTemporalDerivativeBasis() {
  // build it if not built
  if (!hrfIsBuilt) {
    buildHrf();
  }

  return hiresTemporalDerivative;
}

// loads an hrf vector from a file
bool RtDesignMatrix::loadHrfFile(vnl_vector<double> &hrf, string filename) {
  vcl_ifstream in(filename.c_str(), ios::in);
  if (in.fail()) {
    return false;
  }

  if (!hrf.read_ascii(in)) {
    return false;
  }

  // compute other parms
  hrfSamplePeriod = tr;
  hiresHrf = hrf;

  // compute the temporal derivative basis
  temporalDerivative.set_size(hrf.size());
  temporalDerivative.fill(0);
  temporalDerivative.update(hrf.extract(hrf.size() - 1, 1), 1);
  temporalDerivative = hrf - temporalDerivative;
  hiresTemporalDerivative = temporalDerivative;

  return true;
}

// build a gamma pdf with shape and scale parms
// see http://www.itl.nist.gov/div898/handbook/apr/section1/apr165.htm
double gammaPDF(double t, double a, double b) {
  return pow(b, a) / vnl_gamma(a) * pow(t, a - 1) * exp(-b * t);
}

// builds an hrf vector
// NOTE: first file loading is attempted, then if that fails a double gamma
// cannonical hrf is built
bool RtDesignMatrix::buildHrf() {

  // try to load from file
  if (loadHrf) {
    if (!loadHrfFile(hrf, hrfFilename)) { // try to load
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
  hiresHrf.set_size(static_cast<unsigned int> (ceil(hrfLength / dt)));
  hiresHrf.fill(0);
  vnl_vector<double> shiftedHrf(hiresHrf.size(), 0);
  for (unsigned int i = 0; i < hiresHrf.size(); i++) {
    hiresHrf.put(i, gammaPDF(i, hrfTimeToPeakPos, dt)
                 - gammaPDF(i, hrfTimeToPeakNeg, dt) / hrfPosToNegRatio);
    shiftedHrf.put(
        i, gammaPDF(i - 1 / dt, hrfTimeToPeakPos, dt)
        - gammaPDF(i - 1 / dt, hrfTimeToPeakNeg, dt) / hrfPosToNegRatio);
  }
  hiresHrf /= hiresHrf.sum();
  shiftedHrf /= shiftedHrf.sum();

  // build a temporal derivative basis function
  hiresTemporalDerivative = hiresHrf - shiftedHrf;


  // subsample hrf and td at the tr
  hrf.set_size((int) floor(hrfLength / tr + 1));
  hrf.fill(0);
  temporalDerivative.set_size((int) floor(hrfLength / tr + 1));
  temporalDerivative.fill(0);
  for (unsigned int i = 0; i < hrf.size(); i++) {
    hrf.put(i, hiresHrf[static_cast<unsigned int> (rint(i / hrfSamplePeriod))]);
    temporalDerivative.put(
        i, hiresTemporalDerivative[static_cast<unsigned int>(
              rint(i / hrfSamplePeriod))]);
  }

  if (DEBUG_LEVEL & MODERATE) {
    cerr << "hires hrf: ";
    for (unsigned int i = 0; i < hiresHrf.size(); i++) {
      cerr << hiresHrf[i] << " ";
    }
    cerr << endl;

    cerr << "hrf: ";
    for (unsigned int i = 0; i < hrf.size(); i++) {
      cerr << hrf[i] << " ";
    }
    cerr << endl;

    cerr << "hires temporal derivative basis: ";
    for (unsigned int i = 0; i < hiresTemporalDerivative.size(); i++) {
      cerr << hiresTemporalDerivative[i] << " ";
    }
    cerr << endl;

    cerr << "temporal derivative basis: ";
    for (unsigned int i = 0; i < temporalDerivative.size(); i++) {
      cerr << temporalDerivative[i] << " ";
    }
    cerr << endl;
  }

  hrfIsBuilt = true;
  return hrfIsBuilt;
}

// convolves a vector with the hrf for this design
vnl_vector<double> RtDesignMatrix::convolveVecWithHrf(
    const vnl_vector<double> &v) {
  vnl_vector<double> conHiresV(v.size() / hrfSamplePeriod);

  // upsample
  double curTr = 0;
  for (unsigned int i = 0; i < conHiresV.size();
       i++, curTr += hrfSamplePeriod) {
    conHiresV.put(i, v[(int) floor(curTr)]);
  }

  // convolve
  vnl_vector<double> tmp = vnl_convolve(conHiresV, getHiresHrf());
  conHiresV.update(tmp.extract(conHiresV.size()));

  // downsample
  vnl_vector<double> conV(v.size(), 0);
  for (unsigned int i = 0; i < conV.size(); i++) {
    conV.put(i, conHiresV[i / hrfSamplePeriod]);
  }

  return conV;
}

// convolves a vector with the temporal derivative response vector
vnl_vector<double> RtDesignMatrix::convolveVecWithTemporalDerivative(
    const vnl_vector<double> &v) {
  vnl_vector<double> conHiresV(v.size() / hrfSamplePeriod);

  // upsample
  double curTr = 0;
  for (unsigned int i = 0; i < conHiresV.size();
       i++, curTr += hrfSamplePeriod) {
    conHiresV.put(i, v[(int) floor(curTr)]);
  }

  // convolve
  vnl_vector<double> tmp = vnl_convolve(conHiresV,
                                        getHiresTemporalDerivativeBasis());
  conHiresV.update(tmp.extract(conHiresV.size()));

  // downsample
  vnl_vector<double> conV(v.size(), 0);
  for (unsigned int i = 0; i < conV.size(); i++) {
    conV.put(i, conHiresV[i / hrfSamplePeriod]);
  }

  return conV;
}

// add event to be modeled
// in
//   onset (trs) of the event
//   duration (trs) of the event
bool RtDesignMatrix::addEvent(double onset, double duration) {
  // check bounds
  if (onset >= numMeas) {
    cerr << "WARNING: RtDesignMatrix::addEvent trying to add out of bounds tr ("
         << onset << ")" << endl;
    return false;
  }

  if (numEvents >= maxNumEvents) {
    cerr << "WARNING: RtDesignMatrix::addEvent trying to add too many events ("
         << numEvents << ")" << endl;
    return false;
  }

  // build a vector to convolve with the hrf
  unsigned int hiresEventLen = numMeas * tr / hrfSamplePeriod;
  vnl_vector<double> hiresEvent(hiresEventLen, 0);
  unsigned int hiresOnset = (unsigned int) rint(onset * tr / hrfSamplePeriod);
  unsigned int hiresDuration =
      (unsigned int) rint(duration * tr / hrfSamplePeriod);

  // fill with ones at on times
  hiresEvent.put(0, 1);
  for (unsigned int i = hiresOnset; i < hiresDuration; i++) {
    hiresEvent.put(i, 1);
  }

  // scale if duration is less than a tr
  if (duration < 1.0) {
    hiresEvent /= (hiresEvent.sum() * hrfSamplePeriod);
  }

  // convolve
  vnl_vector<double> tmp = vnl_convolve(hiresEvent, getHiresHrf());
  hiresEvent.update(tmp.extract(hiresEvent.size()));

  // make a design matrix column
  vnl_vector<double> column(numMeas, 0);
  for (unsigned int i = 0; i < numMeas; i++) {
    column.put(i, hiresEvent[i * (tr / hrfSamplePeriod)]);
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
  if (thisTr >= numMeas) {
    cerr << "WARNING: RtDesignMatrix::addArtifact trying to add "
         << "out of bounds tr (" << thisTr << ")" << endl;
    return false;
  }

  if (numArtifacts >= maxNumArtifacts) {
    cerr << "WARNING: RtDesignMatrix::addArtifact trying to add "
         << "too many artifacts (" << numArtifacts << ")" << endl;
    return false;
  }

  // add the artifact and update the names
  vnl_vector<double> artCol(numMeas, 0);
  artCol.put(thisTr, 1);
  addColumn(artCol, "artifact" + numArtifacts, false);
  numArtifacts++;

  return true;
}
vnl_vector<double> RtDesignMatrix::getArtifactTimepoints() {

  // initialize return vector
  vnl_vector<double> listOfTr(numArtifacts, 0);
  unsigned int foundArt = 0;

  // loop through all columns
  for (unsigned int col = 0; col < getNumColumns(); col++) {

    // look for column name "artifact"
    if (!strcmp(getColumnName(col).c_str(), "artifact")) {

      vnl_vector<double> curCol = getColumn(col);

      // go through rows to find artifact timepoint
      for (unsigned int row = 0; row < getNumRows(); row++) {
        if (curCol[row] == 1) {
          listOfTr.put(foundArt, row);
          foundArt++;
          break; // go to next col
        }
      }
    }
  }
  return listOfTr;
}

// update the design matrix for a new timepoint
// NOTE: this should be called after all events and  artifacts have been
// added (through addEvent and addArtifact) and motion has been added to the
// datastore for this timepoint. usually this function will be called from
// RtModelFit::process()
//  in
//   tr of the timepoint to update for
bool RtDesignMatrix::updateAtTr(unsigned int thisTr) {

  if (!isBuilt) {
    cerr << "WARNING: can't update() a design matrix that hasn't been build()t."
         << endl;
    return false;
  }

  // check bounds
  if (thisTr >= numMeas) {
    cerr
        << "WARNING: RtDesignMatrix::update trying to process tr out of range ("
        << thisTr << ")" << endl;
    return false;
  }

  // copy the motion parameters
  if (modelMotionParameters) {
    RtDataID motID(templateDataID);
    motID.setModuleID("motion");

    RtMotion *mot = static_cast<RtMotion*> (getDataStore().getData(motID));

    if (mot != NULL) {
      unsigned int motionCol = getNumConditionBases() + maxTrendOrder;
      vnl_matrix<double> motionMat(mot->getMotion(), 1, NUM_MOTION_DIMENSIONS);
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
  if (timepoint >= numMeas) {
    return vnl_vector<double>(0);
  }

  return get_row(timepoint);
}

// get the covariance of a contrast given a contrast vector
// if this matrix is X, this function computes c' pinv(X'X) c
double RtDesignMatrix::computeContrastCovariance(
    vnl_vector<double> &contrastVector) {
  if (contrastVector.size() != columns()) {
    cerr << "ERROR: number of elements in contrast vector does not match the "
         << "number of columns in the design matrix" << endl;
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
  return result.get(0, 0);
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
                                       string name, bool interest) {
  unsigned int col = numAddedColumns;

  if (col >= columns()) { // check bounds
    return UINT_MAX;
  }

  // assign
  set_column(col, vals);
  columnNames[col] = name;
  columnOfInterestIndicator[col] = interest;

  numAddedColumns++;

  return numAddedColumns - 1;
}

// read design matrix information from a text file
//  in
//   filename: text file with design matrix info, specified in configuration
//   file
bool RtDesignMatrix::loadDesignMatrixFile(string filename) {
  // declare file stream
  ifstream in(filename.c_str(), ifstream::in);

  // check file
  if (!in.good()) {
    cout << "Problem opening design matrix file" << endl;
    return false;
  }

  unsigned int numTP = UINT_MAX;
  string cthname = "";
  unsigned int currentRow = 0;

  while (in) {
    // declare variables
    string s;
    stringstream ss;

    // get line from file and put it into stringstream
    getline(in, s);
    ss << s;

    // ignore comment lines
    if (!strncmp(ss.str().c_str(), "#", 1)) {
      continue;
    }

    // read in number of time points and number of conditions
    if (numTP == UINT_MAX) {
      ss >> numTP;
      ss >> numInputConditions;

      // check numTP just for consistency
      if (numTP != numMeas) {
        cerr << "warning: number of timepoints specified in design matrix "
             << "file does not match numMeas set in conf file" << endl;
        return false;
      }

      // check declared number of conditions
      if (numInputConditions > MAX_CONDITIONS) {
        cerr << "warning: max number of conditions exceeded." << endl;
        return false;
      }
      else if (numInputConditions > 0) {
        // allocate condition matrix
        conditionNames.reserve(MAX_CONDITIONS);
        conditionInterestIndicator.reserve(MAX_CONDITIONS);
        inputConditions.clear();
        inputConditions.set_size(numMeas, MAX_CONDITIONS);
        inputConditions.fill(0);
        conditionShift = 0; // TODO should this be set somehow?
      }
      else {
        // numInputConditions is zero or less (i.e. a null case)
      }
      continue;
    }

    // read in condition names. Note this assumes each condition has a name
    if (cthname.empty()) {
      for (unsigned int c = 0; c < numInputConditions; c++) {
        ss >> cthname;
        conditionNames.push_back(cthname);

        // TODO(ohinds): modify file format to allow conditions to
        // be nuisance
        conditionInterestIndicator.push_back(true);

        // if we've read all of the condition names, continue
        if (c == numInputConditions - 1) {
          continue;
        }
      }
      continue;
    }
    // read in condition data
    double cond;
    for (unsigned int c = 0; c < numInputConditions; c++) {
      ss >> cond;
      inputConditions.put(currentRow, c, cond);
    }
    currentRow++;

    // if we are on the last row, set blockLen
    if (in.eof()) {
      // set the blockLen if its not been set
      if (blockLen == 0) {
        blockLen = currentRow;
        cout << "auto setting blockLen to " << blockLen << endl;
      }
      break;
    }
  }
  return true;
}

// print
void RtDesignMatrix::print() {
  for (unsigned int r = 0; r < rows(); r++) {
    for (unsigned int c = 0; c < cols(); c++) {
      cout << data[r][c] << " ";
    }
    cout << endl;
  }
}

// save the matrix to a file
bool RtDesignMatrix::save(const string& filename) const {
  ofstream out(filename.c_str());
  if(!out.good()) {
    return false;
  }

  // write header
  out << "# ";
  for(unsigned int c = 0; c < columnNames.size(); c++)
  {
    out << columnNames[c] << "(" << columnOfInterestIndicator[c] << ") ";
  }
  out << endl;

  for(unsigned int r = 0; r < rows(); r++) {
    for(unsigned int c = 0; c < cols(); c++) {
      out << data[r][c] << " ";
    }
    out << endl;
  }

  out.close();

  return true;
}

// serialize the data as xml for transmission or saving to a file
// TODO this monolithic function should be split up
TiXmlElement *RtDesignMatrix::serializeAsXML(TiXmlElement *requestElement) {
#define NUM_SIGFIGS 6

  // create a designmatrix element for output
  TiXmlElement *designmatrix = new TiXmlElement("data");
  designmatrix->SetAttribute("name", "designmatrix");

  // if there are no elements in designmatrix, assume user wants all parts, so
  // add all parts and then continue to loop.
  if (requestElement->FirstChild() == NULL) {
    requestElement->LinkEndChild(new TiXmlElement("designmatrix"));
    requestElement->FirstChild()->LinkEndChild(new TiXmlElement("matrix"));
    requestElement->FirstChild()->LinkEndChild(new TiXmlElement("columnnames"));
    requestElement->FirstChild()->LinkEndChild(
        new TiXmlElement("isofinterest"));
    requestElement->FirstChild()->LinkEndChild(new TiXmlElement("artifacts"));
  }

  // step through request element looking for a designmatrix request node
  for (TiXmlElement *dm = 0;
       (dm = (TiXmlElement*) requestElement->IterateChildren(
           "designmatrix", dm));) {

    // step through designmatrix request node, checking all nodes for valid
    // part requests
    for (TiXmlElement *dmPart = 0;
         (dmPart = (TiXmlElement*) dm->IterateChildren(dmPart));) {

      // if matrix was requested, add to designmatrix
      if (!strcmp(dmPart->Value(), "matrix")) {

        // add an element for the matrix output
        TiXmlElement *matrix = new TiXmlElement("matrix");
        matrix->SetAttribute("rows", getNumRows());
        matrix->SetAttribute("cols", getNumColumns());

        // build text string containing the matrix elements separated by spaces
        stringstream matrixStr;
        matrixStr << fixed << setprecision(NUM_SIGFIGS);

        // loop through design matrix rows
        for (unsigned int r = 0; r < getNumRows(); r++) {

          // pull out row r
          vnl_vector<double> row = getRow(r);

          // loop through each column
          for (unsigned int c = 0; c < getNumColumns(); c++) {

            // put c-th element of row r onto stream
            matrixStr << row[c] << " ";
          }
        }

        // put string into xml text node
        TiXmlText *matrixText = new TiXmlText(matrixStr.str());
        matrix->LinkEndChild(matrixText);

        // add matrix element to designmatrix
        designmatrix->LinkEndChild(matrix);
      }

      // if column names were requested, add to designmatrix
      if (!strcmp(dmPart->Value(), "columnnames")) {
        // add an element for the columnnames vector
        TiXmlElement *columnnames = new TiXmlElement("columnnames");

        // loop through columns
        stringstream nameStr;
        for (unsigned int c = 0; c < getNumColumns(); c++) {
          nameStr << getColumnName(c) << " ";
        }

        // put string into xml text node
        TiXmlText *namesText = new TiXmlText(nameStr.str());
        columnnames->LinkEndChild(namesText);

        // add columnnames element to designmatrix
        designmatrix->LinkEndChild(columnnames);
      }

      // if isofinterest vector was requested, add to designmatrix
      if (!strcmp(dmPart->Value(), "isofinterest")) {
        // add an element for the isofinterest vector
        TiXmlElement *isofinterest = new TiXmlElement("isofinterest");

        // loop through interest vector
        stringstream interestStr;
        for (unsigned int c = 0; c < getNumColumns(); c++) {
          interestStr << isColumnOfInterest(c) << " ";
        }

        // put string into next node
        TiXmlText *interestText = new TiXmlText(interestStr.str());
        isofinterest->LinkEndChild(interestText);

        // add isofinterest element to designmatrixDesignMatrix
        designmatrix->LinkEndChild(isofinterest);
      }

      // if artifacts vector is requested, add them (as a list of TRs) to
      // the designmatrix
      if (!strcmp(dmPart->Value(), "artifacts")) {

        // add an element for the artifacts
        TiXmlElement *artifacts = new TiXmlElement("artifacts");

        // get vector of artifact timepoints
        vnl_vector<double> artTR = getArtifactTimepoints();

        stringstream artStr;
        artStr << fixed << setprecision(NUM_SIGFIGS);

        // loop through vector of artifact timepoints
        for (unsigned int a = 0; a < artTR.size(); a++) {

          // put the a-th element onto the stream
          artStr << artTR[a] << " ";
        }

        // put the string into xml text node
        TiXmlText *artText = new TiXmlText(artStr.str());
        artifacts->LinkEndChild(artText);

        // add artifacts element to designmatrix
        designmatrix->LinkEndChild(artifacts);
      }

      // if a single column is requested, add it to the designmatrix
      if (!strcmp(dmPart->Value(), "column")) {

        // add an element for the column output
        TiXmlElement *column = new TiXmlElement("column");

        // note: don't need number of rows because it's only one column,
        // so reshaping isn't necessary

        // get the requested column from given name attribute
        vnl_vector<double> requestedColumn =
            getColumn(dmPart->Attribute("name"));

        // build text string containing the column elements separated by spaces
        stringstream columnStr;
        columnStr << fixed << setprecision(NUM_SIGFIGS);

        // loop through the rows of the selected column
        for (unsigned int r = 0; r < getNumRows(); r++) {

          // put the r-th element of column onto stream
          columnStr << requestedColumn[r] << " ";
        }

        // put string into xml text node
        TiXmlText *columnText = new TiXmlText(columnStr.str());
        column->LinkEndChild(columnText);

        // add column element to designmatrix
        designmatrix->LinkEndChild(column);
      }
    }
  }

  return designmatrix;
}

// unserialize the data received as xml and make appropriate changes to design
// TODO this monolithic function should be split up
void RtDesignMatrix::unserializeXML(TiXmlElement *element) {

  // step through request element looking for a designmatrix request node
  for (TiXmlElement *dm = 0;
       (dm = (TiXmlElement*) element->IterateChildren("designmatrix", dm));) {

    // step through designmatrix request node, checking all nodes for valid
    // part requests
    for (TiXmlElement *dmPart = 0;
         (dmPart = (TiXmlElement*) dm->IterateChildren(dmPart));) {

      // look for condition tags (implies setting design matrix via infoserver
      if (!strcmp(dmPart->Value(), "condition")) {

        TiXmlElement *condition = dmPart;

        //get condition name TODO put this somewhere meaningful
        const char *conditionName = condition->Attribute("name");
        if (conditionName == NULL) {
          cerr << "Specified attribute 'condtion name' does not exist" << endl;
          continue;
        }

        // initalize variables to be filled by parsing
        vector<double> onsets, durations, column, weights;

        // loop through condition node to find parts and act accordingly
        for (TiXmlElement *condPart = 0;
             (condPart = (TiXmlElement*) condition->IterateChildren(
                 condPart));) {

          // read through getText and fill up onsets
          if (!strcmp(condPart->Value(), "onsets")) {
            stringstream onsetsStr;
            onsetsStr << condPart->GetText();

            // this goes through string and puts each onset into the onset
            // vector
            unsigned int i = 0;
            while (!onsetsStr.eof()) {
              // resize onset vector to hold latest element
              onsets.resize(onsets.size() + 1);

              // puts latest onset into onset vector
              onsetsStr >> onsets.at(i);

              // iterate counter
              i++;
            }
            continue;
          }

          // read through getText and fill up durations
          if (!strcmp(condPart->Value(), "durations")) {

            stringstream durationsString;
            durationsString << condPart->GetText();

            // this goes through string and puts each onset into the onset
            // vector
            unsigned int i = 0;
            while (!durationsString.eof()) {
              // resize vector to hold latest element
              durations.resize(durations.size() + 1);

              // puts latest value into vector
              durationsString >> durations.at(i);

              // iterate counter
              i++;

              // TODO: durations should be either 1 or the same length as
              // onsets.  this check'll have to be done later probably
            }
            continue;
          }

          // read through GetText and fill up column
          if (!strcmp(condPart->Value(), "column")) {
            // read in column and add it
            stringstream columnString;
            columnString << condPart->GetText();

            // this goes through string and puts the column into the column
            // vector
            unsigned int i = 0;
            while (!columnString.eof()) {
              // resize vector to hold latest element
              column.resize(column.size() + 1);

              // puts latest value into vector
              columnString >> column.at(i);

              // iterate counter
              i++;

              // todo will need check on number of elements
            }
            continue;
          }

          // read through GetText and fill up weights
          if (!strcmp(condPart->Value(), "weights")) {
            stringstream weightsString;
            weightsString << condPart->GetText();

            // this goes through string and puts the column into the column
            // vector
            unsigned int i = 0;
            while (!weightsString.eof()) {
              // resize vector to hold latest element
              weights.resize(weights.size() + 1);

              // puts latest value into vector
              weightsString >> weights.at(i);

              // iterate counter
              i++;
            }
            continue;
          }
        }

        // now that we have either onsets/durations or a column, do
        // appropriate things
        if (!onsets.empty() && !durations.empty()) {
          for (unsigned int ondur = 0; ondur < onsets.size(); ondur++) {
            // if there is only one duration, use it for all onsets
            // to add events
            if (durations.size() == 1) {
              addEvent(onsets[ondur], durations[0]);
            }
            else if (durations.size() == onsets.size()) {
              addEvent(onsets[ondur], durations[ondur]);
            }
            else {
              cerr << "mismatch between onsets and durations" << endl;
              continue;
            }
          }
        }
        else if (!column.empty()) {
          // todo
        }
        else {
          // something is missing
        }

      }

      // look for conditionShift (should accompany a condition)
      if (!strcmp(dmPart->Value(), "conditionShift")) {
        if (!RtConfigVal::convert<unsigned int>(conditionShift,
                                                dmPart->GetText())) {
          string errString = "error converting conditionShift";
          cerr << errString << endl;
          continue;
        }
      }

      // look for matrix tags (implies setting full matrix via infoserver)
      if (!strcmp(dmPart->Value(), "matrix")) {

        TiXmlElement *matrix = dmPart;

        // then loop through rows,cols and fill in each number accordingly
        unsigned int rows, cols;

        // read rows attribute
        const char *attribute = matrix->Attribute("rows");
        if (attribute == NULL) {
          cerr << "Specified attribute 'rows' does not exist" << endl;
          continue;
        }
        if (!RtConfigVal::convert<unsigned int>(rows, attribute)) {
          string errString = "error converting rows attribute to integer";
          cerr << errString << endl;
          continue;
        }

        // read cols attribute
        attribute = matrix->Attribute("cols");
        if (attribute == NULL) {
          cerr << "Specified attribute 'cols' does not exist" << endl;
          continue;
        }
        if (!RtConfigVal::convert<unsigned int>(cols, attribute)) {
          string errString = "error converting cols attribute to integer";
          cerr << errString << endl;
          continue;
        }

        // allocate space for matrix
        clear();
        set_size(rows,cols);
        fill(0);

        // read matrix data text into string
        stringstream matrixStr;
        matrixStr << matrix->GetText();

        // create some iteration variables
        unsigned int r = 0;
        unsigned int c = 0;

        // loop through string and read into design matrix
        while (!matrixStr.eof()) {

          // read a number from string into design matrix and
          // move to next column
          matrixStr >> this->operator()(r, c);
          c++;

          // wrap rows when we reach the end of the columns
          if (c == cols) {
            c = 0;
            r++;
          }

          // if we hit the end, break out
          if (r > rows) {
            cerr << "There is a descrepancy between stated number of rows "
                 << "and actual numbers" << endl;
            break;
          }
        }
      }

      // look for cell tags (implies setting single element in design matrix
      // via infoserver)
      if (!strcmp(dmPart->Value(), "cell")) {

        TiXmlElement *cell = dmPart;

        // read cell for row,col,data
        unsigned int row, col;
        double cellValue;

        const char *attribute = cell->Attribute("row");
        if (attribute == NULL) {
          cerr << "Specified row does not exist" << endl;
          continue;
        }
        if (!RtConfigVal::convert<unsigned int>(row, attribute)) {
          string errString = "error converting row attribute to integer";
          cerr << errString << endl;
          continue;
        }

        attribute = cell->Attribute("col");
        if (attribute == NULL) {
          cerr << "Specified col does not exist" << endl;
          continue;
        }
        if (!RtConfigVal::convert<unsigned int>(col, attribute)) {
          string errString = "error converting col attribute to integer";
          cerr << errString << endl;
          continue;
        }

        if (!RtConfigVal::convert<double>(cellValue, cell->GetText())) {
          string errString = "error converting cell value attribute to double";
          cerr << errString << endl;
          continue;
        }

        // check bounds on row and col values and put new value into design
        // matrix
        if ((row >= 0 && row < getNumRows()) &&
            (col >= 0 && col < getNumColumns())) {
          put(row, col, cellValue);
        }
        else {
          cerr << "Invalid number of rows or columns." << endl;
          continue;
        }
      }

      // set artifact for tr if it is sent
      if (!strcmp(dmPart->Value(), "artifact")) {
        TiXmlElement *artifact = dmPart;
        bool isArtifact;
        //                }
        if (!RtConfigVal::convert<bool>(isArtifact, artifact->GetText())) {
          string errString = "error converting artifact attribute to boolean";
          cerr << errString << endl;
          continue;
        }
        addArtifact(tr);
      }

      // look for columninfo tags
      if (!strcmp(dmPart->Value(), "columninfo")) {

        TiXmlElement *columninfo = dmPart;

        // declare index
        unsigned int index;

        // go through columninfo
        for (TiXmlElement *columnPart =
                 (TiXmlElement*) columninfo->FirstChildElement();
             columnPart != NULL;
             columnPart = (TiXmlElement*) columnPart->NextSiblingElement()) {

          bool isOfInterest;

          // find index
          if (!strcmp(columnPart->Value(), "index")) {
            if (!RtConfigVal::convert<unsigned int>(index,
                                                    columnPart->GetText())) {
              string errString = "error converting index attribute to integer";
              cerr << errString << endl;
              continue;
            }
          }
          else {
            // check that index has already been set
            if (index == NULL) {
              cerr << "Index is not set" << endl;
              continue;
            }
          }
          // break to next set of columninfo if index is too large
          if (index > getNumColumns() - 1) {
            string errString = "Index exceeds design matrix dimensions";
            cerr << errString << endl;
            break;
          }

          // set new name if it is being set
          if (!strcmp(columnPart->Value(), "name")) {
            columnNames[index] = columnPart->GetText();
            continue;
          }

          // set new isOfInterest value if it is being set
          if (!strcmp(columnPart->Value(), "isofinterest")) {
            if (!RtConfigVal::convert<bool>(isOfInterest,
                                            columnPart->GetText())) {
              string errString = "error converting index attribute to integer";
              cerr << errString << endl;
              continue;
            }
            columnOfInterestIndicator[index] = isOfInterest;
          }
        }
      }
    }
  }
}
