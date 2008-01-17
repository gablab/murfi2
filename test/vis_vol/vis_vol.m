%
% Oliver Hinds <ohinds@mit.edu>
% 2007-11-09

function vis_vol(vol, thresh, base)
  
  squareside = ceil(log2(size(vol,3)))+1;

  if(nargin < 2)
    thresh = 0;
  end
  
  if(nargin < 3)
    base = zeros(size(vol));
  end
  
  
  % build custom colormap
  bbits = 2^(2^ceil(log2(ceil(log2(max(base(:)))))));
  if(bbits == 2^16 & max(base(:)) < 2^12)
    bbits = 2^12;
  end
  
  cmprec = 64;
  j = jet(cmprec+20);
  cm = [gray(bbits); j([cmprec/2:-1:1 cmprec+20:-1:cmprec/2+21],:)];
  
  im = zeros(squareside*size(vol,2),squareside*size(vol,1));
  if(nargin < 3)
    base = zeros(squareside*size(vol,2),squareside*size(vol,1));
  end
  
  % decide scale
  maxv = max(abs(vol(:)));
  step = (cmprec/(maxv-thresh))/2;
  
  curz = 1;
  for(i=1:squareside)
    for(j=1:squareside)
      if(curz > size(vol,3))
	break;
      end
      
      panel = base(:,:,curz)';
      act = vol(:,:,curz)';
            
      % find vol below -threshold
      inds = find(act <= -thresh);
      panel(inds) = bbits + cmprec/2+step*(act(inds)+thresh)+1;

      % find vol above threshold
      inds = find(act >= thresh);
      panel(inds) = bbits + cmprec/2+step*(act(inds)-thresh);
      
      % assign base
      im(...
	  size(vol,1)*(i-1)+1:size(vol,1)*(i-1)+size(vol,2),...
	  size(vol,2)*(j-1)+1:size(vol,2)*(j-1)+size(vol,1)...
	  ) = panel;
      
      curz = curz+1;
    end
  end

  imshow(im,[0 bbits+cmprec],'InitialMagnification',400);
%  imshow(im,[0 bbits+cmprec]);
  colormap(cm);
  
return

%************************************************************************%
%%% $Source$
%%% Local Variables:
%%% mode: Matlab
%%% fill-column: 76
%%% comment-column: 0
%%% End:
