% im = read_raw(filename)
%
% read an image writen by the siemens receiver in raw format
%
% Oliver Hinds <ohinds@mit.edu> 2009-06-09

function im = read_raw(filename)

  % error check
  if(~exist('filename','var'))
    error('filename is a required argument');
  end
  
  % open the file
  fp = fopen(filename);
  if(fp == -1) 
    error(['couldnt open file ' filename]);
  end
  
  % read the header
  ndim = fread(fp, 1, 'uint32');
  if(ndim < 1)
    error(['error parsing header of ' filename]);
  end
  
  dims = fread(fp, ndim, 'uint32');
  if(any(dims < 1))
    error(['error parsing header of ' filename]);
  end
  dims = dims';
  
  % read the data
  data = fread(fp, prod(dims), 'int16=>int16');
  
  % reshape the data
  im = reshape(data,dims);
  
return