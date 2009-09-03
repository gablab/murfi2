function response = getRTUpdate(strParams)

% get response
sIncXML = tcpipInfoGet(strParams);

% parse response from the infoserver
response = parseInfoServerResponse(sIncXML);