/**
 * infoclientLib provides an interface to murfi's infoserver
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-01
 **/

#ifndef INFOCLIENT_LIB_H
#define INFOCLIENT_LIB_H

#include<string>
#include<iostream>
#include<vector>

using namespace std;

// defines
const int SUCCESS = 0;
const int FAILURE = 1;
const int MAX_STR_LEN = 1024;

// types

// store info about data
struct Info {
  string dataName;
  string roiName;

  // things we will change within the set must be pointers to allow
  // changing them within the set. annoying.
  double value;
  unsigned int tr;
  unsigned long time;
  bool changed;
  
  // comparison operator
  bool operator==(const Info &other) const {

    return
      dataName == other.dataName
      && 
      roiName  == other.roiName;
  }  

  // less than operator
  bool operator<(const Info &other) const {

    if(dataName != other.dataName) {
      return (dataName < other.dataName);
    }
    if(roiName != other.roiName) {
      return (roiName < other.roiName);
    }

    return false;
  }
};

// comparison class for storing ids in a set
struct InfoCompare {
  bool operator()(const Info &one, const Info &two) const {
    return (one) < (two);
  }
};

struct TcpInfo {
  string host;
  int port;
};

// functions

/**
 * start the infoclient 
 */
int startInfoclient(const TcpInfo &tcpInfo, 
		    const TcpInfo &remoteTcpInfo, 
		    string &errMsg);

/**
 * add data to the infoclient data listener 
 */
int addInfoclient(const string &dataName, const string &roiName, 
		  string &errMsg);

/**
 * remove  data from  the infoclient data listener 
 */
int removeInfoclient(const string &dataName, const string &roiName, 
		     string &errMsg);

/**
 * check for new data on the infoclient data listener 
 */
int checkInfoclient(vector<Info> &changed, string &errMsg);

/**
 * acknowledge receipt of data on the infoclient data listener 
 */
int acknowledgeInfoclient(const Info &info, string &errMsg);

/**
 * stop the infoclient
 */
int stopInfoclient(string &errMsg);

/**
 * send a message to a remote client
 */
int sendMessage(const string &message, const TcpInfo &info, string &errMsg);

#endif
