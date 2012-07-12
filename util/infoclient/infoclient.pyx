__author__ = 'satra'
from cython.operator cimport dereference as deref, preincrement as inc

from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "infoclientLib.h":
    cdef struct Info:
        string dataName
        string roiName
        double value
        unsigned int tr
        unsigned long time
        bint changed

    cdef struct TcpInfo:
        string host
        int port

    ctypedef TcpInfo & const_tcpinfo_ref "const TcpInfo &"
    ctypedef string & const_string_ref "const string &"
    bint startInfoclient(const_tcpinfo_ref, const_tcpinfo_ref, string&)
    bint stopInfoclient(string&)
    bint addInfoclient(const_string_ref, const_string_ref, string &)
    bint removeInfoclient(const_string_ref, const_string_ref, string &)
    bint sendMessage(const_string_ref, const_tcpinfo_ref, string &)
    bint checkInfoclient(vector[Info] &, string &)

infoFieldNames = ["dataName", "roiName", "value", "tr", "time"]
infoNumFields = len(infoFieldNames)

def setInfoFields(info):
    """Convert info to a dictionary"""
    return {'dataName': info.dataName.c_str(),
            'roiName': info.roiName.c_str(),
            'value': float(info.value),
            'tr': float(info.tr),
            'time': float(info.time)}

class InfoClient(object):
    def start(self, localport, remotehost, remoteport):
        cdef TcpInfo info
        cdef TcpInfo remoteInfo
        cdef string errMsg
        cdef bytes py_remotehost = remotehost

        info.port = localport
        remoteInfo.host = string(py_remotehost)
        remoteInfo.port = remoteport
        return startInfoclient(info, remoteInfo, errMsg)

    def stop(self):
        cdef string errMsg
        return stopInfoclient(errMsg)

    def add(self, dataName, roiName):
        cdef string errMsg
        cdef bytes py_dataName = dataName
        cdef bytes py_roiName = roiName
        return addInfoclient(string(py_dataName), string(py_roiName), errMsg)

    def remove(self, dataName, roiName):
        cdef string errMsg
        cdef bytes py_dataName = dataName
        cdef bytes py_roiName = roiName
        return removeInfoclient(string(py_dataName), string(py_roiName), errMsg)

    def send(self, message, remotehost, remoteport):
        cdef TcpInfo remoteInfo
        cdef string errMsg
        cdef bytes py_remotehost = remotehost
        cdef bytes py_message = message

        remoteInfo.host = string(py_remotehost)
        remoteInfo.port = remoteport
        return sendMessage(string(py_message), remoteInfo, errMsg)

    def check(self):
      cdef vector[Info] changed
      cdef string errMsg
      cdef vector[Info].iterator it
      status = checkInfoclient(changed, errMsg)
      print status
      if not status:
          print changed.size()
          it = changed.begin()
          while it != changed.end():
              print deref(it).dataName.c_str()
              inc(it)
      else:
          raise Exception(errMsg.c_str())

'''

/**
 * check for new data on the infoclient data listener
 */
int checkInfoclient(vector<Info> &changed, string &errMsg);

/**
 * acknowledge receipt of data on the infoclient data listener
 */
int acknowledgeInfoclient(const Info &info, string &errMsg);

"""
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
"""
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
'''