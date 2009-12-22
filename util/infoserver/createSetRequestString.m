function strOut = createSetRequestString(dataIDString,varargin)
%CREATESETREQUESTSTRING creates the XML string to set data via the
% infoserver based on inputs. The first required input is a dataIDString.
% The other input will determine what information is set. Depending on what
% needs to be set, a parameter/value input is used. Examples:
%
% To change column info, use the following struct:
%   columninfo = struct('index',index,'name',name,'isofinterest',isit)
%   createSetRequestString(dataIDString,'columninfo',columninfo)
%
% To change cell info for one cell to 'value'
%   cell = struct('row',row,'col',col,'value',value);
%   createSetRequestString(dataIDString,'cell',cell)
%
% To set the matrix in full, just pass in the matrix in an array
%   matrix = [1 2 3 4; 2 3 5 7];
%   createSetRequestString(dataIDString,'matrix',matrix);
%
% To set an artifact, just pass in artifact flag with a 1
%   createSetRequestString(dataIDString,'artifact',1);
%
% To set the design matrix via onsets and durations, use something like:
%   condition = struct('name','conditionName','convolve',1,'onsets',[1 4 2],'durations',2,'weights',2);
%   createSetRequestString(dataIDString,'condition',condition)
%
% To set design matrix via column, use something like:
%   condition = struct('name','on','convolve',1,'column',[0 1 1 0 1 1 0 0 0 0 0 0 1 1],'weights',2);
%   createSetRequestString(dataIDString,'condition',condition)
%
% In both condition examples, weights is optional. It will default to 1 if
% not set.
%

% TODO possibly change this function to handle doing multiple sets within
% one string

% start XML string
strOut = '<?xml version="1.0" encoding="UTF-8"?><info>';

% parse inputs
input = parseInputs(dataIDString,varargin);

fields = fieldnames(input);
fields = fields{:};

% dataIDString points to a design matrix (currently the only setting
% supported)
if ~isempty(strfind(dataIDString,'design-matrix'))
    
    % open set tag
    strOut = [strOut,'<set dataid="',dataIDString,'">'];
    
    % set designmatrix tag
    strOut = [strOut,'<designmatrix>'];
    
    switch fields
        case 'columninfo'
            strOut = [strOut,'<columninfo>'];
            strOut = [strOut,'<index>',num2str(input.columninfo.index),'</index>'];
            strOut = [strOut,'<name>',input.columninfo.name,'</name>'];
            strOut = [strOut,'<isofinterest>',num2str(input.columninfo.isofinterest),'</isofinterest>'];
            strOut = [strOut,'</columninfo>'];
        case 'cell'
            strOut = [strOut,'<cell row="',num2str(input.cell.row),'" col="',num2str(input.cell.col),'">'];
            strOut = [strOut,num2str(input.cell.value)];
            strOut = [strOut,'</cell>'];
        case 'matrix'
            [r,c] = size(input.matrix);
            strOut = [strOut,'matrix rows="',num2str(r),'" cols="',num2str(c),'">'];
            strOut = [strOut,num2str(input.matrix)];
            strOut = [strOut,'</matrix>'];
        case 'artifact'
            strOut = [strOut,'<artifact>1</artifact>'];
        case 'condition'
            % TODO tentative, will need to be finalized once rtdesignmatrix
            % code is finalized (setting via onsets and durations)
            
            % either condition type needs a name and convolve flag
            strOut = [strOut,'<condition name="',input.condition.name,'" convolve="',num2str(input.condition.convolve),'">'];
            
            % create an appropriate string based on if we're using a column
            % or onsets/durations.
            if isfield(input.condition,'column')
                strOut = [strOut,'<column>',num2str(input.condition.column),'</column>'];
            elseif all(isfield(input.condition,{'onsets','durations'}))
                strOut = [strOut,'<onsets>',num2str(input.condition.onsets),'</onsets>'];
                strOut = [strOut,'<durations>',num2str(input.condition.durations),'</durations>'];
            else
                error('Some inputs for condition XML string are missing/incorrect');
            end
            
            % add weights tag if applicable
            if isfield(input.condition,'weights')
                strOut = [strOut,'<weights>',num2str(input.condition.weights),'</weights>'];
            end
            strOut = [strOut,'</condition>'];
        otherwise
            % something we haven't coded for yet
    end
    
    % close designmatrix tag
    strOut = [strOut,'</designmatrix>'];
end

% close string
strOut = [strOut,'</set></info>'];


function input = parseInputs(dataIDString,args)

% create inputParser object
p = inputParser;

% set it to not expand structure inputs (i.e. code in this function will
% rely on it being in a struct
p.StructExpand = false;
p.addRequired('dataIDString',@designMatrixCheck);
p.addParamValue('columninfo',struct([]),@columnInfoCheck);
p.addParamValue('cell',struct([]),@cellCheck);
p.addParamValue('matrix',[],@isnumeric);
p.addParamValue('artifact',[],@isnumeric);
p.addParamValue('condition',struct([]),@conditionCheck);

% parse input values
p.parse(dataIDString,args{:});

% set the verified, non-empty input for use
paramList = {'columninfo','cell','matrix','artifact','condition'};

% for now, we can only set one thing at a time
for i = 1:numel(paramList)
    if ~isempty(p.Results.(paramList{i}))
        input = struct(paramList{i},p.Results.(paramList{i}));
    end
end

% checking functions
function bool = designMatrixCheck(dataIDString)
bool = ~isempty(strfind(dataIDString,'design-matrix'));

function bool = columnInfoCheck(columnInfo)
bool = isstruct(columnInfo) && ...
    all(isfield(columnInfo,{'index','name','isofinterest'})) && ...
    isnumeric(columnInfo.index) && ...
    ischar(columnInfo.name) && ...
    (islogical(columnInfo.isofinterest)||isnumeric(columnInfo.isofinterest));

function bool = cellCheck(cellinfo)
bool = isstruct(cellinfo) && ...
    all(isfield(cellinfo,{'row','col','value'})) && ...
    isnumeric(cellinfo.row) && ...
    isnumeric(cellinfo.col) && ...
    isnumeric(cellinfo.value);

function bool = conditionCheck(condition)
bool = isstruct(condition) && ...
    ischar(condition.name) && ...
    isnumeric(condition.convolve) && ...
    (all(isfield(condition,{'onsets','durations'})) || isfield(condition,'column'));

