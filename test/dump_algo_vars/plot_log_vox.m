% plots timeseries of voxels from a log file 
%
% plot_log_vox.m
% Login : <ohinds@jerome>
% Started on  Wed May 21 08:14:07 2008 ohinds
% $Id$
% 
% Copyright (C) 2008 ohinds

function plot_log_vox(log,inds,block_starts)

  if(nargin > 2)
    show_block = 1;
  else
    show_block = 0;
  end
  
  numvars = length(log.fields);
  skip = 2;
  for(i=1:length(inds))
    figure('position',[800 0 683 969]);
    
    for(cur=skip+1:numvars)
      subplot(numvars-skip,1,cur-skip);
      plot(log.data(:,inds(i),cur));
      
      if(show_block)
	plot_cond_lines(block_starts);
      end
      
      ylabel(log.fields{cur});
    
      if(cur == skip+1) 
	title(sprintf('voxel %d',log.data(1,inds(i),2)));  
      end
    end
    
  end
return

function plot_cond_lines(conds)
  ylim = get(gca,'ylim');
  
  for(i=1:length(conds))
    line([conds(i) conds(i)],ylim);
  end
return