#!/bin/bash

set -xe

libs="
/opt/fsl/lib/libfsl-newimage.so
/opt/fsl/lib/libfsl-miscmaths.so
/opt/fsl/lib/libfsl-NewNifti.so
/opt/fsl/lib/liblapack.so.3
/opt/fsl/lib/libblas.so.3
/opt/fsl/lib/libstdc++.so.6
/opt/fsl/lib/libgcc_s.so.1
/opt/fsl/lib/libfsl-utils.so
/opt/fsl/lib/libfsl-cprob.so
/opt/fsl/lib/libfsl-znz.so
/opt/fsl/lib/libgfortran.so.5
/opt/fsl/lib/libgomp.so.1
/opt/fsl/lib/libz.so.1
/opt/fsl/lib/libquadmath.so.0
"

for lib in $libs; do
    cp $lib /usr/lib/;
done

cp /opt/fsl/bin/flirt /usr/bin/

rm -rf /opt/fsl
