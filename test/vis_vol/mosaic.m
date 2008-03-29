% mosaic a volume (c++ style)
%
% Oliver Hinds <ohinds@mit.edu>
% 2008-03-20

function [mvol m2dvol] = mosaic(vol, matrixSize, numSlices)
  dim = sqrt(matrixSize^2*ceil(sqrt(numSlices))^2);  
  nr = floor(dim/matrixSize);
  nc = floor(dim/matrixSize);
  
  mvol = zeros(matrixSize^2*numSlices,1);
  fp = fopen('/tmp/sameimg.asc','w');
  for(i=0:length(mvol(:))-1)
    row = mod(floor(i/matrixSize),matrixSize) + matrixSize*floor(i/(dim*matrixSize));
    col = mod(matrixSize*floor(i/matrixSize^2) + mod(i,matrixSize),dim);
    
    newind = row*dim + col+1;
    fprintf(fp,'%d %d %d %d %d\n', mvol(i+1), newind, i, row, col);
    
    %umvol(col+1,row+1,slc+1) = mvol(i+1);
    mvol(newind) = vol(i+1);
    m2dvol(row+1,col+1) = vol(i+1);
  
%    imagesc(m2dvol);
%    pause(0.1);
  
  end
return

