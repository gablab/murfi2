% Oliver Hinds <ohinds@mit.edu>
% 2007-11-28

function [ps as Ts] = accum_cor(vol, onoff)

  % hack to make scale same as dicom
    vol = vol.*49;
  
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
  s(:,2) = 1:size(reg,2);

  % build mask
  firstvol = squeeze(vol(:,:,:,1));
  mask = zeros(imsiz);

%  maskThresh = 0.3*mean(firstvol(:));
  maskThresh = 1681.38;
  mask(find(firstvol > maskThresh)) = 1;
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

  % initialize C,c,p,a
  C = [10^-7*eye(L+1) zeros(L+1,1)]; % choleski rows that dont change per vox
  c = zeros([imsiz L+2]);
  c(:,:,:,end) = 10^-7; % row that does change
  p = zeros(imsiz);
  a = zeros(imsiz);
  T = zeros(imsiz);

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

%    keyboard

    % update p,a in each voxel
    im = vol(:,:,:,t);
    count = -1;
    for(vj=1:size(im,2))
      for(vk=1:size(im,3))
	for(vi=1:size(im,1))
	  count = count+1;

%	  if(mask(vi,vj,vk) == 0)
%	    continue;
%	  end

%	  keyboard
	  
	  z_hat = im(vi,vj,vk);

	  for(j=1:L+1)
	    z_hat = z_hat - h(j)*c(vi,vj,vk,j);
	    c(vi,vj,vk,j) = f(j)*c(vi,vj,vk,j) + g(j)*z_hat;
	  end

	  c(vi,vj,vk,L+2) = sqrt(c(vi,vj,vk,L+2)^2+(z_hat/b_old)^2);
	  p(vi,vj,vk) = c(vi,vj,vk,L+1)/sqrt(c(vi,vj,vk,L+2)^2+c(vi,vj,vk,L+1)^2);
	  a(vi,vj,vk) = c(vi,vj,vk,L+1)/C(L+1,L+1);

	  if(t > L+1)
	    T(vi,vj,vk) = sqrt(t-L-1) * c(vi,vj,vk,L+1)/c(vi,vj,vk,L+2);
	  end
	  
	  if(t == 2)
	    fprintf('%d %d %f %f\n', count, im(vi,vj,vk), z_hat, T(vi,vj,vk));
	    z
	    f
	    g
	    h
	    C
	    squeeze(c(vi,vj,vk,:))
	    keyboard
	  end
	end
      end
    end
%    keyboard

    visualize = 0;
    if(visualize)

      % find corrected threshold
      thresh = abs(tinv(0.05/numvox,t-L-1));

      vis_vol(T,thresh,im);

      if(reg(t) > 0.5)
	title([num2str(t) ': on']);
      else
	title([num2str(t) ': off']);
      end
      pause(.1);
    end

    ps(:,:,:,t) = p;
    as(:,:,:,t) = a;
    Ts(:,:,:,t) = T;
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
