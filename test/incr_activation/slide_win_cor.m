% Oliver Hinds <ohinds@mit.edu>
% 2007-11-21

function cor = slide_win_cor(vol,onoff,windowLen)

  base = mean(vol,4);
  
  hrf = spm_hrf(2);
  ntp = size(vol,4);
  imsiz = [size(vol,1) size(vol,2) size(vol,3)];
  
  % build regressors
  
  if(size(onoff,1) < size(onoff,2))
    onoff = onoff';
  end
  
  if(size(onoff,1) < ntp)
    stim = repmat(onoff,1,ceil(ntp/size(onoff,1)));
    stim = stim(1:ntp);
  else
    stim = onoff;
  end
  
  reg = conv(stim,hrf);
%  reg = stim;
  reg = reg(1:length(stim));

  plot(reg);pause
  
  cor = zeros(size(vol));
  
  warning('off','all');
  
  b = 0;
  d = 0;
  a = zeros(size(vol(:,:,:,1)));
  c = zeros(size(vol(:,:,:,1)));
  e = zeros(size(vol(:,:,:,1)));
  
  for(t=1:ntp)
    lcor = zeros(size(vol(:,:,:,1)));
    regNewIndex = t;
    regOldIndex = t-windowLen;
    dataIndex   = mod(t-1,windowLen)+1;

    regold = 0;
    regnew = reg(regNewIndex);

    if(t <= windowLen) 
      b = regnew;
      d = regnew*regnew;    
    else
      regold = reg(regOldIndex);
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
    vis_vol(lcor,0.2,base); title(num2str(t)); pause(0.1);
  end
  
return

%************************************************************************%
%%% $Source$
%%% Local Variables:
%%% mode: Matlab
%%% fill-column: 76
%%% comment-column: 0
%%% End:
