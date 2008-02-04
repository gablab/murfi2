
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<iostream>
#include<cstring>

using namespace std;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  // Local server address.
  ACE_INET_Addr my_addr ( argc > 5 ? atoi(argv[5]) : 15000,
			  argc > 4 ? argv[4] : "localhost");
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  // keep making new connections 
  while(connector.connect (stream, my_addr) != -1) {

    cout << "made connection, sending msg" << endl;

    char *msg = "todd rox";
    stream.send_n (msg, strlen(msg)+1);

    usleep(1000000);

    stream.close();
  }

  cout << errno << endl;

  return 0;
}
