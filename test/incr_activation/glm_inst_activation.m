% Oliver Hinds <ohinds@mit.edu>
% 2007-11-28

function b = glm_inst_activation(vol, onoff, timeres)
  
  hrf = spm_hrf(2);
  
  % build regressors
  
  if(size(onoff,1) < size(onoff,2))
    onoff = onoff';
  end
  
  if(size(onoff,1) < size(vol,4))
    stim = repmat(onoff,1,ceil(size(vol,4)/size(onoff,1)));
    stim = stim(1:size(vol,4));
  else
    stim = onoff;
  end
  
  allblockreg = conv(stim,hrf);
  allblockreg = allblockreg(1:length(stim));
  
  numreg = sum(stim == 1)/timeres;
  if(numreg - floor(numreg) > eps)
    error('warning, the time resolution does not divide the stim length evenly. exit');
  end
  
  numdetrendreg = 1;
  reg = zeros(size(vol,4), numreg+numdetrendreg);

  % regs of interest
  curon = 1;
  ons = find(stim==1);
  for(i=1:numreg)
    reg(ons(curon:curon+timeres-1),i) = 1;
    curon = curon+timeres;
  end

  % detrend regs
  reg(:,numreg+1) = linspace(-1,1,size(reg,1));
  
  % reshape the volume
  siz = size(vol);
  vol = reshape(vol,[prod(size(vol))/size(vol,4) size(vol,4)])';
  
  % solve
  b = zeros(numreg+numdetrendreg+1,size(vol,2));
  for(i=1:size(vol,2))
    b(:,i) = glmfit(reg,vol(:,i));
  end

  b = b(1:numreg,:)';
  b = reshape(b,[siz(1:3) numreg]);
  
 keyboard
  
return

%************************************************************************%
%%% $Source$
%%% Local Variables:
%%% mode: Matlab
%%% fill-column: 76
%%% comment-column: 0
%%% End:
