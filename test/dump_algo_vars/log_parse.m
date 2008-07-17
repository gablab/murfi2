% parse a dump algo vars logfile created by arfest
%
% log_parse.m
% Login : <ohinds@jerome>
% Started on  Tue May 20 23:59:03 2008 ohinds
% $Id$
% 
% Copyright (C) 2008 ohinds

function log = log_parse(file)
  
  logfile = fopen(file);

  % parse start of entry
  fscanf(logfile,'started at',1);
  fscanf(logfile,'%s',2);
  
  % get field names
  log.fields = {};
  s = fscanf(logfile,'%s',1);
  while(~strcmp(s,'end'))
    log.fields{end+1} = s;
    s = fscanf(logfile,'%s',1);
  end  
  num_fields = length(log.fields);
  
  % read the data
  logdata = zeros(1000000,num_fields);
  i=1;
  while(~feof(logfile))
    try
      logdata(i,:) = fscanf(logfile,'%f',num_fields);
    catch
      break
    end

    if(i>1 && logdata(i,1) ~= logdata(i-1,1))
      fprintf('reading timepoint %d\n',logdata(i,1));
    end
    i=i+1;
  end
  num_rows = i-1;
  logdata(i:end,:) = [];

  % reshape for voxels
  skip=2;
  nvox = size(unique(logdata(:,2)),1);
  log.data = zeros(size(logdata,1)/nvox,nvox,size(logdata,2));
  for(i=1:size(logdata,1))
    log.data(logdata(i,1)-skip,mod(i-1,nvox)+1,:) = logdata(i,:);
  end
  
return