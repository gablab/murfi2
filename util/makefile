all: thresholdnifti threshold2masknifti flipnifti intersectniftimask unmosaic_nifti mosaic_nifti
clean: 
	rm *.o thresholdnifti threshold2masknifti flipnifti

thresholdnifti: thresholdnifti.cpp
	g++ -g -o thresholdnifti thresholdnifti.cpp -lniftiio -I/usr/include/nifti

threshold2masknifti: threshold2masknifti.cpp
	g++ -g -o threshold2masknifti threshold2masknifti.cpp -lniftiio -I/usr/include/nifti

flipnifti: flipnifti.cpp
	g++ -g -o flipnifti flipnifti.cpp -lniftiio -I/usr/include/nifti

unmosaic_nifti: unmosaic_nifti.cpp
	g++ -g -o unmosaic_nifti unmosaic_nifti.cpp -lniftiio -I/usr/include/nifti

mosaic_nifti: mosaic_nifti.cpp
	g++ -g -o mosaic_nifti mosaic_nifti.cpp -lniftiio -I/usr/include/nifti

intersectniftimask: intersectniftimask.cpp
	g++ -g -o intersectniftimask intersectniftimask.cpp -lniftiio -I/usr/include/nifti

make_design_matrix: make_design_matrix.cpp
	g++ -g -o make_design_matrix make_design_matrix.cpp -lvnl -lvnl_algo -I/usr/local/include/vxl/core -I/usr/local/include/vxl/vcl

make_stim_design: make_stim_design.cpp
	g++ -g -o make_stim_design make_stim_design.cpp -lvnl -I/usr/local/include/vxl/core -I/usr/local/include/vxl/vcl