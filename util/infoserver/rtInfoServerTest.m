function rtInfoServerTest(varargin)
%RTINFOSERVER runs connection, setting, and receiving tests for the
% RtInfoServer in festr.
%
% NOTE: currently you have to start festr (with the rtInfoServerTest.xml
% conf file) as well as the example_data/scripts/servedata.sh image server
% before running this test. 

% debug
if ~isempty(varargin) && varargin{1}
    % if we pass in a handle, do only that one.
    cases{1,1} = @testArtifactAddition;
else
    % create list of tests via function handles in first column of 'cases'
    cases{1,1} = @testDesignMatrixRetrieval;
    cases{2,1} = @testActivationRetrieval;
    cases{3,1} = @testConditionVectorChange;
    cases{4,1} = @testSingleCellChange;
    cases{5,1} = @testArtifactAddition;
end


% start test
fprintf('Starting RtInfoServer test.\n');

% % start up festr
% fprintf('Starting festr...\n')
% [status,result] = system('../../bin/festr -f rtInfoServerTest.xml &');
% 
% % if something broke, print it
% if status
%     fprintf(result);
% end
% 
% % let festr warm up a little
% fprintf('Letting festr warm up...\n')
% pause(3);
% 
% % start picture server
% fprintf('Starting picture server...\n')
% [status,result] = system('cd ../../example_data/scripts; ./servedata.sh 500');
% 
% % let pictures run a bit
% pause(3);

% run tests and put results into second column of 'cases'
for c = 1:size(cases,1)
    % this calls each test subfunction via function handles set above. each
    % subufunction gets 'strParams' as an input
    cases{c,2} = cases{c,1}();
    
    % if there is some output (i.e., error), display it
    if ~isempty(cases{c,2})
        fprintf('\nFailure #%d\n',c)
        fprintf('----------- \n')
        disp(getReport(cases{c,2},'extended'))
    end
end

% % kill festr process
% fprintf('Killing festr...\n')
% [status,result] = system('kill -9 `ps -o pid h -C festr`');
% TODO need to kill the serving?
% 
% % if something broke, print out result
% if status
%     fprintf(result)
% end

% process errors
fprintf('Finished RtInfoServer test\n');

% subfunctions
function out = testDesignMatrixRetrieval(strParams)

fprintf('Starting design matrix retrieval test\n');

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
    
    fprintf('Finished design matrix retrieval test\n');    
catch ME
    out = ME;
end

function out = testActivationRetrieval

fprintf('Starting activation retrieval test\n');

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
    
    fprintf('Finished activation retrieval test\n');
catch ME
    out = ME;
end

function out = testConditionVectorChange(strParams)

fprintf('Starting condition vector change test\n');

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
    
    fprintf('Finished condition vector change test\n');
    
catch ME
    out = ME;
end

function out = testSingleCellChange(strParams)

fprintf('Starting single element change test\n');

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
    
    fprintf('Finished single element change test\n');
catch ME
    out = ME;
end

function out = testArtifactAddition(strParams)

fprintf('Starting artifact addition test\n');

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
    
    fprintf('Finished artifact addition test \n');
catch ME
    out = ME;
end