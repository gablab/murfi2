function response = parseInfoServerResponse(xmlstr)
%PARSEINFOSERVERRESPONSE parses the response from the infoserver which it
% receives in the form of an xml string.

% java imports
import java.*;
import java.io.*;
import java.lang.*;
import java.util.*;
import java.text.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.*;

% do nothing for an empty string
if isempty(xmlstr)
    return
end

% build response struct
response = struct;
response.xmlstr = xmlstr;
response.error = [];

% parse the xml response and fill subjopts
factory = DocumentBuilderFactory.newInstance();
builder = factory.newDocumentBuilder();
reader = StringReader(String(xmlstr));
is = InputSource(reader);
doc = builder.parse(is);

% search the response for info tags
infoList = doc.getElementsByTagName('info');
for i = 0:infoList.getLength() - 1
    
    % handle designmatrix
    designmatrix = infoList.item(i).getElementsByTagName('designmatrix');
    for j = 0:designmatrix.getLength() - 1
        response = parseDesignMatrixNode(response,designmatrix.item(j));
    end
    
    % handle activation
    activation = infoList.item(i).getElementsByTagName('activation');
    for j = 0:activation.getLength() - 1
        response = parseActivationNode(response,activation.item(j));
    end
    
    % any other nodes to handle in the future must be handled here, similar
    % to the designmatrix code above
    
end
