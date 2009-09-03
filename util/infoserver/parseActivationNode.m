function response = parseActivationNode(response,activation)
%PARSEACTIVATIONNODE parses an activation xml node

% set up index actInd (for the case where someone asks for two separate
% activations in the same get)
if ~isfield(response,'activation')
    response.activation = struct;
    actInd = 1;
else % there is already some info in response.activation, so we will add to it
    actInd = length(response.activation)+1;
end

% set dataid
response.activation(actInd).dataid = ...
    char(activation.getAttributes().getNamedItem('dataid').getNodeValue());

% TODO maybe parse up dataid into nice fields in the response structure (once the
% actual data id of what is being sent is present as opposed to wild cards)

% set roiID
response.activation(actInd).roiid = ...
    char(activation.getAttributes().getNamedItem('roiid').getNodeValue());

% set data
response.activation(actInd).data = ...
    cell2mat(textscan(char(activation.getTextContent()),'%f'));