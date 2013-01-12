/*=========================================================================
 *  RtSingleImageCor.cpp is the implementation of a class that computes the
 *  activation in a single image based on Hinds, et al., 2008
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

#include"RtSingleImageCor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include"RtElementAccess.h"
#include<limits>

#include"debug_levels.h"


string RtSingleImageCor::moduleString(ID_SINGLEIMCOR);

// default constructor

RtSingleImageCor::RtSingleImageCor() : RtIncrementalGLM() {
    componentID = moduleString;

    //  residualSumSqImg = NULL;

    estErrSum = NULL;
    numDataPointsForErrEst = std::numeric_limits<unsigned int>::max();
    errorNorm = L2;


    numTimepoints = 0;

    onlyEstErrInBaseline = false;

    includeConditionMean = true;
}

// destructor

RtSingleImageCor::~RtSingleImageCor() {
    if (estErrSum != NULL) {
        freeEstErrSum();
    }
}

void RtSingleImageCor::freeEstErrSum() {
    if (estErrSum != NULL) {
        delete estErrSum;
    }
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node

bool RtSingleImageCor::processOption(const string &name, const string &text,
                                     const map<string, string> &attrMap) {

    // look for known options
    if (name == "numDataPointsForErrEst") {
        return RtConfigVal::convert<unsigned int>(numDataPointsForErrEst, text);
    }
    if (name == "errorNorm") {
        if (text == "l1" || text == "L1") {
            errorNorm = L1;
            return true;
        }
        else if (text == "l2" || text == "L2") {
            errorNorm = L2;
            return true;
        }
        else if (text == "linf" || text == "LINF" || text == "LInf") {
            errorNorm = LINF;
            return true;
        }
    }
    if (name == "includeConditionMean") {
        return RtConfigVal::convert<bool>(includeConditionMean, text);
    }
    if (name == "onlyEstErrInBaseline") {
        return RtConfigVal::convert<bool>(onlyEstErrInBaseline, text);
    }
    if (name == "dumpAlgoVars") {
        return RtConfigVal::convert<bool>(dumpAlgoVars, text);
    }
    if (name == "dumpAlgoVarsFilename") {
        dumpAlgoVarsFilename = text;
        return true;
    }

    return RtIncrementalGLM::processOption(name, text, attrMap);
}

// validate the configuration

bool RtSingleImageCor::validateComponentConfig() {
    bool result = true;

    return RtIncrementalGLM::validateComponentConfig() && result;
}

// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits

void RtSingleImageCor::initEstimation(RtMRIImage &dat, RtMaskImage *mask) {
    RtIncrementalGLM::initEstimation(dat, mask);

    // setup the estimation error sum of squares
    if (estErrSum != NULL) {
        freeEstErrSum();
    }

    estErrSum = new RtActivation(dat);

    numDataPointsCount = 0;

    // open the dump file
    if (dumpAlgoVars) {
        dumpFile.open(dumpAlgoVarsFilename.c_str());
        if (dumpFile.fail()) {
            cout << "couldn't open file to dump vars" << endl;
            dumpAlgoVars = false;
        }
        else {
            cout << "opened file " << dumpAlgoVarsFilename << " to dump vars" << endl;
            startDumpAlgoVarsFile();
        }
    }

}

// process a single acquisition

int RtSingleImageCor::process(ACE_Message_Block *mb) {
    ACE_TRACE(("RtSingleImageCor::process"));

    RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

    // get the current image to operate on
    RtMRIImage *dat = (RtMRIImage*) msg->getCurrentData();

    if (dat == NULL) {
        cout << "RtSingleImageCor::process: data passed is NULL" << endl;

        ACE_DEBUG((LM_INFO, "RtSingleImageCor:process: data passed is NULL\n"));
        return 0;
    }

    RtMaskImage *mask = getMaskFromMessage(*msg);

    numTimepoints++;

    // initialize the computation if necessary
    if (needsInit) {
        initEstimation(*dat, mask);
    }

    // ohinds: 2009-02-16 removed functionality
    //  // validate sizes
    //  if(dat->getNumEl() != numData) {
    //    ACE_DEBUG((LM_INFO, "RtSingleImageCor::process: new data has wrong number of elements\n"));
    //    return -1;
    //  }
    //
    // allocate a new data images for the stats
    RtActivation *stat = new RtActivation(*dat);

    // setup the data id
    stat->getDataID().setFromInputData(*dat, *this);
    stat->getDataID().setDataName(NAME_ACTIVATION);
    stat->getDataID().setRoiID(mask->getDataID().getRoiID());

    stat->initToZeros();

    //  // set threshold
    //  if(numTimepoints > getNumNuisanceRegressors()+1) {
    //    stat->setThreshold(getTStatThreshold(1));
    //    if(DEBUG_LEVEL & BASIC) {
    //      cerr << "single image est: using t threshold of "
    //	   << stat->getThreshold() << endl;
    //    }
    //  }

    // residual sum of squares map
    RtActivation *res = new RtActivation(*dat);
    // setup the data id
    res->getDataID().setFromInputData(*dat, *this);
    res->getDataID().setDataName(NAME_RESIDUAL_MSE);
    res->getDataID().setRoiID(mask->getDataID().getRoiID());
    res->initToZeros();

    //// element independent setup

    /// build a design matrix row
    vnl_vector<double> Xrow = design.getRow(dat->getDataID().getTimePoint());

    bool anyOverZero = false;
    for (unsigned int i = 0; i < design.getNumColumns(); i++) {
        // check for on condition for any stimulus
        if (design.isColumnOfInterest(i)
                && Xrow[i] > std::numeric_limits<double>::epsilon()) {
            anyOverZero = true;
            break;
        }
    }


    if (DEBUG_LEVEL & MODERATE) {
        cout << " xrow " << numTimepoints << ":";
        for (unsigned int i = 0; i < Xrow.size(); i++) {
            cout << Xrow[i] << " ";
        }
        cout << endl;
    }

    // check if we should include this timepoint in variance computation
    bool includeInErr;
    if ((numTimepoints > numDataPointsForErrEst)
            || (onlyEstErrInBaseline && anyOverZero)) {
        includeInErr = false;
    }
    else {
        includeInErr = true;
        numDataPointsCount++;
    }

    //// compute t map for each element
    RtElementAccess elAc(dat, mask);
    vector<unsigned int> indices = elAc.getElementIndices();
    unsigned int curSolver = 0;
    for (vector<unsigned int>::iterator i = indices.begin(); i != indices.end();
            i++, curSolver++) {
        if (!mask->getPixel(*i)) {
            stat->setPixel(*i, numeric_limits<double>::quiet_NaN()); // assign nan
            res->setPixel(*i, numeric_limits<double>::quiet_NaN()); // assign nan
            continue;
        }

        // include this timepoint in the solver for this voxel
        double y = dat->getElement(*i);
        solvers[curSolver]->include(&y, Xrow.data_block(), 1.0);

        // get stats and residual
        double *beta = solvers[curSolver]->regress(0);

        // note that residual is always the standard L2 version, despite which
        // norm we are using for the stat scaling
        res->setPixel(*i, sqrt(solvers[curSolver]->getTotalSquaredError(0) / (numTimepoints - 1)));

        // get activation signal take out everything except regressor of interest
        //    unsigned int regressorOfInterestIndex = conditionOfInterest;
        //    if(modelEachBlock) {
        //      unsigned int blockNum = (numTimepoints > conditionShift)
        //	? (numTimepoints-1-conditionShift)/blockLen : 0;
        //      regressorOfInterestIndex
        //	= conditionOfInterest * numMeas/blockLen + blockNum;
        //    }
        //    if(modelTemporalDerivatives) {
        //      regressorOfInterestIndex*=2;
        //      //regressorOfInterestIndex++;
        //    }
        //    regressorOfInterestIndex += getNumNuisanceRegressors();

        // compute stat
        double err = y;
        double meanCondActivity = 0;
        for (unsigned int j = 0; j < Xrow.size(); j++) {
            if (!design.isColumnOfInterest(j)) {
                err -= beta[j] * Xrow[j];
            }
            else {
                meanCondActivity += beta[j] * Xrow[j];
            }
        }

        if (DEBUG_LEVEL & ADVANCED) {
            static double lastErr = 0;
            cerr << *i << ": err is " << err << " ?= " << sqrt(solvers[curSolver]->getTotalSquaredError(0)) - lastErr << endl;
            lastErr = sqrt(solvers[curSolver]->getTotalSquaredError(0));
        }


        // update the error in the estimate for the voxel

        // get estimation error and compute the standard deviation based on
        // the error sum and current error norm and include the error in the
        // estimate if desired
        double dev;
        switch (errorNorm) {
        case L1:
            if (includeInErr) {
                estErrSum->setPixel(*i, estErrSum->getPixel(*i) + fabs(err));
            }

            dev = estErrSum->getPixel(*i) / (numDataPointsCount - 1);
            break;

        case L2:
        default:
            if (includeInErr) {
                estErrSum->setPixel(*i, estErrSum->getPixel(*i) + err * err);
            }

            dev = sqrt(estErrSum->getPixel(*i) / (numDataPointsCount - 1));
            break;

        case LINF:
            if (includeInErr) {
                estErrSum->setPixel(*i, std::max(estErrSum->getPixel(*i), fabs(err)));
            }

            dev = estErrSum->getPixel(*i);

            break;
        }

        if (DEBUG_LEVEL & ADVANCED) {
            cerr << dev << " ?= "
                    << sqrt(solvers[curSolver]->getTotalSquaredError(0) / (numTimepoints - 1))
                    << endl;
        }


        // compute the sds away from the mean (magic kinda happens here)
        if (includeConditionMean) {
            stat->setPixel(*i, (meanCondActivity + err) / dev);
        }
        else {
            stat->setPixel(*i, (meanCondActivity) + (err / dev));
        }

        if (DEBUG_LEVEL & ADVANCED) {
            cerr
                    << numTimepoints << " "
                    << *i << " "
                    << y << " "
                    << err + meanCondActivity << " "
                    << err << " "
                    << dev << " "
                    << stat->getPixel(*i) << " " << endl;
        }
        if (DEBUG_LEVEL & MODERATE) {
            cerr << err << "/" << dev << "=" << stat->getPixel(*i) << endl;
        }

        if (numTimepoints > 2) {
            dumpFile
                    << numTimepoints << " "
                    << *i << " "
                    << y << " "
                    << err + meanCondActivity << " "
                    << Xrow[0] << " "
                    << err << " "
                    << dev << " "
                    << stat->getPixel(*i) << " ";
            for (unsigned int b = 0; b < design.getNumColumns(); b++) {
                dumpFile << beta[b] << " ";
            }
            dumpFile << endl;
        }

        delete beta;
    }


    if (DEBUG_LEVEL & BASIC) {
        cout << "done processing single image correlation at ";
        printNow(cout);
        cout << endl;
    }

    // set the results
    setResult(msg, stat);
    setResult(msg, res);

    return 0;
}

// start a logfile

void RtSingleImageCor::startDumpAlgoVarsFile() {
    dumpFile << "started at ";
    printNow(dumpFile);
    dumpFile << endl
            << "time_point "
            << "voxel_index "
            << "voxel_intensity "
            << "activation_signal "
            << "condition "
            << "residual "
            << "std_dev "
            << "feedback ";
    for (unsigned int b = 0; b < design.getNumColumns(); b++) {
        dumpFile << "beta[" << b << "] ";
    }

    dumpFile << "end" << endl;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

