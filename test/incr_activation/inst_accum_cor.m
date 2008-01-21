% Oliver Hinds <ohinds@mit.edu>
% 2007-11-28

function [ps as Ts tcs] = inst_accum_cor(vol, onoff)
  
  hrf = spm_hrf(2);
  ntp = size(vol,4);
%  ntp = 64;
  imsiz = [size(vol,1) size(vol,2) size(vol,3)];
  vol = vol.*49;
  
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

%  keyboard
  
  vox = [
      15 27 14 % good response
      14 27 14 % good resp, linear trend
      13 27 14 % good response, quadratic trend
      14 27 12 % noisy but good response
      11 20 21 % good motor
      24 20 21 % noisy motor
      ];
  voi = 1;
  
  % build detrend regs
  L = 2;
  s = zeros(ntp, L);  
  s(:,1) = 1;
  s(:,2) = 1:size(reg,2);

  % build mask
  firstvol = squeeze(vol(:,:,:,1)); 
  mask = zeros(imsiz);
  mask(find(firstvol > 0.3*mean(firstvol(:)))) = 1;
  numvox = sum(mask(:));
  
  %% incremental solve
  
  % allocate space to store vars over time
  ps = zeros(size(vol));
  as = zeros(size(vol));
  Ts = zeros(size(vol));

  % alloc temp vars
  f = zeros(L+1,1);
  g = zeros(L+1,1);
  h = zeros(L+1,1);
  z = zeros(L+1,1);
  
  % initialize C,c,p,a
%  C = [10^-7*eye(L+1) zeros(L+1,1)]; % choleski rows that dont change per vox
  C = [10^-7*eye(L+1) zeros(L+1,1)]; % choleski rows that dont change per vox
  c = zeros([imsiz L+2]);
  c(:,:,:,end) = 10^-7; % row that does change
  p = zeros(imsiz);
  a = zeros(imsiz);
  T = zeros(imsiz);
  lastoff = zeros(imsiz);
  lastwason = 0;

%  tcs = {};
%  for(i=1:9)
%    tcs{i} = zeros(ntp,1);
%  end
%  
%  tcs{1} = squeeze(vol(vox(voi,1),vox(voi,2),vox(voi,3),:));
%  m = [];
%  
%  hndl = figure; hold on;

  offind = 0;
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

    % this timepoint
    im = vol(:,:,:,t);
    
    % start the running mean
    if(reg(t) < eps)
      if(offind == 0)
	lastoff = im;
      else
	lastoff = lastoff + (im - lastoff)./(offind+1);
%	fprintf();
      end
      offind = offind+1;
    else
%      if(offind > 0)
%	keyboard
%	lastoff = mean(m,4);
%      end
      offind = 0;
%      m = [];
    end
    
    
    % update p,a in each voxel
    tsum = 0;
    for(vi=1:size(im,1))
      for(vj=1:size(im,2))
	for(vk=1:size(im,3))
	  
	  if(mask(vi,vj,vk) == 0)
	    continue;
	  end
	  
	  if(reg(t) < eps)
	    z_hat = im(vi,vj,vk);	  
	  else
	    z_hat = lastoff(vi,vj,vk);
	  end

%	  if([vi vj vk] == vox(voi,:))
%	    tcs{2}(t) = z_hat;
%	  end
	  
	  if(vi == 15 && vj == 27 && vk == 13)
fprintf('%f ', z_hat);
	  end

	  for(j=1:L+1)
	    z_hat = z_hat - h(j)*c(vi,vj,vk,j);
	    c(vi,vj,vk,j) = f(j)*c(vi,vj,vk,j) + g(j)*z_hat;
	  end
	  
	  c(vi,vj,vk,L+2) = sqrt(c(vi,vj,vk,L+2)^2+(z_hat/b_old)^2);
	  
	  
	  % theirs
	  p(vi,vj,vk) = c(vi,vj,vk,L+1)/sqrt(c(vi,vj,vk,L+2)^2+c(vi,vj,vk,L+1)^2);
	  a(vi,vj,vk) = c(vi,vj,vk,L+1)/C(L+1,L+1);

%	  tcs{1}(vi,vj,vk,t) = c(vi,vj,vk,1)*h(1);
%	  tcs{2}(vi,vj,vk,t) = c(vi,vj,vk,2)*h(2);
%	  tcs{3}(vi,vj,vk,t) = C(L+1,L+1)/c(vi,vj,vk,L+2);
	  
	    
	  % ours
%	  T(vi,vj,vk) =
%	  (im(vi,vj,vk)-c(vi,vj,vk,1)*h(1))*C(L+1,L+1)/c(vi,vj,vk,L+2)*sqrt(t-L-1);
          if(t > L+1)
	    T(vi,vj,vk) = (im(vi,vj,vk)-c(vi,vj,vk,1)*h(1)-c(vi,vj,vk,2)* ...
			   h(2))*C(L+1,L+1)/c(vi,vj,vk,L+2);
%	    tsum = tsum + T(vi,vj,vk);
	  end
	  %          tcs{2}(t) = T(vi,vj,vk);

	  if(vi == 15 && vj == 27 && vk == 13)
	    fprintf('%d %d %f %f %f\n', t, im(vi,vj,vk), reg(t), z_hat, T(vi,vj,vk));
	  end	  
	  
%	  if([vi vj vk] == vox(voi,:))
%	  if([vi vj vk] == vox(voi,:) & t > 5)
%	    figure(hndl);
%	    plot(t,im(vi,vj,vk),'b.');
%	    plot(t,c(vi,vj,vk,1)*h(1)+c(vi,vj,vk,2)*h(2),'r.');
%	    plot(t,c(vi,vj,vk,1)*h(1),'k.');
%	    plot(t,T(vi,vj,vk),'c.');

%	    tcs{3}(t) = lastoff(vi,vj,vk);
%	    tcs{4}(t) = T(vi,vj,vk);
%	    tcs{5}(t) = reg(t);
	    
%
%            tcs{3}(t) = c(vi,vj,vk,1);
%            tcs{4}(t) = c(vi,vj,vk,1)*h(1);
%
%            tcs{5}(t) = c(vi,vj,vk,2);
%            tcs{6}(t) = c(vi,vj,vk,2)*h(2);
%	    
%	    tcs{7}(t) = C(L+1,L+1)/c(vi,vj,vk,L+2);
%	    tcs{8}(t) = sqrt(t-L-1);
%
%	    tcs{9}(t) = T(vi,vj,vk);
%	  end
	    
	    
	end
      end
    end

%    T = T-tsum/sum(mask(:));
    
    visualize = 1;
    if(visualize)
      thresh = abs(tinv(0.05/numvox,t-L-1));

      vis_vol(T,2,im);

      if(reg(t) > 0.5)
	title([num2str(t) ': on']);
      else
	title([num2str(t) ': off']);
      end
      pause(.5);
    end
 
    ps(:,:,:,t) = p;
    as(:,:,:,t) = a;
    Ts(:,:,:,t) = T;
    %fprintf('t=%d, mean T=%f\n',t,mean(T(:)));
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
