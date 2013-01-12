% unmosaic a volume (c++ style)
%
% Oliver Hinds <ohinds@mit.edu>
% 2008-03-20

function umvol = unmosaic(mvol,matrixSize,numSlices)
  dim = size(mvol);
  umvol = zeros(matrixSize^2*numSlices,1);
  fp = fopen('/tmp/sameimg.asc','w');
  for(i=0:length(mvol(:))-1)
    slc = floor(mod(i,dim(1))/matrixSize) + floor(i/(matrixSize*dim(1))) * ...
        floor(dim(1)/matrixSize);
    row = mod(floor(i/dim(1)),matrixSize);
    col = mod(i,matrixSize);

    newind = slc*matrixSize^2 + row*matrixSize + col+1;

    fprintf(fp,'%d %d %d %d %d %d\n', mvol(i+1), newind, i, slc, row, col);

    %umvol(col+1,row+1,slc+1) = mvol(i+1);
    umvol(newind) = mvol(i+1);
  end
return
