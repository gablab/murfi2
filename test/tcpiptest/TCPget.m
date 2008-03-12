function [strOut] =  TCPget(hostname,port,msg)

  if(nargin < 3)
    msg = '<?xml version="1.0" encoding="UTF-8"?><info><roi id="roi_id"><get name="activation" tr="last"></get></roi></info>';
  end
    
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
xmlstr = String(msg);

out.println(xmlstr);

strOut = [];
tmp = in.read();
while(~strcmp(tmp,term) && tmp ~= -1)
  strOut(end+1) = char(tmp);
  tmp = in.read();
end
disp(['Retrieved from server socket: ' strOut]);

in.close();
out.close();
skt.close();
