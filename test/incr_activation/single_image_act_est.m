% f = single_image_act_est(X, ...
%			   nuisance_col_indices, ...
%			   Y [, ...
%			   roi_comb_method,...
%			   tp_for_err_est,...
%			   err_norm ])
%
% implementation of the single image activation estimation
% algorithm for testing and validation
%
% INPUTS
% X                    design matrix (time x basis) representing the model
% nuisance_col indices vector of indices of columns in X
%                      representing nuisance bases
% Y                    data matrix (time x voxel) representing the input data
% roi_comb_method      method for stat combination. ('mean')
%                      one of 'mean', 'median', 'weighted_ave'
% tp_for_err_est       timepoints over which to estimate error (20)
% err_norm             error norm for converting each timecourse
%                      into a stat ('L2')
%                      one of 'L1', 'L2', 'LINF'
%
% OUTPUTS
% f                    feedback

function f = single_image_act_est(X, ...
				  nuisance_col_indices, ...
				  Y, ...
				  roi_comb_method,...
				  tp_for_err_est,...
				  err_norm)

  %% validate inputs
  if(~(exist('X','var') && exist('Y','var')) || size(X,1) ~= size(Y,1))
    error(['X, Y, and nuisance_col_indices are required arguments,' ...
	   ' see help ' mfilename]);;
  end

  if(~exist('roi_comb_method','var'))
    roi_comb_method = 'mean';
  elseif(~(strcmp(roi_comb_method,'mean')
        || strcmp(roi_comb_method,'median'))
        || strcmp(roi_comb_method,'weighted_ave'))
    error(['invalid roi_comb_method,' ...
	   ' see help ' mfilename]);;
  end

  if(~exist('tp_for_err_est','var'))
    tp_for_err_est = 20;
  end

  if(~exist('err_norm','var'))
    err_norm = 'L2';
  elseif(~(strcmp(err_norm,'L1')
        || strcmp(err_norm,'L2'))
        || strcmp(err_norm,'LINF'))
    error(['invalid err_norm,' ...
	   ' see help ' mfilename]);;
  end

  % storage
  numt = size(X,1);
  numv = size(Y,2);

  tp_in_err_so_far = 0;
  err = zeros(numv,1);

  % for each timepoint
  f = zeros(numt,1);  % feedback
  for(t=1:numt)

    % fit each voxel
    vox_stats = zeros(numv,1);
    vox_res = zeros(numv,1);
    for(v=1:numv)

      % extract data
      Y_t = Y(1:t,v);
      X_t = X(1:t,:);

      % fit the model at this time
      [b bint r rint] = regress(Y_t, X_t);

      % save the residual for weighting (always L2)
      if(t > 1)
	vox_res(v) = sqrt(sum(r^2))/(t-1);
      end

      % get the residual based on the err_norm
      if(strcmp(err_norm,'L1'))
	if(t <= tp_for_err_est)
	  err(v) = err(v) + abs(r);
	  tp_in_err_so_far = tp_in_err_so_far + 1;
	end
	dev = err(v) / (tp_in_err_so_far-1);
      elseif(strcmp(err_norm,'LINF'))
	if(t <= tp_for_err_est)
	  err(v) = max([err(v) abs(r)]);
	  tp_in_err_so_far = tp_in_err_so_far + 1;
	end
	dev = err(v);
      else
	if(t <= tp_for_err_est)
	  err(v) = err(v) + r^2;
	  tp_in_err_so_far = tp_in_err_so_far + 1;
	end
	dev = sqrt(err(v)) / (tp_in_err_so_far-1);
      end

      % estimate neural portion of the signal
      neural = Y(t,v) - X(t,nuisance_col_indices) * b(nuisance_col_indices);

      % get the voxel stat
      vox_stats(v) = neural / dev;
    end

    % combine the the voxels
    if(strcmp(roi_comb_method,'mean'))
      f(t) = mean(vox_stats);
    elseif(strcmp(roi_comb_method,'median'))
      f(t) = median(vox_stats);
    elseif(strcmp(roi_comb_method,'weighted_ave'))
      f(t) = vox_stats.*vox_res ./ sum(vox_res);
    end

  end

return;

function usage

return
