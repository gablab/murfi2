% Oliver Hinds <ohinds@mit.edu>
% 2007-11-21

function cor = slide_win_cor(vol,regressor,windowLen)
  ntp = size(vol,4);
  cor = zeros(size(vol));
  
  warning('off','all');
  
  b = 0;
  d = 0;
  a = zeros(size(vol(:,:,:,1)));
  c = zeros(size(vol(:,:,:,1)));
  e = zeros(size(vol(:,:,:,1)));
  
  for(t=1:ntp)
    lcor = zeros(size(vol(:,:,:,1)));
    regNewIndex = mod(t-1,length(regressor))+1;
    regOldIndex = mod(t-windowLen-1,length(regressor))+1;
    dataIndex   = mod(t-1,windowLen);

    regold = 0;
    regnew = regressor(regNewIndex);

    if(t <= windowLen) 
      b = regnew;
      d = regnew*regnew;    
    else
      regold = regressor(regOldIndex);
      b = b - regold + regnew;
      d = d - regold*regold + regnew*regnew;
    end
      
    for(i=1:size(vol,1))
      for(j=1:size(vol,2))
	for(k=1:size(vol,3))
	  la = a(i,j,k);
	  lc = c(i,j,k);
	  le = e(i,j,k);
	  xnew = vol(i,j,k,t);

	  if(t <= windowLen)
	    la = la + xnew*regnew; 
	    lc = lc + xnew;
	    le = le + xnew*xnew; 
	    n = t;
	  else
	    xold = vol(i,j,k,t-windowLen);
	    la = la + xnew*regnew - xold*regold; 
	    lc = lc + xnew - xold;
	    le = le + xnew*xnew - xold*xold; 
	    n = windowLen;
	  end

	  lcor(i,j,k) = (n*la - b*lc) / sqrt((n*d - b*b) * (n*le-lc*lc));
	
	  a(i,j,k) = la;
	  c(i,j,k) = lc;
	  e(i,j,k) = le;
	end
      end
    end    
    lcor = lcor./max(lcor(:));
    cor(:,:,:,t) = lcor;
  end
  
return

%************************************************************************%
%%% $Source$
%%% Local Variables:
%%% mode: Matlab
%%% fill-column: 76
%%% comment-column: 0
%%% End:
