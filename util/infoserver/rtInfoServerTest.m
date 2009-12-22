function rtInfoServerTest(varargin)
%RTINFOSERVERTEST runs connection, setting, and receiving tests for the
% RtInfoServer in murfi.
%
% NOTE: currently you have to start murfi (with the rtInfoServerTest.xml
% conf file) as well as the example_data/scripts/servedata.sh image server
% before running this test.

% TODO
%   beef up tests
%   automate festr starting for each test?

% create list of tests via function handles in first column of 'cases'
cases{1,1} = @testDesignMatrixRetrieval;
cases{2,1} = @testActivationRetrieval; % needs to have images being dealt
cases{3,1} = @testConditionVectorChange;
cases{4,1} = @testSingleCellChange;
cases{5,1} = @testArtifactAddition;
cases{6,1} = @testSetFullMatrix; % needs a fresh instance of murfi
cases{7,1} = @testSetDesignMatrixFromOnsetsAndDurations;
cases{8,1} = @testSetDesignMatrixFromColumn;
cases{9,1} = @testDummyInfoServer;

% currently you have to specify what test you want to run
if isempty(varargin)
    fprintf('You need to specify a specific test to run. Options are:\n');
    for c = 1:size(cases,1)
        fprintf('%s\n',func2str(cases{c,1}))
    end
    return
end

fprintf('Starting InfoServer testing.\n');

% check if a specific test has been specified in the function inputs,
% otherwise run all tests
for c = 1:size(cases,1)
    if ischar(varargin{1})
        % create handle for the specified test and then run it
        specificTest = str2func(varargin{1});
        fprintf('    Starting %s test.\n',func2str(specificTest));
        cases{c,2} = specificTest();
        fprintf('    Finished %s test.\n',func2str(specificTest));
        break
    else
        % currently you have to specify what test you want to run
        %         % call each test subfunction via function handles set above
        %         fprintf('    Starting %s test.\n',func2str(cases{c,1}));
        %         cases{c,2} = cases{c,1}();
        %         fprintf('    Finished %s test.\n',func2str(cases{c,1}));
    end
end

% check through results in second column of cases for errors
for c = 1:size(cases,1)
    % if there is some output (i.e., error), display it
    if ~isempty(cases{c,2})
        fprintf('\nFailure #%d in %s\n',c,func2str(cases{c,1}))
        fprintf('---------------------------------------------------------- \n')
        disp(getReport(cases{c,2},'extended'))
    end
end

fprintf('Finished InfoServer testing.\n');

function out = testDesignMatrixRetrieval
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create request to get design matrix
    strParams.dataIDString = createDataIDString('data','design-matrix');
    strParams.requestString = createGetRequestString(strParams.dataIDString);
    
    % get design matrix from infoserver
    latest = getRTUpdate(strParams);
    
    % basically check that it came back
    reqFields = {'matrix','columnnames','isofinterest','artifacts'};
    fieldCheck = isfield(latest.designmatrix,reqFields);
    
    if any(~fieldCheck)
        error('Missing field in result %s\n',reqFields{~fieldCheck})
    end
catch ME
    out = ME;
end

function out = testActivationRetrieval
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create request to get an activation
    strParams.dataIDString = createDataIDString('history','mri','module','current-activation','data','activation-img','roi','brain');
    strParams.requestString = createGetRequestString(strParams.dataIDString);
    
    % get design matrix from infoserver
    latest = getRTUpdate(strParams);
    
    % verify retrieval
    if ~isfield(latest.activation,'data') || ~isnumeric(latest.activation.data)
        error('Problem retrieving activation data');
    end
catch ME
    out = ME;
end

function out = testConditionVectorChange
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create request to get design matrix parts
    strParams.dataIDString = createDataIDString('data','design-matrix');
    strParams.requestString = createGetRequestString(strParams.dataIDString,'columnnames','isofinterest');
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % make a change to the condition vector based on latest design matrix
    columninfo.index = numel(latest.designmatrix.columnnames)-1; % get last column (take into account zero-based indexing)
    columninfo.name = ['modified_' latest.designmatrix.columnnames{columninfo.index}]; % modify column name
    columninfo.isofinterest = ~latest.designmatrix.isofinterest(columninfo.index); % flip interest bool
    
    % send changes via infoserver
    strParams.requestString = createSetRequestString(strParams.dataIDString,'columninfo',columninfo);
    tcpipInfoSet(strParams);
    
    % create request to get edited design matrix
    strParams.requestString = createGetRequestString(strParams.dataIDString,{'column',columninfo.name});
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify changes
    if isempty(latest.designmatrix.column)
        error('Requested column is empty')
    end
catch ME
    out = ME;
end

function out = testSingleCellChange
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create request to get design matrix parts
    strParams.dataIDString = createDataIDString('data','design-matrix');
    strParams.requestString = createGetRequestString(strParams.dataIDString,'matrix');
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % make a change to the design matrix
    cell.row = floor(size(latest.designmatrix.matrix,1)/2)-1; % account for zero-based indexing
    cell.col = 1-1; % account for zero-based indexing
    cell.value = latest.designmatrix.matrix(cell.row+1,cell.col+1) + 25; % account for one-based indexing
    
    % send set request
    strParams.requestString = createSetRequestString(strParams.dataIDString,'cell',cell);
    tcpipInfoSet(strParams);
    
    % create request to get edited design matrix
    strParams.requestString = createGetRequestString(strParams.dataIDString,'matrix');
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify changes
    if floor(latest.designmatrix.matrix(cell.row+1,cell.col+1))~=floor(cell.value)
        error('single cell change was not propagated')
    end
catch ME
    out = ME;
end

function out = testArtifactAddition
out = [];
try
    
    %   Note that the configuration file must have ''maxNumArtifacts'' set to
    %   something other than 0 in order for the above set command to be
    %   effective:
    %   <code xml>
    %       <option name="modelArtifacts"> true </option>
    %       <option name="maxNumArtifacts"> 1 </option>
    %   </code>
    
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create request to set artifact
    strParams.dataIDString = createDataIDString('data','design-matrix');
    strParams.requestString = createSetRequestString(strParams.dataIDString,'artifact',1);
    
    % send set request
    tcpipInfoSet(strParams);
    
    % create request to get new artifacts
    strParams.requestString = createGetRequestString(strParams.dataIDString,'artifacts');
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify changes
    if isempty(latest.designmatrix.artifacts)
        error('Artifact was not added correctly');
    end
catch ME
    out = ME;
end

function out = testSetFullMatrix
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create set string
    strParams.dataIDString = createDataIDString('data','design-matrix');
    matrix = [0.000000 1.000000 1.000000 0.000000 0.000000 1.000000 2.000000 0.000000 0.000000 1.000000 3.000000 0.000000 0.000000 1.000000 4.000000 0.000000 0.000000 1.000000 5.000000 0.000000 0.000000 1.000000 6.000000 0.000000 0.112265 1.000000 7.000000 0.000000 0.682949 1.000000 8.000000 0.000000 1.062987 1.000000 9.000000 0.000000 1.144360 1.000000 10.000000 0.000000 1.108411 1.000000 11.000000 0.000000 0.943096 1.000000 12.000000 0.000000 0.338357 1.000000 13.000000 0.000000 -0.056515 1.000000 14.000000 0.000000 -0.142804 1.000000 15.000000 0.000000 -0.108175 1.000000 16.000000 0.000000 0.056904 1.000000 17.000000 0.000000 0.661643 1.000000 18.000000 0.000000 1.056515 1.000000 19.000000 0.000000 1.142804 1.000000 20.000000 0.000000 1.108175 1.000000 21.000000 0.000000 0.943096 1.000000 22.000000 0.000000 0.338357 1.000000 23.000000 0.000000 -0.056515 1.000000 24.000000 0.000000 -0.142804 1.000000 25.000000 0.000000 -0.108175 1.000000 26.000000 0.000000 0.056904 1.000000 27.000000 0.000000 0.661643 1.000000 28.000000 0.000000 1.056515 1.000000 29.000000 0.000000 1.142804 1.000000 30.000000 0.000000 1.108175 1.000000 31.000000 0.000000 0.943096 1.000000 32.000000 0.000000 0.338357 1.000000 33.000000 0.000000 -0.056515 1.000000 34.000000 0.000000 -0.142804 1.000000 35.000000 0.000000 -0.108175 1.000000 36.000000 0.000000 0.056904 1.000000 37.000000 0.000000 0.661643 1.000000 38.000000 0.000000 1.056515 1.000000 39.000000 0.000000 1.142804 1.000000 40.000000 0.000000 1.108175 1.000000 41.000000 0.000000 0.943096 1.000000 42.000000 0.000000 0.338357 1.000000 43.000000 0.000000 -0.056515 1.000000 44.000000 0.000000 -0.142804 1.000000 45.000000 0.000000 -0.108175 1.000000 46.000000 0.000000 0.056904 1.000000 47.000000 0.000000 0.661643 1.000000 48.000000 0.000000 1.056515 1.000000 49.000000 0.000000 1.142804 1.000000 50.000000 0.000000 1.108175 1.000000 51.000000 0.000000 0.943096 1.000000 52.000000 0.000000 0.338357 1.000000 53.000000 0.000000 -0.056515 1.000000 54.000000 0.000000 -0.142804 1.000000 55.000000 0.000000 -0.108175 1.000000 56.000000 0.000000 0.056904 1.000000 57.000000 0.000000 0.661643 1.000000 58.000000 0.000000 1.056515 1.000000 59.000000 0.000000 1.142804 1.000000 60.000000 0.000000 1.108175 1.000000 61.000000 0.000000 0.943096 1.000000 62.000000 0.000000 0.338357 1.000000 63.000000 0.000000 -0.056515 1.000000 64.000000 0.000000 -0.142804 1.000000 65.000000 0.000000 -0.108175 1.000000 66.000000 0.000000 0.056904 1.000000 67.000000 0.000000 0.661643 1.000000 68.000000 0.000000 1.056515 1.000000 69.000000 0.000000 1.142804 1.000000 70.000000 0.000000 1.108175 1.000000 71.000000 0.000000 0.943096 1.000000 72.000000 0.000000 0.338357 1.000000 73.000000 0.000000 -0.056515 1.000000 74.000000 0.000000 -0.142804 1.000000 75.000000 0.000000 -0.108175 1.000000 76.000000 0.000000 0.056904 1.000000 77.000000 0.000000 0.661643 1.000000 78.000000 0.000000 1.056515 1.000000 79.000000 0.000000 1.142804 1.000000 80.000000 0.000000 1.108175 1.000000 81.000000 0.000000 0.943096 1.000000 82.000000 0.000000 0.338357 1.000000 83.000000 0.000000 -0.056515 1.000000 84.000000 0.000000 -0.142804 1.000000 85.000000 0.000000];
    strParams.requestString = createSetRequestString(strParams.dataIDString,'matrix',matrix);
    
    % send set request
    tcpipInfoSet(strParams);
    
    % create request to get design matrix
    strParams.requestString = createGetRequestString(strParams.dataIDString);
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify
    if ~all(all(reshape(matrix,4,85)'==latest.designmatrix.matrix(:,1:4)))
        error('Retrieved design matrix is not equal to set design matrix.')
    end
catch ME
    out = ME;
end

function out = testDummyInfoServer
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create request string
    str1 = createDataIDString('history','mri','module','current-activation','data','activation-img','roi','brain');
    str2 = createDataIDString('history','mri','module','current-activation','data','activation-img','roi','active');
    strParams.requestString = createGetRequestString({str1,str2});
    
    % get stuff
    strOut = tcpipInfoGet(strParams,'test');
    response = parseInfoServerResponse(strOut);
    
    % add in checks/verification
    % TODO
catch ME
    out = ME;
end

function out = testSetDesignMatrixFromOnsetsAndDurations
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create set string
    strParams.dataIDString = createDataIDString('data','design-matrix');
    condition = struct('name','conditionName','convolve',1,'onsets',[1 4 2],'durations',2,'weights',2);
    strParams.requestString = createSetRequestString(strParams.dataIDString,'condition',condition);
    
    % send set command
    tcpipInfoSet(strParams);
    
    % create request to get design matrix parts
    strParams.requestString = createGetRequestString(strParams.dataIDString);
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify changes TODO
    %     r = load('dm_regress');
    %     if r.regression.designmatrix.matrix ~= latest.designmatrix.matrix
    %         error('Matrix portion of design matrix does not match regression matrix.');
    %     end
    %     if ~strcmpi(r.regression.designmatrix.columnnames,latest.designmatrix.columnnames)
    %         error('Column names of design matrix do not match regression column names.');
    %     end
    %     if r.regression.designmatrix.isofinterest ~= latest.designmatrix.isofinterest
    %         error('Interest values of design matrix do not match regression interest values.');
    %     end
catch ME
    out = ME;
end

function out = testSetDesignMatrixFromColumn
out = [];
try
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    
    % create set string
    strParams.dataIDString = createDataIDString('data','design-matrix');
    condition = struct('name','on','convolve',1,'column',[0 1 1 0 1 1 0 0 0 0 0 0 1 1],'weights',2);
    strParams.requestString = createSetRequestString(strParams.dataIDString,'condition',condition);
    
    % send set string
    tcpipInfoSet(strParams);
    
    % create request to get design matrix parts
    strParams.requestString = createGetRequestString(strParams.dataIDString);
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify
    
catch ME
    out = ME;
end

function out = testTemplate % template test subfunction
% after filling in this, add a function handle to it in the cases
% declaration at the top of the main function
out = [];
try
    % set up strParams parameters
    
    % create request or set string
    
    % get/set stuff
    
    % add in checks/verification
catch ME
    out = ME;
end

%% leftovers
% % start up murfi
% fprintf('Starting murfi...\n')
% [status,result] = system('../../bin/murfi -f rtInfoServerTest.xml &');
%
% % if something broke, print it
% if status
%     fprintf(result);
% end
%
% % let murfi warm up a little
% fprintf('Letting murfi warm up...\n')
% pause(3);
%
% % start picture server
% fprintf('Starting picture server...\n')
% [status,result] = system('cd ../../example_data/scripts; ./servedata.sh 500');
%
% % let pictures run a bit
% pause(3);

% % kill murfi process
% fprintf('Killing murfi...\n')
% [status,result] = system('kill -9 `ps -o pid h -C murfi`');
% TODO need to kill the serving?
%
% % if something broke, print out result
% if status
%     fprintf(result)
% end