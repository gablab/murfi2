function response = parseDesignMatrixNode(response,designmatrix)
%PARSEDESIGNMATRIXNODE parses a designmatrix xml node

% set up index dmInd (for the case where someone sent two separate get things to
% infoserver in the same string, like "get designmatrix matrix" and "get
% designmatrix columnnames" insead of "get designmatrix
% matrix,columnnames")
if ~isfield(response,'designmatrix')
    response.designmatrix = struct;
    dmInd = 1;
else % there is already some info in response.designmatrix, so we will add to it
    dmInd = length(response.designmatrix)+1;
end

% set dataid
response.designmatrix(dmInd).dataid = ...
    char(designmatrix.getAttributes().getNamedItem('dataid').getNodeValue());

%% get design matrix parts

% get matrix if it exists
matrixElement = designmatrix.getElementsByTagName('matrix');
if matrixElement.getLength
    % get matrixtestActivationRetrieval
    matrix = matrixElement.item(0);
    
    % get rows and columns attributes
    numRows = str2double(matrix.getAttributes().getNamedItem('rows').getNodeValue());
    numCols = str2double(matrix.getAttributes().getNamedItem('cols').getNodeValue());
    
    % reshape matrix string into an actual matrix
    response.designmatrix(dmInd).matrix = ...
        transpose(reshape(str2num(char(matrix.getTextContent())),numCols,numRows));
end

% get columnnames vector if it exists
columnnamesElement = designmatrix.getElementsByTagName('columnnames');
if columnnamesElement.getLength
    % parse column names into a cell array
    columnnames = textscan(char(columnnamesElement.item(0).getTextContent()),'%s');
    response.designmatrix(dmInd).columnnames = columnnames{:};
end

% get interest vector if it exists
isofinterestElement = designmatrix.getElementsByTagName('isofinterest');
if isofinterestElement.getLength
    response.designmatrix(dmInd).isofinterest = logical(str2num(char(isofinterestElement.item(0).getTextContent())));
end

% get artifacts vector if it exists
artifactsElement = designmatrix.getElementsByTagName('artifacts');
if artifactsElement.getLength
    response.designmatrix(dmInd).artifacts = str2num(char(artifactsElement.item(0).getTextContent()));
end

% get single column(s) if it was requested
columnElements = designmatrix.getElementsByTagName('column');
if columnElements.getLength
   response.designmatrix(dmInd).column = str2num(char(columnElements.item(0).getTextContent));
end