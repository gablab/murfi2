/******************************************************************************
 * RtModelFit.cpp is the implementation of a base class for
 * any method for estimation of activation
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#include"RtModelFit.h"

#include"gsl/gsl_cdf.h"

// for gamma functions
#include<vnl/vnl_gamma.h>

// for hrf file reading
//#include<vcl/vcl_istream.h>

#include"debug_levels.h"

string RtModelFit::moduleString("model-fit");


// default constructor

RtModelFit::RtModelFit() : RtStreamComponent() {
  // standard init
  componentID = moduleString;
}

// destructor

RtModelFit::~RtModelFit() {
}


// configure this stream component
//  in
//   xml module node from which to read <option>s

bool RtModelFit::init(TiXmlElement *module, RtConfig *config, RtConductor *_conductor) {
    ACE_TRACE(("RtModelFit::init"));

    bool ret = true;
    // set the tr and number of measurements
    if (config->isSet("scanner:disabled")
            && config->get("scanner:disabled") == false
            && !config->isSet("scanner:tr")) {
        cerr << "ERROR: tr has not been set" << endl;
        ret = false;
    }
    else {
        design.setTR(config->get("scanner:tr"));
    }

    if (config->isSet("scanner:disabled")
            && config->get("scanner:disabled") == false
            && !config->isSet("scanner:measurements")) {
        cerr << "ERROR: number of measurements has not been set" << endl;
        ret = false;
    }
    else {
        design.setNumMeas(config->get("scanner:measurements"));
    }

    // find the design and process it
    TiXmlNode *designNode = module->FirstChild("design");
    if (designNode == NULL) {
        cerr << "ERROR: no model design configuration found" << endl;
        ret = false;
    }
    else if (designNode->Type() != TiXmlNode::ELEMENT) {
        cerr << "ERROR: model design is not an xml element" << endl;
        ret = false;
    }
    TiXmlElement *designElmt = (TiXmlElement*) designNode;
    if (!design.configure(designElmt, *config)) {
        cerr << "ERROR: error in model design configuration" << endl;
        ret = false;
    }
    else {
        if (!design.build()) {
            cerr << "ERROR: error in model design configuration" << endl;
            ret = false;
        }
        else {
            getDataStore().setData(&design);
        }
    }

    return RtStreamComponent::init(module, config, _conductor) && ret;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node

bool RtModelFit::processOption(const string &name,
                               const string &text,
                               const map<string, string> &attrMap) {

    return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration

bool RtModelFit::validateComponentConfig() {
    bool result = true;

    if (maskRoiID == "unset") {
        cerr << "ERROR: maskRoiID must be set to do intensity normalization" << endl;
        result = false;
    }

    return result;
}


// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits

void RtModelFit::initEstimation(const RtData &dat, RtMaskImage *mask) {

    // store the data id as a template
    templateDataID = dat.getDataID();
    templateDataID.setTimePoint(DATAID_UNSET_VALUE);
    design.setTemplateDataID(templateDataID);

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


