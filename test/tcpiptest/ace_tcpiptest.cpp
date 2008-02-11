
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<iostream>
#include<cstring>

using namespace std;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  // Local server address.
  ACE_INET_Addr my_addr ( argc > 1 ? atoi(argv[1]) : 15000,
			  argc > 2 ? argv[2] : "localhost");
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  if(connector.connect (stream, my_addr) == -1) {
    cout << "connection failed with errno " 
	 << errno << endl;
    return 0;
  }

  // send forever
  while(1) {
    cout << "sending msg" << endl;

    char *msg = "todd rox";
    stream.send_n (msg, strlen(msg)+1);

    usleep(1000000);
  }

  stream.close();

  return 0;
}
