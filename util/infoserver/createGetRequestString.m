function strOut = createGetRequestString(dataIDString,varargin)
%CREATEGETREQUESTSTRING creates the XML string to get data from the
% infoserver based on inputs. Required input is a dataIDString. If there
% are no other arguments, it will just create a string to request whatever
% is specified in the dataIDString. Subsequent arguments should request
% specific parts of whatever is being requested with the dataIDString.
%
% For example, if the dataIDString is requesting the design matrix, a
% subsequent 'columnnames' argument would return just the columnnames of
% the design matrix.
%
% For requests that require additional input (e.g., 'column' requires a
% specific column name in order to return the proper column), a
% parameter-value pair inside a cell is required:
%
%   createGetRequestString(dataIDString,{'column','mycol'})
%
% Note: The subsequent arguments need to be in line with whatever
% functionality is in the serializeAsXML() function for whatever data class
% is being requested. Currently only the design matrix has the ability to
% return parts:
%
%   'matrix'
%   {'column','colname'}
%   'columnnames'
%   'isofinterest'
%   'artifacts'
%
% Activation just returns a single number, so there is no need for
% subsequent arguments. To return multiple activations, you can pass in a
% cell array of dataIDStrings as follows:
%
%   {':*:*:*:*:mri:current-activation:activation-img:brain:';...
%    ':*:*:*:*:mri:current-activation:activation-img:active:'};
%
% This will create one request with two things to request.

% % parse inputs
% parseInputs(dataIDString);

% create get request TODO will need other cases, logic, etc. here for all

% start XML string
strOut = '<?xml version="1.0" encoding="UTF-8"?><info>';

% if dataIDString is not a cell, just one thing is in request
if ~iscell(dataIDString)
    
    % add in optional arguments for design-matrix if they exist
    if ~isempty(strfind(dataIDString,'design-matrix'))
        
        % open get tag
        strOut = [strOut,'<get dataid="',dataIDString,'">'];
        
        % if more arguments, just asking for specific parts, so find those
        if nargin > 1
        
        % add in <designmatrix> flag
        strOut = [strOut,'<designmatrix>'];
        
        for i = 1:nargin-1
            % if the input is a cell, it's a pair, so handle those
            if iscell(varargin{i})
                % if there is a 'column' flag
                if strcmp(varargin{i}{1},'column')
                    strOut = [strOut,'<column name="',varargin{i}{2},'"></column>'];
                end
            else
                switch varargin{i}
                    case 'matrix'
                        strOut = [strOut,'<matrix></matrix>'];
                    case 'columnnames'
                        strOut = [strOut,'<columnnames></columnnames>'];
                    case 'isofinterest'
                        strOut = [strOut,'<isofinterest></isofinterest>'];
                    case 'artifacts'
                        strOut = [strOut,'<artifacts></artifacts>'];
                    otherwise
                        % do nothing
                end
            end
        end
        
        % add in </designmatrix> flag
        strOut = [strOut,'</designmatrix>'];
        
        end
        
        % close get tag
        strOut = [strOut,'</get>'];
    else % just create a get statement with the id
        % print dataid line
        strOut = [strOut, '<get dataid="',dataIDString,'"></get>'];
    end
elseif iscell(dataIDString) % dataIDString is a cell array, someone is requesting more than one thing in this request (e.g., two activations)
    for d = 1:numel(dataIDString)
        % print dataid line
        strOut = [strOut, '<get dataid="',dataIDString{d},'"></get>'];
    end
end

% close out info tag
strOut = [strOut,'</info>'];

% function parseInputs(dataIDString)
%
% p = inputParser;
% p.addRequired('dataIDString',@ischar);
%
% % parse input values
% p.parse(dataIDString);