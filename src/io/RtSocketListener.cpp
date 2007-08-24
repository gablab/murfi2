/******************************************************************************
 * RtSocketListener.cpp defines a class for input from a tcp socket.
 * Uses the GNU CommonC++ library for sockets.
 * This implementation is based on SampleSocketPort.cpp, available at
 * http://www.gnu.org/software/commoncpp/docs/refman/html/examples.html
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/
static char *VERSION = "$Id$";

#include "RtSocketListener.h"

// construct
RtSocketListener::RtSocketListener(SocketService *ss, TCPSocket &ts) :
                  SocketPort(ss, ts) {

  tpport_t port;
  InetHostAddress ia = getPeer(&port);
  cerr << "connecting from " << ia.getHostname() << ":" << port << endl;

  // set up non-blocking reads
  setCompletion(false);

  // init members
  open = true;
  disconnecting = false;
  timedOut = false;
  rxStarted = false;
  lastNumAvail = 0;
  buffer = new char[MAX_RXBUF];
}

// destruct
RtSocketListener::~RtSocketListener() {
  endSocket();
  delete [] buffer;
}

void RtSocketListener::pending(void) {
  cerr << "pending called " << endl;

  if(!open) {
    return;
  }

  // read all available bytes
  int numAvail = peek(buffer, MAX_RXBUF);

  cerr << "pending sees " << numAvail << endl;

  if(!rxStarted) {
    // start receive timer, timeout after n secs
    setReadTimer(MAX_RXTIMEOUT);
    rxStarted = true;
  }
  else {
    // test if receive timer expired
    if(timedOut) { // handle it
      setReadTimer(MAX_RXTIMEOUT);
      lastNumAvail = 0;
      rxStarted = false;
      onRxTimeout();
      return;
    }
  }

  // check if any more data has been received while reading
  if(lastNumAvail == numAvail)  {
    //Maybe yield in here!
    //Thread::yield();
    if(numAvail == 0) {
      // if we have been called with 0 bytes available (twice now)
      // a disconnection has occurred
      if(!disconnecting) {
	close();  //Force the close
      }
    }

    return;
  }

  // attempt to process the extra data
  if(numAvail > MAX_RXBUF) {
    cerr << "ERROR: received data greater than max buffer size. increase MAX_RXBUFFER" << endl;

    flushRxData();
    lastNumAvail = 0;
    rxStarted = false;
    return;
  }
  lastNumAvail = numAvail;

  dataReceived(buffer, numAvail);
}

void RtSocketListener::disconnect() {
  if(open) {
    disconnecting = true;
    close();
  }
}

void RtSocketListener::expired() {
  if(disconnecting && open) {
    close();
  }
  else if(open && rxStarted) {
    //Timer must have expired because the rx data has not all been received
    timedOut = true;
  }
}


bool RtSocketListener::close() {
  // where the disconnection really occurs
  if(open && disconnecting) {
    open = false;                                
    onClose();
    delete this;
  }
  else if(open) {
    disconnecting = true;       // set the timer and the flag so we can
    setTimer(DISCONNECT_MS);   // disconnect safely, in DISCONNECT_MS
  }
  return true;
}


// time the read
void RtSocketListener::setReadTimer(timeout_t timeout) {
  timedOut = false;
  setTimer(timeout);
}

// flush on error
void RtSocketListener::flushRxData() {
  while(receive(buffer, MAX_RXBUF) > 0);
}


// do stuff on read timeout
void RtSocketListener::onRxTimeout() {
  flushRxData();
}


// gets the version
//  out:
//   cvs version string for this class
char *RtSocketListener::getVersionString() {
  return VERSION;
}

//#define WITH_EXAMPLE

#ifdef WITH_EXAMPLE


/************ THE FOLLOWING CODE DEMONSTRATES THE USE OF THE ABOVE CLASS ********************
 ****
 ****   To test it, compile with:
 ****
 ****   g++ SampleSocketPort.cpp -lccgnu -lpthread -ldl -oSampleSocketPort -ggdb -I/usr/local/include/cc++/
 ****   Run the program.
 ****
 ****   From another terminal telnet to port 3999 of the server
 ****
 ****           'telnet localhost 3999'
 ****
 ****   Anything you type should be sent back to you in reverse!
 ****
 ****   To test the corrupt data detection, send a control code (like ^D),
 ****   if the terminating charcters are not detected within the specified time
 ****   the receive timeout will occur.
 ****
 ****/


//define the following to include the example classes and functions

int g_nOpenPorts = 0;                   //Dirty global to allow us to quit simply

class ReverserPort : public RtSocketListener
{
public:
  ReverserPort(SocketService *pService, TCPSocket & tcpSocket) :
    RtSocketListener(pService, tcpSocket)
  {
    g_nOpenPorts++;
  }
  virtual ~ReverserPort()
  {
    g_nOpenPorts--;
  }
  virtual void OnConnectionClosed(void)
  { cerr << "Connection Closed!" << endl; }

  virtual void OnDataReceived(char *pszData, unsigned int nByteCount)
  {
    //Reverse the data and send it back

    int nLen = strlen(pszData);
    char *szToSend = new char[nLen+1];

    //No need to reverse the \r\n or \0
    int nIndex = nLen-3;

    int i;
    for(i=0; i < nLen - 2; i++)
      {
	szToSend[i] = pszData[nIndex - i];
      }
    szToSend[i++] = '\r';
    szToSend[i++] = '\n';
    szToSend[nLen] = '\0';

    WriteData(szToSend, nLen);
    delete [] szToSend;
  }

};

class ReverserServer : public SampleSocketServiceServer
{
public:
  ReverserServer(InetHostAddress & machine, int port) :
    SampleSocketServiceServer(machine, port), TCPSocket(machine, port), Thread()
  {
  }
  virtual ~ReverserServer()
  {
  }
  virtual SocketPort *CreateSocketPort(SocketService *pService, TCPSocket & Socket)
  {
    return(new ReverserPort(pService, Socket));
  }
};


int main(void)
{
  InetHostAddress LocalHost;
  LocalHost = htonl(INADDR_ANY);
  ReverserServer *Server = NULL;
  try
    {
      Server = new ReverserServer(LocalHost, 3999);
      Server->StartServer();
    }
  catch(...)
    {
      cerr << "Failed to start server" << endl;
      return(false);
    }
  cerr << "Waiting for connections...type \"quit\" to exit." << endl;

  char cmd[255];

  cin.getline(cmd, 255);


  while(strcmp(cmd, "quit") != 0)
    {
      cin.getline(cmd, 255);
    }

  Server->StopServer();
  delete Server;
  return 0;
}

#endif  //WITH_EXAMPLE


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


