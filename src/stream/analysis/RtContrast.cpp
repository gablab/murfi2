/******************************************************************************
 * RtContrast.h is the implementation of a class that computes
 * activation stats based on a model fit
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-02-11
 *
 *****************************************************************************/

#include"RtContrast.h"
#include"RtExperiment.h"
#include"RtMRIImage.h"
#include"RtDataIDs.h"
#include"RtElementAccess.h"
#include"RtLimit.h"

#include"vnl/vnl_matrix.h"
#include"vnl/algo/vnl_svd.h"

string RtContrast::moduleString(ID_CONTRAST);

// default constructor
RtContrast::RtContrast() : RtStreamComponent(),
			   numInputConditions(0) {
  componentID = moduleString;
}

// destructor
RtContrast::~RtContrast() { }

// configure this stream component
//  in
//   xml module node from which to read <option>s
bool RtContrast::init(TiXmlElement *module, RtConfig *config, RtConductor *_conductor) {
  ACE_TRACE(("RtContrast::init"));

  // find the design and process it
  TiXmlNode *convec = module->FirstChild("contrastVector");
  if(convec == NULL) {
    cerr << "ERROR: no contrast vector found" << endl;
    return false;    
  }
  else if(convec->Type() != TiXmlNode::ELEMENT) {
    cerr << "ERROR: contrastVector is not an xml element" << endl;
    return false;
  }
  TiXmlElement *convecElmt = (TiXmlElement*) convec;
  
  // prepare the input vector (needed to store the vector temporarily
  // because the design matrix won't be built yet. see init() for real
  // building of the vector
  inputContrastVec.clear();
  inputContrastVec.set_size(MAX_CONDITIONS);
  inputContrastVec.fill(0);
  inputConditionNames.reserve(MAX_CONDITIONS);

  // find the name
  string getName;
  if(TIXML_SUCCESS != convecElmt->QueryValueAttribute("contrastName", getName)) {
    cerr << "ERROR: contrastName is a required attribute of the contrastVector option" << endl;
    return false;
  }
  contrastName = getName;

  // find the spec type
  string type;
  if(!(TIXML_SUCCESS == convecElmt->QueryValueAttribute("type", type)
       && (type != "vect" || type != "pairs" ))) {
    cerr << "ERROR: the \"type\" attribute of the contrastVector must be either \"vect\" or \"pairs\"" << endl;
      return false;
  }

  // handle a single double contrast vector input
  if(type == "vect") {
    string text(convecElmt->GetText());

    // parse the text string into a contrast vector
    double el;
    numInputConditions = 0;
    size_t i = 0;
    for(size_t i1 = 0, i2 = text.find(" ", 0); i2 <= text.size();
	i++, i1 = i2+1, i2 = text.find(" ", i1), numInputConditions++) {

      if(!RtConfigVal::convert<double>(el,
	       text.substr(i1,i2 >= text.size() ? text.size()-i1 : i2-i1))) {
	continue;
      }
      inputContrastVec.put(i,el);
    }   
  } 
  // handle a set of name/weight pairs
  else if(type == "pairs") {
    // iterate over weights
    string condName;
    double condWeight;
    TiXmlElement *weightElmt;
    for(TiXmlNode *weight = 0;
	(weight = convecElmt->IterateChildren("weight", weight)); 
	numInputConditions++) {
      if(weight->Type() != TiXmlNode::ELEMENT)
	continue;

      weightElmt = (TiXmlElement*) weight;

      if(TIXML_SUCCESS != weightElmt->QueryValueAttribute("condition", condName)) {
	continue;
      }

      // assign the name to the end
      inputConditionNames.push_back(condName);

      // get the weight
      RtConfigVal::convert<double>(condWeight, weightElmt->GetText());
      inputContrastVec.put(inputConditionNames.size()-1,condWeight);
    }
    
  }
    
  return RtStreamComponent::init(module, config, _conductor);
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtContrast::processOption(const string &name, const string &text,
				     const map<string,string> &attrMap) {
  // look for known options
  if(name == "estimationModuleID") {
    estimationModuleID = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtContrast::validateComponentConfig() {
  bool result = true;

  if(estimationModuleID.empty()) {
    cerr << "ERROR: estimationModuleID must be set to compute contrasts" << endl;
    result = false;
  }
  
  if(maskRoiID.empty()) {
    cerr << "ERROR: maskRoiID must be set to compute contrasts" << endl;
    result = false;
  }
  
  return result;
}


// initialize the contrast for a particular image size
// in
//  first acquired image to use as a template for parameter inits
bool RtContrast::initContrast(RtMRIImage &image) {

  // find the design matrix
  RtDataID desID;
  desID.setDataName(NAME_DESIGN);
  designMatrix = (RtDesignMatrix*) getDataStore().getData(desID);
  if(designMatrix == NULL) {
    cerr << "ERROR: RtContrast could not load the experimental design matrix. bailing." << endl;
    return false;
  }

  // resolve inputs to build a contrast vector
  
  // find the names associated with each nonzero weight
  contrastVector.reserve(numInputConditions);
  for(unsigned int c = 0; c < numInputConditions; c++) {
    if(abs(inputContrastVec[c]) > std::numeric_limits<double>::epsilon()) {
      string condName = (inputConditionNames.size() > c) 
	? inputConditionNames[c]       
	: designMatrix->getColumnName(c);
      contrastVector.push_back(RtContrastVecEntry(condName,inputContrastVec[c]));
    }
  }

  // build the contrast covariance
  vnl_vector<double> fullContrastVector(designMatrix->getNumColumns(),0.0);
  for(vector<RtContrastVecEntry>::iterator i = contrastVector.begin();
      i != contrastVector.end(); i++) {
    unsigned int index = designMatrix->getColumnIndex((*i).name);
    if(index == std::numeric_limits<unsigned int>::max()) {
      cerr << "ERROR: column " << (*i).name << " does not exist, skipping" << endl;
      continue;
    }

    fullContrastVector[index] = (*i).weight;
  }

  contrastCov = designMatrix->computeContrastCovariance(fullContrastVector);
  if(contrastCov == std::numeric_limits<double>::quiet_NaN()) {
    contrastCov = 1.0;
  }

  needsInit = false;

  return true;
}


// process a single acquisition
int RtContrast::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtContrast::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtContrast::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtContrast:process: data passed is NULL\n"));
    return 0;
  }

  RtMaskImage *mask = getMaskFromMessage(*msg);

  unsigned int timePoint = dat->getDataID().getTimePoint();

  // initialize the computation if necessary
  if(needsInit) {
    if(!initContrast(*dat)) {
      cerr << "ERROR: could not initialize contrast computation." << endl;
      return 0;
    }
  }

  // retreive the betas and residual map
  RtActivation *resMs = (RtActivation*) msg->getData(estimationModuleID, 
						     NAME_RESIDUAL_MSE);

  if(resMs == NULL) {
    cout << "RtContrast::process:  no residual image found" << endl;
    return 0;
  }

  // vector for betas of interest
  vector<RtActivation*> betas;
  betas.reserve(contrastVector.size());
  for(vector<RtContrastVecEntry>::iterator i = contrastVector.begin(); 
      i != contrastVector.end(); i++) {
    RtActivation *beta = (RtActivation*) msg->getData(estimationModuleID, 
					      NAME_BETA + "_" + (*i).name);
    if(beta) {
      betas.push_back(beta);
    }
    else {
      cout << "RtContrast::process:  no beta image found for condition " 
	   << (*i).name << endl;
      return 0;
    }
  }

  // allocate data images for con and stats 
  RtActivation *con = new RtActivation(*dat);
  con->getDataID().setFromInputData(*dat,*this);
  con->getDataID().setDataName(string(NAME_CONTRAST) + "_" + contrastName);
  con->getDataID().setRoiID(mask->getDataID().getRoiID());

  con->initToNans();

  RtActivation *tstat = new RtActivation(*dat);
  tstat->getDataID().setFromInputData(*dat,*this);
  tstat->getDataID().setDataName(NAME_TSTAT + "_" + contrastName);
  tstat->getDataID().setRoiID(mask->getDataID().getRoiID());
  tstat->initToNans();

  //// compute weighted sum for each voxel
  RtElementAccess elAc(resMs, mask);
  vector<unsigned int> inds = elAc.getElementIndices();
  for(vector<unsigned int>::iterator it = inds.begin(); it != inds.end(); it++) {

    // compute the contrast value
    double sum = 0;
    vector<RtActivation*>::iterator image = betas.begin();
    vector<RtContrastVecEntry>::iterator vector = contrastVector.begin(); 
    for(; vector != contrastVector.end() && image != betas.end(); 
	vector++, image++) {
      sum += (*vector).weight * (*image)->getElement(*it);
    }
    con->setPixel(*it, sum);

    // compute the t stat
    double numerator = sum;
    double denominator = resMs->getElement(*it) 
      / (timePoint-designMatrix->getNumColumns())
      * contrastCov;

    if((DEBUG_LEVEL & MODERATE) && *it == 31472) {
      cout << numerator << " / " << denominator << " = " << numerator / denominator << endl;
    }


    if(denominator > std::numeric_limits<double>::epsilon()) {
      tstat->setPixel(*it, numerator / sqrt(denominator));
    }
  }

  setResult(msg,con);
  setResult(msg,tstat);


  if(print) {
    cout << "RtContrast: done at tr " 
	 << dat->getDataID().getTimePoint() << endl;
  }

  if(logOutput) {
    stringstream logs("");
    logs << "RtContrast: done at tr " 
	 << dat->getDataID().getTimePoint() << endl;
    log(logs);
  }


  return 0;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

