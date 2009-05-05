/**
 * infoclient is a mex interface to infoclientLib, which provides
 * communication with festr's infoserver
 *
 *    first argument is a command (success indicated with logical 0) :
 *       start: starts the infoclient.
 *              usage: success = infoclient('start', host, port)
 *
 *       add:    adds a data id to the notify list.
 *               usage: success = infoclient('add', dataName, roiName)
 *
 *       remove: removes a data id from the notify list.
 *               usage: success = infoclient('remove', dataName, roiName)
 *
 *       check:  checks for new data. dataStruct array holds data if there is
 *               new data, otherwise it is empty.
 *               usage: dataStructArray = infoclient('check')
 *
 *       acknlg: acknowledges reciept of data. 
 *               usage: success = infoclient('acknowledge', dataStruct)
 *
 *       stop:   stops the infoclient.
 *               usage: success = infoclient('stop')
 *
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-01
 **/

#include"infoclientLib.h"
static char *infoFieldNames[] = {"dataName", "roiName", "value", "tr", "time"};
static unsigned int infoNumFields = 5;


#include<sstream>

#include"mex.h"
#include"matrix.h"


/**
 * assign success status for return values
 *  plhs:    the pointer to the output argument
 *  success: 0 for success, else failure
 */
void assignSuccessStatus(int nlhs, mxArray *plhs[], int success) {
  if(nlhs > 0) {
    plhs[0] = mxCreateLogicalScalar(success != SUCCESS);
  }
}

/**
 * sets the fields of an info struct array element from an info object
 */
void setInfoFields(mxArray *arr, unsigned int ind, const Info &info) {
  mxSetField(arr,ind,"dataName",mxCreateString(info.dataName.c_str()));
  mxSetField(arr,ind,"roiName",mxCreateString(info.roiName.c_str()));
  mxSetField(arr,ind,"value",mxCreateDoubleScalar(info.value));
  mxSetField(arr,ind,"tr",mxCreateDoubleScalar(info.tr));
  mxSetField(arr,ind,"time",mxCreateDoubleScalar(info.time));
}

/**
 * creates a matlab struct array from a vector of info objects
 */
mxArray *createInfoStructArray(vector<Info> &infov) {
  mwSize dims[2];
  dims[0] = infov.size();
  dims[1] = infov.size() == 0 ? 0 : 1;

  // create array
  mxArray *arr = mxCreateStructArray(2, dims, 
			   infoNumFields, (const char**) infoFieldNames);
  if(arr == NULL) {
    mexErrMsgTxt("couldn't create struct array.");
    return NULL;
  }

  // fill array
  unsigned int ind = 0;
  for(vector<Info>::iterator i = infov.begin(); i != infov.end(); i++, ind++) {
    setInfoFields(arr, ind, (*i));
  }
  
  return arr;
}

/**
 * entry for mex
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  string command;
  char tmpChars[MAX_STR_LEN];

  // check parameters
  if(nrhs < 1) {
    mexErrMsgTxt("'command' is a required input argument.");
    assignSuccessStatus(nlhs,plhs,FAILURE);
    return;
  }

  if(nlhs > 1) {
    mexErrMsgTxt("no more than one output argument is supported.");
    assignSuccessStatus(nlhs,plhs,FAILURE);
    return;
  }

  if(mxGetString(prhs[0],tmpChars,MAX_STR_LEN)) {
    mexErrMsgTxt("'command' can't be converted to a string.");
    return;
  }
  command = tmpChars;

  // different behavior based on command
  int status;
  string errMsg;
  if(command == "start") {  // find host and port from args 
    TcpInfo info;

    // check number of parameters
    if(nrhs != 3) {
      mexErrMsgTxt("host and port are required arguments for 'start'.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }

    // get the host
    if(mxGetString(prhs[1],info.host,MAX_STR_LEN)) {
      mexErrMsgTxt("host name can't be converted to a string.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }

    // get the port
    if(!mxIsNumeric(prhs[2])) {
      mexErrMsgTxt("port can't be converted to a number.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }
    info.port = (int) mxGetScalar(prhs[2]);

    status = startInfoclient(info, errMsg);

    if(status == SUCCESS) {
      mexPrintf("started the infoclient.\n");
    }

    assignSuccessStatus(nlhs,plhs,status);
  }
  else if(command == "add") { // 

    // check number of parameters
    if(nrhs != 3) {
      mexErrMsgTxt("dataName and roiName are required arguments for 'add'.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }

    // get the dataName
    if(mxGetString(prhs[1],tmpChars,MAX_STR_LEN)) {
      mexErrMsgTxt("dataName can't be converted to a string.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }
    string dataName(tmpChars);

    // get the roiName
    if(mxGetString(prhs[2],tmpChars,MAX_STR_LEN)) {
      mexErrMsgTxt("roiName can't be converted to a string.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }
    string roiName(tmpChars);

    status = addInfoclient(dataName, roiName, errMsg);

    if(status == SUCCESS) {
      mexPrintf("listening for %s:%s on the infoclient.\n", 
		dataName.c_str(), roiName.c_str());
    }

    assignSuccessStatus(nlhs,plhs,status);
  }
  else if(command == "remove") { // 

    // check number of parameters
    if(nrhs != 3) {
      mexErrMsgTxt("dataName and roiName are required arguments for 'remove'.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }

    // get the dataName
    if(mxGetString(prhs[1],tmpChars,MAX_STR_LEN)) {
      mexErrMsgTxt("dataName can't be converted to a string.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }
    string dataName(tmpChars);

    // get the roiName
    if(mxGetString(prhs[2],tmpChars,MAX_STR_LEN)) {
      mexErrMsgTxt("roiName can't be converted to a string.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }
    string roiName(tmpChars);

    status = removeInfoclient(dataName, roiName, errMsg);

    if(status == SUCCESS) {
      mexPrintf("removed %s:%s from the infoclient.\n", 
		dataName.c_str(), roiName.c_str());
    }

    assignSuccessStatus(nlhs,plhs,status);
  }
  else if(command == "check") {

    // check parameters
    if(nlhs < 1) {
      mexErrMsgTxt("'check' requires one output argument.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }
    
    vector<Info> changed;
    status = checkInfoclient(changed,errMsg);

    // copy the vector to a struct array
    plhs[0] = createInfoStructArray(changed);

  }
  else if(command == "acknlg" || command == "acknowledge") {
    // check parameters
    if(nrhs != 2) {
      mexErrMsgTxt("dataStruct is a required argument for 'acknowledge'.");
      assignSuccessStatus(nlhs,plhs,FAILURE);
      return;
    }

    // convert the argument into an Info struct;
    Info info;

    status = acknowledgeInfoclient(info, errMsg);
  }
  else if(command == "stop") {
    status = stopInfoclient(errMsg);
    assignSuccessStatus(nlhs,plhs,status);
  }
  else {
    stringstream err;
    err << "command '" << command << "' unrecognized.";
    errMsg = err.str();
    assignSuccessStatus(nlhs,plhs,FAILURE);
  }
  
  if(status == FAILURE) {
    mexErrMsgTxt(errMsg.c_str());
  }

  return;
}

