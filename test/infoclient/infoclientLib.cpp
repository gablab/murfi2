/**
 * infoclientLib provides an interface to festr's infoserver
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-01
 **/

#include"infoclientLib.h"

#ifdef WIN32
  #include<windows.h> /* for threading */
#else
  #include<pthread.h> /* for threading */
  #include<unistd.h> /* for sleep */
#endif

#include<iostream>
#include<sstream>
#include<set>
#include<queue>

using namespace std;

#ifdef WIN32
  #include<windows.h> /* for threading */
#else
  #include<pthread.h> /* for threading */
  #include<unistd.h> /* for sleep */
#endif

#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"

#include"tinyxml/tinyxml.h"


// file static vars

// net
static bool serverRunning = false;
static ACE_SOCK_Acceptor acceptor;

// thread
static bool threadRunning = false;
#ifdef WIN32
  
#else
  static pthread_t thread;
#endif

// data
static set<Info,InfoCompare> dataBase; 

// changed data
static queue<Info> changedData;


// functions

/**
 * open a listener for data
 */
int openDataListener(char *host, unsigned short port, string &errMsg) {

  ACE_INET_Addr address(port, host);
  errMsg = "";

  if(acceptor.open(address,1) == -1) {
    stringstream err;
    err << "couldn't open acceptor to listen on " << address.get_host_name() 
	<< ":" << address.get_port_number() << endl;
    errMsg = err.str();
    return FAILURE;
  }

  serverRunning = true;

  return 0;
}

/**
 * handle an xml string received from the infoserver
 */
void handleReceivedXml(const string &xml) {
  // parse the xml
  TiXmlDocument doc;
  doc.Parse(xml.c_str());
  if(doc.Error()) {
    cerr << "ERROR (infoclientLib): could not parse XML" << endl;
    return;
  }  

  // search for info tags
  for(TiXmlNode *info = 0; (info = doc.IterateChildren("info", info)); ) {
    // iterate over children looking for added data
    for(TiXmlElement *data = 0; 
	data = (TiXmlElement*) info->IterateChildren("data", data); ) {

      Info info;
      info.dataName = data->Attribute("name");
      info.roiName =  data->Attribute("roi");
      
      //cout << "searching for " << info.dataName << ":" << info.roiName << endl;

      // find this data in the database
      set<Info>::iterator i = dataBase.find(info);
      if(i != dataBase.end()) { // if found, update the info in the set
	info.tr =  atoi(data->Attribute("tr"));
	info.time= time(NULL);
	info.value = atof(data->FirstChild()->Value());
	info.changed = true;

//	cout << "updating: " 
//	     << info.dataName << " "
//	     << info.roiName << " "
//	     << info.tr << " "
//	     << info.time << " "
//	     << info.value << " "
//	     << endl;

	// add it to the changed queue
	changedData.push(info);

      }
    }
  }
  
}

/**
 * close a listener for data
 */
int closeDataListener() {
  acceptor.close();
  serverRunning = false;

  // remove all data from database and changed queue
  while(!changedData.empty()) changedData.pop(); 
  dataBase.clear();

  return 0;
}

/** 
 * blocking listen for data. used as thread entry
 */
void *listenForData(void *tcpInfo) {
  string errMsg;
  ACE_SOCK_Stream stream;
  ACE_INET_Addr remoteAddress;
  ACE_Time_Value timeout(0,10000);

  TcpInfo *info = (TcpInfo*) tcpInfo;

  if(openDataListener(info->host, info->port, errMsg) == FAILURE) {
    cout << errMsg << endl;
    return NULL;
  }

  // continuously try to accept connections
  while(serverRunning) {
    if(acceptor.accept(stream, &remoteAddress, &timeout, 0) != -1) {
      stringstream message;

      // get characters until null character
      char last;
      do {
	if(0 == stream.recv_n(&last,1)) {
	  break;
	}

	message << last;
      } while(last != '\n');

      // receive the message, store the response
      string recieved = message.str();
      cout << "received: " << recieved << endl;

      handleReceivedXml(recieved);

      stream.close();
    }
  }

  threadRunning = false;
  pthread_join(thread,NULL);
  thread = NULL;
}

/**
 * start the infoclient 
 */
int startInfoclient(const TcpInfo &tcpInfo, 
		    string &errMsg) {
  errMsg = "";

  // check if we have already started 
  if(threadRunning || serverRunning) {
    errMsg = "refusing to start the infoclient again."; 
    return FAILURE;
  }

  // start the thread
  if(pthread_create(&thread, NULL, listenForData, (void*) &tcpInfo)) {
    errMsg = "thread creation failed.";
    return FAILURE;
  }
  else {
    threadRunning = true;
  }

  return SUCCESS;
}

/**
 * add data to the infoclient data listener 
 */
int addInfoclient(const string &dataName, const string &roiName, 
		  string &errMsg) {
  if(!threadRunning || !serverRunning) {
    errMsg = "must start the infoclient before adding or removing data."; 
    return FAILURE;
  }

  // send add message
  
  // add template to database
  Info info;
  info.dataName = dataName;
  info.roiName  = roiName;
  pair<set<Info,InfoCompare>::iterator,bool> i = dataBase.insert(info);

  cout << "inserted " << (*i.first).dataName << ":" << (*i.first).roiName << " " 
       << i.second << endl;


  return SUCCESS;
}

/**
 * remove data from the infoclient data listener 
 */
int removeInfoclient(const string &dataName, const string &roiName, 
		     string &errMsg) {
  if(!threadRunning || !serverRunning) {
    errMsg = "must start the infoclient before adding or removing data."; 
    return FAILURE;
  }

  // send remove message
  
  // add template to database
  Info info;
  info.dataName = dataName;
  info.roiName  = roiName;
  set<Info>::iterator i = dataBase.find(info);
  if(i == dataBase.end()) {
    errMsg = "data not found in the infoclient database"; 
    return FAILURE;    
  }

  dataBase.erase(i);

  return SUCCESS;
}

/**
 * check for new data on the infoclient data listener 
 */
int checkInfoclient(vector<Info> &changed, string &errMsg) {
  changed.clear();

  // empty changed queue
  while(!changedData.empty()) {
    changed.push_back(changedData.front());
    changedData.pop();
  }

  return SUCCESS;
}

/**
 * acknowledge receipt of data on the infoclient data listener 
 */
int acknowledgeInfoclient(const Info &info, string &errMsg) {
  if(!threadRunning || !serverRunning) {
    errMsg = "infoclient not running, can't acknowledge anything."; 
    return FAILURE;
  }

  errMsg = "not implememented yet :("; 
  return FAILURE;

  // send a message to the backend

  return SUCCESS;
}


/**
 * stop the infoclient
 */
int stopInfoclient(string &errMsg) {
  if(!threadRunning) {
    errMsg = "refusing to stop an infoclient that isn't running.";
    return FAILURE;
  }
    
  closeDataListener();
  return SUCCESS;
}
