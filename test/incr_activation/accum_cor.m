% Oliver Hinds <ohinds@mit.edu>
% 2007-11-28

function [ps as Cs] = accum_cor(vol, onoff)
  
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

  % build detrend regs
  L = 2;
  s = zeros(ntp, L);  
  s(:,1) = 1;
  s(:,2) = 1:size(reg,1);
  
  %% incremental solve
  
  % allocate space to store all vars over time
  Cs = zeros([4 4 ntp]);
  ps = zeros(size(vol));
  as = zeros(size(vol));

  % alloc temp vars
  f = zeros(L+1,1);
  g = zeros(L+1,1);
  h = zeros(L+1,1);
  h = zeros(L+1,1);
  
  % initialize C
  C = 10^-7*eye(L+2);
  p = zeros(imsiz);
  a = zeros(imsiz);
  for(t=1:ntp)
    % voxel independent init
    z = [s(t,:) reg(t)];
    b_old = 1;
    
    
    for(j=1:L+1)
      h(j) = z(j)/C(j,j);
      b_new = sqrt(b_old^2+h(j)^2);
      f(j) = b_new/b_old;
      g(j) = h(j)/(b_new*b_old);
      b_old = b_new;
    
      for(k=1:L+1)
	z(k) = z(k) - h(j)*C(k,j);
	C(k,j) = f(j)*C(k,j) + g(j)*z(k);
      end
    end
    
    %keyboard
    
    % update p,a in each voxel
    im = vol(:,:,:,t);
    for(vi=1:imsiz(1))
      for(vj=1:imsiz(2))
	for(vk=1:imsiz(3))
	  z_hat = im(vi,vj,vk);
	  for(j=1:L+1)
	    z_hat = z_hat - h(j)*C(L+2,j);
	    C(L+2,j) = f(j)*C(L+2,j) + g(j)*z_hat;
	  end
	  
	  C(L+2,L+2) = sqrt(C(L+2,L+2)^2+(z_hat/b_old)^2);
	  p(vi,vj,vk) = C(L+2,L+1)/sqrt(C(L+2,L+2)^2+C(L+2,L+1)^2);
	  a(vi,vj,vk) = C(L+2,L+1)/C(L+1,L+1);
	  p(vi,vj,vk) = im(vi,vj,vk);
	end
      end
    end
%    vis_vol(im,0,im)
    vis_vol(a,0.01,im);
    pause(0.1);
    
    Cs(:,:,t) = C;
    ps(:,:,:,t) = p;
    as(:,:,:,t) = a;
  end
  
 %keyboard
  
return

%************************************************************************%
%%% $Source$
%%% Local Variables:
%%% mode: Matlab
%%% fill-column: 76
%%% comment-column: 0
%%% End:
