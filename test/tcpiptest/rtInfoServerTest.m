function rtInfoServerTest
%RTINFOSERVER runs connection, setting, and receiving tests for the
% RtInfoServer in festr.

%% start test
fprintf('Starting RtInfoServer test.\n');

%% set up initial params
% NOTE: For testing on the same machine, null (i.e. empty quotes) will
% work. For testing on two machines, the commented out host should be used.
strParams.TCP_HOST = ''; %'192.168.2.5';
strParams.TCP_PORT = 15001;

% set data id string
strParams.dataIDString = ':35115::::::design-matrix::';

%% test design matrix retrieval

fprintf('Starting design matrix retrieval test\n');

% create request to get design matrix
strParams.requestString = sprintf([...
    '<?xml version="1.0" encoding="UTF-8"?>' ...
    '<info>' ...
    ['<get dataid="',strParams.dataIDString,'">']...
    '</get>' ...
    '</info>' ...
    ]);

% get design matrix from infoserver
latest = getRTUpdate(strParams);

% basically check that it came back
a = numel(latest.designmatrix.isofinterest);
b = size(latest.designmatrix.matrix,2);
c = numel(latest.designmatrix.columnnames);
if a~=b
    error('Dimension discrepancy between isofinterest and columns in design matrix');
end

if a~=c
    error('Dimension discrepancy between isofinterest and column names vector');
end

if b~=c
    error('Dimension discrepancy between columns in design matrix and column names vector');
end

fprintf('Finished design matrix retrieval test\n');

%% test change to condition vector
fprintf('Starting condition vector change test\n');

% make a change to the condition vector based on latest design matrix
newIndex  = numel(latest.designmatrix.columnnames); % get last column
newName = ['modified_' latest.designmatrix.columnnames{newIndex}]; % modify column name
newIsOfInterest = ~latest.designmatrix.isofinterest(newIndex); % flip interest bool

% create set xml request string and send it to infoserver
strParams.requestString = ...
    sprintf([...
    '<?xml version="1.0" encoding="UTF-8"?>' ...
    '<info>'...
    ['<set dataid="',strParams.dataIDString,'">']...
    '<designmatrix>'...
    '<columninfo>'...
    ['<index>',num2str(newIndex)-1,'</index>']... % take zero-based indexing into account
    ['<name>',newName,'</name>']...
    ['<isofinterest>',num2str(newIsOfInterest),'</isofinterest>']...
    '</columninfo>'...
    '</designmatrix>'...
    '</set>'...
    '</info>']);
tcpipInfoSet(strParams);

% wait a bit then get design matrix again
pause(2)

% create request to get design matrix
strParams.requestString = sprintf([...
    '<?xml version="1.0" encoding="UTF-8"?>' ...
    '<info>' ...
    ['<get dataid="',strParams.dataIDString,'">']...
    '</get>' ...
    '</info>' ...
    ]);
latest = getRTUpdate(strParams);

% verify changes
if ~strcmp(latest.designmatrix.columnnames{newIndex},newName)
    error('Name change was not propagated')
end
if latest.designmatrix.isofinterest(newIndex) ~= newIsOfInterest
    error('isofinterest change was not propagated')
end

fprintf('Finished condition vector change test\n');

%% test change to single cell
fprintf('Finished single element change test\n');

% make a change to the design matrix.
row = floor(size(latest.designmatrix.matrix,1)/2);
col = 1;
newValue = latest.designmatrix.matrix(row,col) + 25;

strParams.requestString = ...
    sprintf([...
    '<?xml version="1.0" encoding="UTF-8"?>' ...
    '<info>'...
    ['<set dataid="',strParams.dataIDString,'">']...
    '<designmatrix>'...
    ['<cell row="',num2str(row-1),'" col="',num2str(col-1),'">']... % account for zero-based stuff
    num2str(newValue)... ,'0.513479'...
    '</cell>'...
    '</designmatrix>'...
    '</set>'...
    '</info>']);
tcpipInfoSet(strParams);

% wait a bit then get design matrix again
pause(2)

% create request to get design matrix
strParams.requestString = sprintf([...
    '<?xml version="1.0" encoding="UTF-8"?>' ...
    '<info>' ...
    ['<get dataid="',strParams.dataIDString,'">']...
    '</get>' ...
    '</info>' ...
    ]);

latest = getRTUpdate(strParams);

% verify changes
if ~strcmp(latest.designmatrix.columnnames{newIndex},newName)
    error('Name change was not propagated')
end
if latest.designmatrix.isofinterest(newIndex) ~= newIsOfInterest
    error('isofinterest change was not propagated')
end

fprintf('Finished single element change test\n');

%% end of test
fprintf('Finished RtInfoServer test\n');

%% subfunctions
function response = getRTUpdate(strParams)

% get response
sIncXML = tcpipInfoGet(strParams);

% parse response from the infoserver
response = parseInfoServerResponse(sIncXML);

function tcpipInfoSet(strParams)

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
    
    in.close();
    out.close();
    skt.close();
catch ME
    strOut = 'error';
    rethrow(ME)
end

function strOut = tcpipInfoGet(strParams)

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
    skt = Socket(host,portnum);
    in = BufferedReader(InputStreamReader(skt.getInputStream()));
    out = PrintWriter(skt.getOutputStream(),true);
    
    % send a request
    sendstr = String(strParams.requestString);
    out.println(sendstr);
    
    % receive the response
    strOut = [];
    tmp = in.read();
    while(~strcmp(tmp,term) && tmp ~= -1)
        strOut(end+1) = char(tmp);
        tmp = in.read();
    end
    in.close();
    out.close();
    skt.close();
catch ME
    strOut = 'error';
    rethrow(ME)
end

strOut = char(strOut);

function response = parseInfoServerResponse(xmlstr)
% parse a response from the infoserver

% build response struct
response = struct;
response.xmlstr = xmlstr;
response.designmatrix = [];
response.bError = false;

if(isempty(xmlstr))
    return
end

% java imports
import java.*;
import java.io.*;
import java.lang.*;
import java.util.*;
import java.text.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.*;

% parse the xml response and fill subjopts
factory = DocumentBuilderFactory.newInstance();
builder = factory.newDocumentBuilder();
reader = StringReader(String(xmlstr));
is = InputSource(reader);
doc = builder.parse(is);

% search the response for info tags
infoList = doc.getElementsByTagName('info');
for i=0:infoList.getLength() - 1
    % handle designmatrix
    designmatrix = infoList.item(i).getElementsByTagName('designmatrix');
    for j = 0:designmatrix.getLength() - 1
        response = parseDesignMatrixNode(response,designmatrix.item(j));
    end
end

function response = parseDesignMatrixNode(response,designmatrix)
% parse a designmatrix xml node

% set up
dmInd = length(response.designmatrix)+1;

% set dataid (still java.lang.String)
response.designmatrix(dmInd).dataid = ...
    designmatrix.getAttributes().getNamedItem('dataid').getNodeValue();

% get design matrix parts:

% get matrix
matrix = designmatrix.getElementsByTagName('matrix').item(0);

% get rows and columns
numRows = str2double(matrix.getAttributes().getNamedItem('rows').getNodeValue());
numCols = str2double(matrix.getAttributes().getNamedItem('cols').getNodeValue());

% reshape matrix string into an actual matrix
response.designmatrix(dmInd).matrix = ...
    transpose(reshape(str2num(char(matrix.getTextContent())),numCols,numRows));

% get column names
columnnames = char(designmatrix.getElementsByTagName('columnnames').item(0).getTextContent());

% parse column names into a cell array
delims = strfind(columnnames,' '); % find spaces in column names string
delims(end+1) = numel(columnnames)+1; % add the total size of string as last element in order to make loop behave nicely
startInd = 1;
for c = 1:numCols
    % create cell array element from each column name
    response.designmatrix(dmInd).columnnames{c} = columnnames(startInd:delims(c)-1);
    
    % move the startInd up to the next column name
    startInd = delims(c)+1;
end

% get interest vector
response.designmatrix(dmInd).isofinterest = ...
    logical(str2num(char(designmatrix.getElementsByTagName('isofinterest').item(0).getTextContent())));