function strOut = dummyInfoServer(strParams)
%DUMMYINFOSERVER is called by tcpipInfoGet when tcpipInfoGet is called with
% a 'test' flag

% parse string for all <get></get> pairs
getStartInds = strfind(strParams.requestString,'<get');
getEndInds   = strfind(strParams.requestString,'get>')+numel('get>')-1; % hacky adjustment

if numel(getStartInds) ~= numel(getEndInds)
    error('Something is wrong here')
end

% loop through the gets
for g = 1:numel(getStartInds)
    
    % get one <get> node at a time
    currentGet = strParams.requestString(getStartInds(g):getEndInds(g));
    
    if any(strfind(lower(currentGet),'activation'))
        % they asked for an activation, so 
        
        % pull dataid out of current <get> node
        delims = strfind(currentGet,':');
        dataid = currentGet(delims(1):delims(end));
        roiid = currentGet((delims(end-1)+1:delims(end)-1));
        
        % create an <activation> string with a normally distributed
        % pseudorandom response
        activationStrings{g} = [...
            '<activation dataid="',dataid,'" roiid="',roiid,'">',...
            num2str(randn,'%f '),...
            '</activation>'];
    end
    
    if any(strfind(lower(currentGet),'design-matrix'))
        % ignore?
        strOut = '';
    end
end

% create a response (strOut)
strOut = strcat('<?xml version="1.0" encoding="UTF-8"?>','<info>',activationStrings{:},'</info>');