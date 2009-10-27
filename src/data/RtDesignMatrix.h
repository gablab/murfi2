/******************************************************************************
 * RtDesignMatrix.h declares a class that represents a model's basis
 * functions
 *
 * NOTE: much of the funcitonality is inhereted from vnl_matrix, so see this
 * help page for usage of this class: 
 * http://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__matrix.html
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-21
 * 
 *****************************************************************************/

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

#include "add_functions.h"

using namespace std;

// class declaration

class RtDesignMatrix : public RtData, protected vnl_matrix<double> {
public:

    //*** constructors/destructors  ***//

    RtDesignMatrix();

    // destructor

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

    //unsigned int getNumMeas() { return numMeas; }

    // build the design matrix from the configuration
    virtual bool build();

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

    // serialize the data as xml for transmission or saving to a file
    TiXmlElement *serializeAsXML(TiXmlElement *requestElement);

    // unserialize the data received as xml and process
    void unserializeXML(TiXmlElement *element);

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

    //   // a column to the design matrix
    //   //  in
    //   //   vals:     desired values
    //   //   interest: whether this column is a basis function representing neural
    //   //             activation of interest
    //   //  out
    //   //   index of column added, or UINT_MAX otherwise
    //   unsigned int addColumn(double  const * const vals,
    // 			 string name,
    // 			 bool interest = false);

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

    //  // set the values of a column
    //  //  in
    //  //   c:        index of the column to set
    //  //   vals:     desired values
    //  //   interest: whether this column is a basis function representing neural
    //  //             activation of interest
    //  //  out
    //  //   true if assigned
    //  //   false if index out of bounds
    //  bool setColumn(unsigned int c,
    //		 double const * const vals,
    //		 bool interest = false);
    //
    //  // set the values of a column
    //  //  in
    //  //   c:        index of the column to set
    //  //   vals:     desired values
    //  //   interest: whether this column is a basis function representing neural
    //  //             activation of interest
    //  //  out
    //  //   true if assigned
    //  //   false if index out of bounds
    //  bool setColumn(unsigned int c,
    //		 const vnl_vector<double> &vals,
    //		 bool interest = false);

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

    // do the actual design matrix construction
    bool buildDesignMatrix();

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

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


