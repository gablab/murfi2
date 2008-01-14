% Oliver Hinds <ohinds@mit.edu>
% 2007-11-09

function show_timecourse(vols,thresh,bg)
  
  if(nargin < 2)
    thresh = 0;
  end
  
  if(nargin < 3)
    bg = zeros(size(vols(:,:,:,1)));
  end
  
  i = 1;
  while(1)
    vis_vol(vols(:,:,:,i),thresh,bg);
    title(i);
    pause(0.1);
    i = i+1;
    if(i > size(vols,4))
      i = 1;
    end
  end
  
return

%************************************************************************%
%%% $Source$
%%% Local Variables:
%%% mode: Matlab
%%% fill-column: 76
%%% comment-column: 0
%%% End:
