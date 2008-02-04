
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"
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
  ACE_SOCK_Acceptor acceptor;
  acceptor.open(my_addr,1);

  // keep receiving new connections 
  while(acceptor.accept(stream, &my_addr) != -1) {

    cout << "made connection, accepting msg from " 
	 << my_addr.get_host_name() << ":" << my_addr.get_port_number() << endl;

    char msg[1024];
    stream.recv_n(msg, 1024);

    cout << "got " << msg << endl;

    stream.close();
  }

  acceptor.close();

  return 0;
}
