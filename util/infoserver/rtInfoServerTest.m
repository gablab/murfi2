function rtInfoServerTest(varargin)
%RTINFOSERVERTEST runs connection, setting, and receiving tests for the
% RtInfoServer in murfi.
%
% NOTE: currently you have to start murfi (with the rtInfoServerTest.xml
% conf file) as well as the example_data/scripts/servedata.sh image server
% before running this test. 

% create list of tests via function handles in first column of 'cases'
cases{1,1} = @testDesignMatrixRetrieval;
cases{2,1} = @testActivationRetrieval;
cases{3,1} = @testConditionVectorChange;
cases{4,1} = @testSingleCellChange;
cases{5,1} = @testArtifactAddition;
cases{6,1} = @testSetFullMatrix;
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
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    
    % create request to get design matrix parts
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
    % TODO redo this
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
    strParams.dataIDString = ':*:*:*:*:mri:current-activation:activation-img:brain:';
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',strParams.dataIDString,'">']...
        '</get>' ...
        '</info>' ...
        ]);
    
    % get design matrix from infoserver
    latest = getRTUpdate(strParams);
    
    % verify retrieval
    % TODO something better
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
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    
    strParams.requestString = ...
        sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>'...
        ['<get dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        '<columnnames></columnnames>'...
        '<isofinterest></isofinterest>'...
        '</designmatrix>'...
        '</get>' ...
        '</info>' ...
        ]);
    
    % TODO just get names, then pick first one, then use:
    %'<column name="whatevernamefromgettting"></column>',...
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
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
    % TODO change this to just get changed column
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        ['<column name=',newName,'></column>']...
        '</designmatrix>'...
        '</get>' ...
        '</info>' ...
        ]);
    latest = getRTUpdate(strParams);
    
    % verify changes
    % TODO add more here
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
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    strParams.requestString = ...
        sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>'...
        ['<get dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        '<matrix></matrix>'...
        '</designmatrix>'...
        '</get>' ...
        '</info>' ...
        ]);
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % make a change to the design matrix
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
    pause(3)
    
    % create request to get design matrix
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        '<matrix></matrix>'...
        '</designmatrix>'...
        '</get>' ...
        '</info>' ...
        ]);
    
    % get latest design matrix
    latest = getRTUpdate(strParams);
    
    % verify changes
    if floor(latest.designmatrix.matrix(row,col))~=floor(newValue)
        error('single cell change was not propagated')
    end
catch ME
    out = ME;
end

function out = testArtifactAddition
out = [];
try
    
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    
    % create request to set artifact
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<set dataid="',strParams.dataIDString,'">']...
        '<designmatrix>' ...
        '<artifact>1</artifact>' ...
        '</designmatrix>' ...
        '</set>' ...
        '</info>' ...
        ]);
    tcpipInfoSet(strParams);
    
    % verify changes
    % create request to get artifact
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',strParams.dataIDString,'">']...
        '<designmatrix>',...
        '<artifacts></artifacts>',...
        '</designmatrix>',...
        '</get>' ...
        '</info>' ...
        ]);
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
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    
    % create request or set string
    dmText = '0.000000 1.000000 1.000000 0.000000 0.000000 1.000000 2.000000 0.000000 0.000000 1.000000 3.000000 0.000000 0.000000 1.000000 4.000000 0.000000 0.000000 1.000000 5.000000 0.000000 0.000000 1.000000 6.000000 0.000000 0.112265 1.000000 7.000000 0.000000 0.682949 1.000000 8.000000 0.000000 1.062987 1.000000 9.000000 0.000000 1.144360 1.000000 10.000000 0.000000 1.108411 1.000000 11.000000 0.000000 0.943096 1.000000 12.000000 0.000000 0.338357 1.000000 13.000000 0.000000 -0.056515 1.000000 14.000000 0.000000 -0.142804 1.000000 15.000000 0.000000 -0.108175 1.000000 16.000000 0.000000 0.056904 1.000000 17.000000 0.000000 0.661643 1.000000 18.000000 0.000000 1.056515 1.000000 19.000000 0.000000 1.142804 1.000000 20.000000 0.000000 1.108175 1.000000 21.000000 0.000000 0.943096 1.000000 22.000000 0.000000 0.338357 1.000000 23.000000 0.000000 -0.056515 1.000000 24.000000 0.000000 -0.142804 1.000000 25.000000 0.000000 -0.108175 1.000000 26.000000 0.000000 0.056904 1.000000 27.000000 0.000000 0.661643 1.000000 28.000000 0.000000 1.056515 1.000000 29.000000 0.000000 1.142804 1.000000 30.000000 0.000000 1.108175 1.000000 31.000000 0.000000 0.943096 1.000000 32.000000 0.000000 0.338357 1.000000 33.000000 0.000000 -0.056515 1.000000 34.000000 0.000000 -0.142804 1.000000 35.000000 0.000000 -0.108175 1.000000 36.000000 0.000000 0.056904 1.000000 37.000000 0.000000 0.661643 1.000000 38.000000 0.000000 1.056515 1.000000 39.000000 0.000000 1.142804 1.000000 40.000000 0.000000 1.108175 1.000000 41.000000 0.000000 0.943096 1.000000 42.000000 0.000000 0.338357 1.000000 43.000000 0.000000 -0.056515 1.000000 44.000000 0.000000 -0.142804 1.000000 45.000000 0.000000 -0.108175 1.000000 46.000000 0.000000 0.056904 1.000000 47.000000 0.000000 0.661643 1.000000 48.000000 0.000000 1.056515 1.000000 49.000000 0.000000 1.142804 1.000000 50.000000 0.000000 1.108175 1.000000 51.000000 0.000000 0.943096 1.000000 52.000000 0.000000 0.338357 1.000000 53.000000 0.000000 -0.056515 1.000000 54.000000 0.000000 -0.142804 1.000000 55.000000 0.000000 -0.108175 1.000000 56.000000 0.000000 0.056904 1.000000 57.000000 0.000000 0.661643 1.000000 58.000000 0.000000 1.056515 1.000000 59.000000 0.000000 1.142804 1.000000 60.000000 0.000000 1.108175 1.000000 61.000000 0.000000 0.943096 1.000000 62.000000 0.000000 0.338357 1.000000 63.000000 0.000000 -0.056515 1.000000 64.000000 0.000000 -0.142804 1.000000 65.000000 0.000000 -0.108175 1.000000 66.000000 0.000000 0.056904 1.000000 67.000000 0.000000 0.661643 1.000000 68.000000 0.000000 1.056515 1.000000 69.000000 0.000000 1.142804 1.000000 70.000000 0.000000 1.108175 1.000000 71.000000 0.000000 0.943096 1.000000 72.000000 0.000000 0.338357 1.000000 73.000000 0.000000 -0.056515 1.000000 74.000000 0.000000 -0.142804 1.000000 75.000000 0.000000 -0.108175 1.000000 76.000000 0.000000 0.056904 1.000000 77.000000 0.000000 0.661643 1.000000 78.000000 0.000000 1.056515 1.000000 79.000000 0.000000 1.142804 1.000000 80.000000 0.000000 1.108175 1.000000 81.000000 0.000000 0.943096 1.000000 82.000000 0.000000 0.338357 1.000000 83.000000 0.000000 -0.056515 1.000000 84.000000 0.000000 -0.142804 1.000000 85.000000 0.000000';
    strParams.requestString = ...
        sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>'...
        ['<set dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        ['<matrix rows="',num2str(85),'" cols="',num2str(4),'">'],...
        dmText,...
        '</matrix>'...
        '</designmatrix>'...
        '</set>'...
        '</info>']);
    
    % send set command
    tcpipInfoSet(strParams);
    
    % add in checks/verification
    % create request to get design matrix parts
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',strParams.dataIDString,'">']...
        '</get>' ...
        '</info>' ...
        ]);
    latest = getRTUpdate(strParams);
    
    % verify set
    if ~all(all(reshape(str2num(dmText),4,85)'==latest.designmatrix.matrix))
        error('Retrieved design matrix is not equal to set design matrix.')
    end
catch ME
    out = ME;
end

function out = testDummyInfoServer
out = [];
try
    % set up strParams parameters
    % set up parameters
    strParams.TCP_HOST = '';
    strParams.TCP_PORT = 15001;
    strParams.requestString = ['<?xml version="1.0" encoding="UTF-8"?>'...
        '<info>'...
        '<get dataid=":*:*:*:*:mri:current-activation:activation-img:brain:"></get>'...
        '<get dataid=":*:*:*:*:mri:current-activation:activation-img:active:"></get>'...
        '</info>'];
    
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
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    
    % create set string
    strParams.requestString = ...
        sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>'...
        ['<set dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        '<condition name="conditionName" convolve="true">'...
        '<onsets>1 4 2</onsets>'...
        '<durations>2</durations>'...
        '<weights>2</weights>'...
        '</condition>'...
        '</designmatrix>'...
        '</set>'...
        '</info>']);
    
    % send set command
    tcpipInfoSet(strParams);
    
    % create request to get design matrix parts
    strParams.requestString = sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>' ...
        ['<get dataid="',strParams.dataIDString,'">']...
        '</get>' ...
        '</info>' ...
        ]);
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
    strParams.dataIDString = ':*:*:*:*:*:*:design-matrix::';
    % create set string
    strParams.requestString = ...
        sprintf([...
        '<?xml version="1.0" encoding="UTF-8"?>' ...
        '<info>'...
        ['<set dataid="',strParams.dataIDString,'">']...
        '<designmatrix>'...
        '<condition name="on" convolve="true">'...
        '<column>'...
        '0 1 1 0 1 1 0 0 0 0 0 0 1 1'...
        '</column>'...
        '<weights>2</weights>'...
        '</condition>'...
        '</designmatrix>'...
        '</set>'...
        '</info>']);
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