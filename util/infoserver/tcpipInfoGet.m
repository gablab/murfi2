function strOut = tcpipInfoGet(strParams,varargin)
%TCPIPINFOGET opens a socket to the real-time infoserver and requests
% information via xml string.

% java networking stuffs
import java.*;
import java.io.*;
import java.net.*;
import java.lang.*;

% very basic input checking
if ~isempty(varargin) && strcmpi(varargin{1},'test')
    isATest = true;
else
    isATest = false;
end

% check inputs and set default values
if isempty(strParams.TCP_HOST)
    % empty is actually a good default value, so do nothing
end

% if TCP_PORT field does not exist or is empty, set to default
if ~isfield(strParams,'TCP_PORT') || isempty(strParams.TCP_PORT)
    strParams.TCP_PORT = 15001;
end

% if dataIDString field does not exist or is empty, set to design-matrix
if ~isfield(strParams,'dataIDString') || isempty(strParams.dataIDString)
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
end

% if requestString field does not exist or is empty, set to retrieve
% just the matrix portion of designmatrix
if ~isfield(strParams,'requestString') || isempty(strParams.requestString)
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',':*:*:*:*:*:*:design-matrix::','">']...
        '<designmatrix>',...
        '<matrix>',...
        '</matrix>',...
        '</designmatrix>',...
        '</get>' ...
        '</info>' ...
        ]);
end

% msg termination string
term = String(sprintf('\n'));

host = strParams.TCP_HOST;
portnum = strParams.TCP_PORT;

if isATest
    % call dummy infoserver
    strOut = rtDummyInfoServer(strParams);
else
    % open the socket
    try
        skt = Socket(host,portnum);
        in = BufferedReader(InputStreamReader(skt.getInputStream()));
        out = PrintWriter(skt.getOutputStream(),true);
        
        % send a request
        sendstr = String(strParams.requestString);
        out.println(sendstr);
        
        %     % old receive the response
        %     strOut = [];
        %     tmp = in.read();
        %     while(~strcmp(tmp,term) && tmp ~= -1)
        %         i = i + 1;
        %         strOut(end+1) = char(tmp);
        %         tmp = in.read();
        %         i
        %     end
        
        % new receive the response
        strOut = in.readLine();
        
        in.close();
        out.close();
        skt.close();
    catch ME
        response.error = ME;
        rethrow(ME)
    end 
    strOut = char(strOut);
end