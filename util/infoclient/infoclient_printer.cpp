/**
 * test program that connects to murfi's infoclient, then prints a message every
 * time data is received from murfi.
 * 
 * compile with: make 0f makefile.printer
 */

#include<cstdlib>
#include<cstring>
#include<iostream>
#include<string>

#include "infoclientLib.h"

using namespace std;

int main(int argc, char** argv) {

  if(argc != 6) {
    cerr << "usage: " << argv[0] << " local_port remote_host remote_port " 
         << " data_name roi_name" << endl;
    return 1;
  }

  int status = int();
  string errMsg;
  
  // start infoclient
  TcpInfo info;
  TcpInfo remoteInfo;
  
  info.port = (int) atoi(argv[1]);
  strcpy(remoteInfo.host, argv[2]);
  remoteInfo.port = (int) atoi(argv[3]);

  status = startInfoclient(info, remoteInfo, errMsg);

  if(status == SUCCESS) {
    cout << "infoclient listener started on " << info.host << ":" << info.port
         << ", listening for " << remoteInfo.host << ":" <<  remoteInfo.port
         << endl;
  }
  else {
    cerr << "error starting infoclient listener on " << info.host << ":"
         << info.port << ", listening for " << remoteInfo.host << ":"
         << remoteInfo.port << endl;
    cerr << errMsg << endl;
    return 1;
  }

  // add the data
  string dataName(argv[4]);
  string roiName(argv[5]);
  status = addInfoclient(dataName, roiName, errMsg);

  if(status == SUCCESS) {
    cout << "listening for " << dataName << ":" << roiName << endl;
  }
  else {
    cerr << "error adding data for listening" << endl;
    cerr << errMsg << endl;
    return 1;
  }
  
  // check in a loop  
  while(true) {
    vector<Info> changed;
    status = checkInfoclient(changed, errMsg);

    if(status != SUCCESS) {
      cerr << "error checking infoclient data" << endl;
      cerr << errMsg << endl;
      return 1;
    }

    for(vector<Info>::iterator it = changed.begin(); it != changed.end(); ++it) {
      cout << "received new data from the infoclient:" << endl;
      cout << it->dataName << ":" << it->roiName << " "
           << it->tr << ":" << it->value << endl;
    }
  }

  return 0;
}

    

  
