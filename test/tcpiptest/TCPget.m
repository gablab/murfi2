function [strOut] =  TCPget(hostname,port)

import java.io.*;
import java.net.*;
import java.lang.*;

host = String(hostname);
%port = 15003;

skt =  Socket(host,port);
in =  BufferedReader(InputStreamReader(skt.getInputStream()));
tmp = in.readLine();
strOut = char(tmp);
disp(['Retrieved from server socket: ' strOut]);

in.close();
skt.close();
