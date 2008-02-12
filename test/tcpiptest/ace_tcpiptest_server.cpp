
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Acceptor.h"
#include"ace/SOCK_Stream.h"
#include<iostream>
#include<cstring>

using namespace std;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  // Local server address.
  ACE_INET_Addr my_addr(argc > 1 ? atoi(argv[1]) : 15000);
			  //	  argc > 2 ? argv[2] : "localhost");
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Acceptor acceptor;
  acceptor.open(my_addr,1);

  cout 
	 << my_addr.get_host_name() << ":" << my_addr.get_port_number() << endl;

  // keep receiving new connections 
  while(acceptor.accept(stream, &my_addr) != -1) {
    cout << "accepting msg from " 
	 << my_addr.get_host_name() << ":" << my_addr.get_port_number() << endl;

    char msg[1024];
    unsigned int recieved = 0;
    for(char last = 'a'; last != '\0'; recieved++) {
      stream.recv_n(&last, 1);      
      msg[recieved] = last;
      cout << last;
      cout.flush();
    }
    msg[recieved] = '\0';

    cout << "got " << msg << endl;


    strcpy(msg,"yer mom has a red mouse tit\n");
    stream.send_n(msg, strlen(msg));      

    sleep(1);

    stream.close();
  }
  acceptor.close();


  return 0;
}
