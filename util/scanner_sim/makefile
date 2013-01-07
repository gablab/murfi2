OBJ_DIR=../../obj
VXL_HOME = /usr
VXL_INC=-I$(VXL_HOME)/include/vxl/core -I$(VXL_HOME)/include/vxl/vcl
VXL_LIB=-lvnl_algo -lvcl -lv3p_netlib -lvnl -L$(VXL_HOME)/vxl/lib


default: servenii4d servenii



servenii4d: servenii4d.cpp
	g++ -g -o servenii4d servenii4d.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -lniftiio -I/usr/include/nifti $(VXL_INC) $(VXL_LIB) -D_REENTRANT

servescanner: servescanner.cpp
	g++ -g -o servescanner servescanner.cpp -lACE

servedicoms: servedicoms.cpp
	g++ -g -o servedicoms servedicoms.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -I/usr/include/dcmtk  -D_REENTRANT -DHAVE_CONFIG_H -ldcmdata -ldcmsr -ldcmdsig -ldcmnet -ldcmtls -ldcmimage -ldcmpstat -ldcmwlm -ldcmimgle -ldcmqrdb -lofstd -lpng12 -lssl3 -lssl -ltiff -lxml2 -lwrap

servenii: servenii.cpp
	g++ -g -o servenii servenii.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -lniftiio -I/usr/include/nifti -D_REENTRANT -lX11
