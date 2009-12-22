function strOut = createDataIDString(varargin)
%CREATEDATAIDSTRING makes a data ID in the form of:
%
%   :siteid:study:series:tr:history:module:data:roi:
%
% If any fields are left blank, a wildcard '*' is used, except in the case
% of the data field, in which case 'design-matrix' is used.
%

% parse inputs
[siteid,study,series,tr,history,module,data,roi] = parseInputs(varargin);
delim = ':';
strOut = [delim,...
    num2str(siteid),delim,...
    num2str(study),delim,...
    num2str(series),delim,...
    num2str(tr),delim,...
    history,delim,...
    module,delim,...
    data,delim,...
    roi,delim];

function [siteid,study,series,tr,history,module,data,roi] = parseInputs(inputs)

% create an inputParser objects and define parameter-value arguments
p = inputParser;
p.addParamValue('siteid','*',@isnumeric);
p.addParamValue('study','*',@isnumeric);
p.addParamValue('series','*',@isnumeric);
p.addParamValue('tr','*',@isnumeric);
p.addParamValue('history','*',@ischar);
p.addParamValue('module','*',@ischar);
p.addParamValue('data','design-matrix',@ischar);
p.addParamValue('roi','*',@ischar);

% parse input values
p.parse(inputs{:});

% assign variables
siteid = p.Results.siteid;
study = p.Results.study;
series = p.Results.series;
tr = p.Results.tr;
history = p.Results.history;
module = p.Results.module;
data = p.Results.data;
roi = p.Results.roi;