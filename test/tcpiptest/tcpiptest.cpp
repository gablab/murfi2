#if defined _MSC_VER
  #include <WINSOCK2.h>
typedef unsigned long       uint32_t;  ///< Unsigned integer having exactly 32 bits
#define close(__fd) closesocket(__fd)
#define socklen_t int
#else //defined _MSC_VER
  #include <unistd.h>
  #include <stdlib.h>
  #include <sys/socket.h>
  #include <sys/select.h>
  #include <netdb.h>
  #include<sys/timeb.h>
  #include <string.h>
#endif //defined _MSC_VER



#define	NULL_TYPE	 0		//for tickle
#define	PING_TYPE	10		//ping message
#define	SEND_TYPE	11		//send message

#include <stdio.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[]) {
  unsigned int port;
  char adr[20];

#if defined(_MSC_VER)
  WORD wVersionRequested;
  WSADATA wsaData;
  wVersionRequested = MAKEWORD( 2, 2 );
  WSAStartup( wVersionRequested, &wsaData );
#endif // defined(_MSC_VER)

   if(argc < 2) {
      port = 15001;
   }
   else {
      port = atoi(argv[1]);
   }

   if(argc < 3) {
      strcpy(adr, "localhost");
   }
   else {
      strcpy(adr, argv[2]);
   }

   printf("starting client: host %s:%d\n", adr, port);
   int connector = socket(AF_INET, SOCK_STREAM, 0);
   if(connector == -1) {
     printf("socket() failed\n");
     return -1;
   }

   struct hostent *phe = gethostbyname(adr);
   if(phe == 0) {
     close(connector);
     printf("gethostadr(%s) failed\n", adr);
     return -1;
   }

   sockaddr_in remoteAddr;
   memset(&remoteAddr, 0, sizeof(remoteAddr));
   remoteAddr.sin_addr.s_addr = *reinterpret_cast<unsigned int *>(phe->h_addr_list[0]);
   remoteAddr.sin_family = PF_INET;
   remoteAddr.sin_port = htons(port);


   char *msg = "todd sux\n";
   char term = '\0';

   int sent;

   while(connect(connector, reinterpret_cast <sockaddr *>(&remoteAddr), sizeof remoteAddr) != -1) {
     sent = send(connector, msg, strlen(msg)*sizeof(char), 0);
     send(connector, &term, sizeof(char), 0);
     printf("sent %d bytes for msg\n",sent);

#if defined _MSC_VER
     Sleep(1000);
#else
     sleep(1);
#endif

   }
     
     close(connector);

   return 0;
}
