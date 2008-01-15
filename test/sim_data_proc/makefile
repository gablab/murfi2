default: sim_data_proc

deps = sim_data_proc.cpp ../../obj/RtMRIImage.o ../../obj/RtData.o ../../obj/RtDataImage.o  ../../obj/RtActivation.o 

incs = -I../../src/executive -I../../src/util -I../../src/executive/tinyxml -DTIXML_USE_STL -I../../src/display  -I../../src/io -I../../src/data -I../../src/stream -I../../src/stream/analysis -I/usr/include/gsl -I/usr/include/dcmtk -I/usr/local/include/vxl/core -I/usr/local/include/vxl/vcl

libs = -lvnl -lvcl -lvnl_algo  -lACE -lgsl -lgslcblas -ldcmdata -ldcmsr -ldcmdsig -ldcmnet -ldcmtls -ldcmimage -ldcmpstat -ldcmwlm -ldcmimgle -ldcmqrdb -lofstd -lpng12 -lssl3 -lssl -ltiff -lxml2 -lwrap

sim_data_proc: testAccumCor.cpp $(deps)
	g++ -g -o sim_data_proc -DHAVE_CONFIG_H $(deps) $(incs) $(libs) 

