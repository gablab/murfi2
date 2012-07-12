/**
 * infoclientLib provides an interface to murfi's infoclient
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

#include"ace/Date_Time.h"
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"
#include"ace/SOCK_Connector.h"

#include"tinyxml/tinyxml.h"


// file static vars

// net
static bool serverRunning = false;
static ACE_SOCK_Acceptor acceptor;
static long WAIT_USEC = 10000;
static ACE_INET_Addr remoteAddress;
static const char messageTerminationChar = '\n';

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
 * convert an ace date/time struct to a long
 */
long timeToLong(const ACE_Date_Time &time) {
  long ret = time.microsec();
  int power = 0;

  power += 6;
  ret += pow(10,power) * time.second();

  power += 2;
  ret += pow(10,power) * time.minute();

  power += 2;
  ret += pow(10,power) * time.hour();

  power += 2;
  ret += pow(10,power) * time.day();

  power += 2;
  ret += pow(10,power) * time.month();
}

/**
 * open a listener for data
 */
int openDataListener(unsigned short port, string &errMsg) {

  ACE_INET_Addr address(port);
  errMsg = "";

  if(acceptor.open(address,1) == -1) {
    stringstream err;
    err << "couldn't open acceptor to listen on port "
        << address.get_port_number() << endl;
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
  for(TiXmlElement *data = 0;
      (data = (TiXmlElement*) doc.IterateChildren("data", data)); ) {
    const char *dnAttr = data->Attribute("name");
    const char *rnAttr = data->Attribute("roi");

    if(dnAttr == NULL || rnAttr == NULL) {
      cerr << "name or roi attributes are null!" << endl;
      continue;
    }

    Info info;
    info.dataName = dnAttr;
    info.roiName = rnAttr;

    cout << info.dataName << " " << info.roiName << endl;

    // find this data in the database
    set<Info>::iterator i = dataBase.find(info);
    if(i != dataBase.end()) { // if found, update the info in the set
      ACE_Date_Time time;

      const char *trAttr = data->Attribute("tr");
      const char *val = data->FirstChild()->Value();

      if(trAttr == NULL || val == NULL) {
        cerr << "tr attribute or value are null!" << endl;
        continue;
      }

      info.tr =  atoi(trAttr);
      info.time = timeToLong(time);
      info.value = atof(val);
      info.changed = true;

      // add it to the changed queue
      changedData.push(info);
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
  ACE_INET_Addr connected;
  ACE_Time_Value timeout(0,10000);

  TcpInfo *info = (TcpInfo*) tcpInfo;

  if(openDataListener(info->port, errMsg) == FAILURE) {
    cout << errMsg << endl;
    return NULL;
  }

  // continuously try to accept connections
  while(serverRunning) {
    if(acceptor.accept(stream, &connected, &timeout, 0) != -1) {
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
      string received = message.str();
      handleReceivedXml(received);

      stream.close();
    }
  }

  threadRunning = false;
  pthread_join(thread,NULL);
  thread = NULL;
}

/**
 * send an add message to the remote client
 */
int sendAddMessage(const Info &info) {
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connector;

  if(!connector.connect(stream, remoteAddress)) {
    // build add message
    stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        << "<info>"
        << "<add name=\"" << info.dataName
        << "\" roi=\"" << info.roiName
        << "\">"
        << "</add>"
        << "</info>";

    unsigned int sent = stream.send_n(xml.str().c_str(), xml.str().length());
    if(sent < xml.str().length()) {
      cerr << "incomplete send" << endl;
      return FAILURE;
    }

    stream.send_n(&messageTerminationChar,1);
    stream.close();

    //cout << "sent on " << remoteAddress.get_host_name() << ":" << remoteAddress.get_port_number() << " " << xml.str() << endl;

  }
  else {
    return FAILURE;
  }

  return SUCCESS;
}

/**
 * send an remove message to the remote client
 */
int sendRemoveMessage(const Info &info) {
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connector;

  if(!connector.connect(stream, remoteAddress)) {
    // build remove message
    stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        << "<info>"
        << "<remove name=\"" << info.dataName
        << "\" roi=\"" << info.roiName
        << "\">"
        << "</remove>"
        << "</info>";

    unsigned int sent = stream.send_n(xml.str().c_str(), xml.str().length());
    if(sent < xml.str().length()) {
      cerr << "incomplete send" << endl;
      return FAILURE;
    }

    stream.send_n(&messageTerminationChar,1);
    stream.close();

    //cout << "sent " << xml.str() << endl;

  }
  else {
    return FAILURE;
  }

  return SUCCESS;
}

/**
 * start the infoclient
 */
int startInfoclient(const TcpInfo &tcpInfo,
                    const TcpInfo &remoteTcpInfo,
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
    // wait a little while for the tcp stuff to happen
    usleep(WAIT_USEC);
    threadRunning = true;
  }

  remoteAddress.set(remoteTcpInfo.port, remoteTcpInfo.host.c_str());

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

  // add template to database
  Info info;
  info.dataName = dataName;
  info.roiName  = roiName;
  pair<set<Info,InfoCompare>::iterator,bool> i = dataBase.insert(info);

  // send add message
  sendAddMessage(info);

  //  cout << "inserted " << (*i.first).dataName << ":" << (*i.first).roiName << " "
  //       << i.second << endl;
  //

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

  sendRemoveMessage(info);

  return SUCCESS;
}

/**
 * check for new data on the infoclient data listener
 */
int checkInfoclient(vector<Info> &changed, string &errMsg) {
  changed.clear();

  cout << changedData.size() << endl;
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

/**
 * send a message to a remote client
 */
int sendMessage(const string &message, const TcpInfo &info, string &errMsg) {
  ACE_INET_Addr addr(info.port, info.host.c_str());
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connector;

  errMsg = "";
  if(!connector.connect(stream, addr)) {
    unsigned int sent = stream.send_n(message.c_str(), message.length());
    if(sent < message.length()) {
      errMsg = "incomplete send";
      return FAILURE;
    }

    stream.send_n(&messageTerminationChar,1);
    stream.close();

    //cout << "sent on " << info.host << ":" << info.port << " " << message << endl;

  }
  else {
    return FAILURE;
  }

  return SUCCESS;
}
