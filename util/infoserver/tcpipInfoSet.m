function tcpipInfoSet(strParams)
%TCPIPINFOSET connects to the real-time infoserver and sets information via
% xml string.

% java networking stuffs
import java.*;
import java.io.*;
import java.net.*;
import java.lang.*;

% msg termination string
term = String(sprintf('\n'));

host = strParams.TCP_HOST;
portnum = strParams.TCP_PORT;

% open the socket
try
    skt =  Socket(host,portnum);
    in =   BufferedReader(InputStreamReader(skt.getInputStream()));
    out =  PrintWriter(skt.getOutputStream(),true);
    
    % send a set string
    sendstr = String(strParams.requestString);
    out.println(sendstr);
    repsonse = in.readLine(); % listen for any response from the infoserver
    in.close();
    out.close();
    skt.close();
catch ME
    rethrow(ME)
end