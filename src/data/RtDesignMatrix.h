/*=========================================================================
 *  RtDesignMatrix.h declares a class that represents a model's basis
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

#ifndef RTDESIGNMATRIX_H
#define RTDESIGNMATRIX_H

#include"RtData.h"
#include"RtDataID.h"
#include"RtConfig.h"

#include<string>
#include"tinyxml/tinyxml.h"

#include<vnl/vnl_matrix.h>
#include<vnl/vnl_vector.h>
#include<vector>
#include<map>

using namespace std;

class RtDesignMatrix : public RtData, protected vnl_matrix<double> {
 public:

  //*** constructors/destructors  ***//

  RtDesignMatrix();

  virtual ~RtDesignMatrix() {
  };

  // configure the design
  virtual bool configure(TiXmlElement *designEle,
                         const RtConfig &config);

  // process a configuration option
  //  in
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text,
                             const map<string, string> &attrMap);

  // set the template input data id

  void setTemplateDataID(const RtDataID &id) {
    templateDataID = id;
  }

  // set the number of measurements
  // NOTE: must be called before build()

  void setNumMeas(unsigned int _numMeas) {
    numMeas = _numMeas;
  }

  // set the tr
  // NOTE: must be called before build()

  void setTR(double _tr) {
    tr = _tr;
  }

  // tools for outside manipulation before build

  // get the name of a condition
  string getConditionName(size_t index) {
    if (index >= numInputConditions) {
      return "";
    }

    return conditionNames[index];
  }

  // get the number of existing input conditions
  size_t getNumInputConditions() {
    return numInputConditions;
  }

  // add an empty condition and set it to all zeros
  void addCondition(const string &name, bool of_interest);

  // set the value of one TR for a condition
  void setConditionValueAtTR(size_t tr, size_t condition, double val);

  // get the value of one TR for a condition
  double getConditionValueAtTR(size_t tr, size_t condition);

  // build the design matrix from the configuration
  virtual bool build();

  double getTR() const {
    return tr;
  }

  int getNumMeas() const {
    return numMeas;
  }

  // get the number of design matrix rows

  unsigned int getNumRows() {
    return rows();
  }

  // get the number of design matrix columns

  unsigned int getNumColumns() {
    return columns();
  }

  // get the name of a design matrix column
  string getColumnName(unsigned int i);

  // get a design matrix column by name
  vnl_vector<double> getColumn(const string &name);

  // get a design matrix column by index
  vnl_vector<double> getColumn(unsigned int index);

  // get a design matrix column index
  unsigned int getColumnIndex(const string &name);

  size_t getNumInputConditions() const {
    return numInputConditions;
  }

  // get event duration

  unsigned int getMaxTrendOrder() {
    return maxTrendOrder;
  }

  // retreive a vector of the indices of columns identified as representing
  // neural signals of interest
  vnl_vector<unsigned int> getColumnOfInterestIndices();

  // determine whether a column is a basis function representing neural
  // signals of interest or not
  bool isColumnOfInterest(unsigned int col);

  // get the hrf vector this design uses
  vnl_vector<double> &getHrf();

  // get the hrf vector without sampling to the tr
  vnl_vector<double> &getHiresHrf();

  // get the temporal derivative basis vector this design uses
  vnl_vector<double> &getTemporalDerivativeBasis();

  // get the temporal derivative basis without sampling to the tr
  vnl_vector<double> &getHiresTemporalDerivativeBasis();

  // get event duration
  double getEventDuration() {
    return eventDuration;
  }

  // add event to be modeled
  // in
  //   onset (trs) of the event
  //   duration (trs) of the event
  bool addEvent(double onset, double duration = 0.0);

  // add artifact to be modeled
  // in
  //   tr of the artifact
  bool addArtifact(unsigned int tr);

  // get a vector of all timepoints with an artifact
  vnl_vector<double> getArtifactTimepoints();

  // build a row of the GLM deign matrix
  // in
  //  current image
  vnl_vector<double> getRow(unsigned int timepoint);

  // get the covariance of a contrast given a contrast vector
  // if this matrix is X, this function computes c' pinv(X'X) c
  double computeContrastCovariance(vnl_vector<double> &contrastVector);

  // update the design matrix for a new timepoint
  // NOTE: this should be called after all events, artifacts, and motion has
  // been processed for this timepoint. usually it will be called from
  // RtModelFit::process()
  //  in
  //   tr of the timepoint to update for
  bool updateAtTr(unsigned int thisTr);

  // print the design
  void print();

  // save the design to a text file
  bool save(string const& filename) const;

  // serialize the data as xml for transmission or saving to a file
  TiXmlElement *serializeAsXML(TiXmlElement *requestElement);

  // unserialize the data received as xml and process
  void unserializeXML(TiXmlElement *element);

  // dummy stub
  virtual char* ditherTo8Bpp() { return NULL; }

  // do the actual design matrix construction
  bool buildDesignMatrix();

 protected:

  // sets the size (old data lost)
  bool set_size(unsigned int row, unsigned int col);

  // get the index of the next free design matrix column
  // NOTE: increments a counter and maintains an internal state, so this
  // function should only be called once per column and should be called for
  // every column

  // retreive the number of neural signal bases in this model
  virtual unsigned int getNumConditionBases();

  // retreive the number of nuisance bases in this model
  virtual unsigned int getNumNuisanceBases();

  // a column to the design matrix
  //  in
  //   vals:     desired values
  //   interest: whether this column is a basis function representing neural
  //             activation of interest
  //  out
  //   index of column added, or UINT_MAX otherwise
  unsigned int addColumn(const vnl_vector<double> &vals,
                         string name,
                         bool interest = false);

  // loads an hrf vector from a file
  virtual bool loadHrfFile(vnl_vector<double> &hrf, string filename);

  // builds the hrf vector associated with this design as well as the
  // temporal derivative basis
  virtual bool buildHrf();

  // convolves a vector with the hrf for this design
  vnl_vector<double> convolveVecWithHrf(const vnl_vector<double> &v);

  // convolves a vector with the temporal derivative basis
  vnl_vector<double> convolveVecWithTemporalDerivative(const vnl_vector<double> &v);

  // read design matrix information from a text file
  bool loadDesignMatrixFile(string filename);

  // data members
  bool isBuilt;
  unsigned int numAddedColumns; // number of columns filled in
  vector<bool> columnOfInterestIndicator; // columns of interest are true
  vector<string> columnNames; // vector of column names
  RtDataID templateDataID; // the go-by template id for
  // incoming data

  unsigned int numMeas; // total expected
  double tr;


  // neural signal basis parms
  vector<string> conditionNames; // vector of condition names
  vector<bool> conditionInterestIndicator; // whether conditions should be
                                           // included in feedback signal
  unsigned int conditionShift; // insert this many zeros at the
  // begining of the design (allows
  // estimation algo to stabilize)
  vnl_matrix<double> inputConditions; // hold the condition bases
  // here temporarily before we copy them
  // to the design matrix
  unsigned int numInputConditions;

  // for block design, have the choice of spliting each block into a
  // separate regressor, even for the same condition type. this potentially
  // helps account for the variability in BOLD response magnitude between
  // blocks
  bool modelEachBlock;
  unsigned int blockLen;

  // hrf spec
  vnl_vector<double> hrf;
  vnl_vector<double> hiresHrf;
  vnl_vector<double> temporalDerivative;
  vnl_vector<double> hiresTemporalDerivative;
  bool hrfIsBuilt;

  // cannonical hrf parameters
  double hrfSamplePeriod; // temporal precision for hrf sampling (s)
  double hrfLength; // seconds
  double hrfTimeToPeakPos;
  double hrfTimeToPeakNeg;
  double hrfPosToNegRatio;

  // load design matrix from file
  bool loadDesignMatrixFromFile;
  string designMatrixFilename;

  // pad with zeros
  bool padConditionWithZeros;

  // load hrf from file (overrides cannonical hrf)
  bool loadHrf;
  string hrfFilename;


  // nuisance bases parms
  vnl_matrix<double> tmpNuisance; // hold the nuisance bases
  // here temporarily

  unsigned int maxTrendOrder; // add up to this order temporal trends
  bool modelMotionParameters; // regressors for motion
  bool modelTemporalDerivatives; // regressors for temporal derivatives
  // (look into adding these as conditions
  // of interest)

  // single neural event bases
  //
  // these nuisance regressors model neural events of no interest for your
  // real-time analysis.  for example, in a brain state triggering
  // experiment you do not want the evoked response from the triggered
  // stimulus to go as unaccounted for variance.
  bool modelEvents;
  unsigned int eventDuration; // duration in trs of the events of interest
  unsigned int maxNumEvents; // needed to know how many columns to
  // reserve for the design matrix. adding
  // unnecessary columns will slow down
  // estimation a lil.
  unsigned int numEvents; // so far

  // image artifact bases
  //
  // these nuisance regressors model artifacts in the image
  // timeseries. duration is always 1 and we do not convolve with the hrf.
  bool modelArtifacts;
  vnl_vector<double> artifactBases;
  unsigned int maxNumArtifacts; // needed to know how many columns to
  // reserve for the design matrix. adding
  // unnecessary columns will slow down
  // estimation a lil.
  unsigned int numArtifacts; // so far
};

#endif
