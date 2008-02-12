function [strOut] =  TCPget(hostname,port,msg)

import java.*;
import java.io.*;
import java.net.*;
import java.lang.*;

% msg termination string
term = String(sprintf('\n'));

host = String(hostname);

skt =  Socket(host,port);
in =  BufferedReader(InputStreamReader(skt.getInputStream()));
out =  PrintWriter(skt.getOutputStream(),true);

% send a request
xmlstr = String('<?xml version="1.0" encoding="UTF-8"?><info><roi id="roi_id"><get name="activation" trStart="120" trEnd="140"></get></roi></info>\n');

out.println(xmlstr);

strOut = [];
tmp = 'a';
while(~strcmp(tmp,term) && tmp ~= -1)
  tmp = in.read();
  strOut(end+1) = char(tmp);
end
disp(['Retrieved from server socket: ' strOut]);

in.close();
out.close();
skt.close();
