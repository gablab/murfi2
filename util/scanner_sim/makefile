OBJ_DIR=../../obj

default: servenii4d

servenii4d: servenii4d.cpp
	g++ -g -o servenii4d servenii4d.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -lniftiio -I/usr/include/nifti -D_REENTRANT

servescanner: servescanner.cpp
	g++ -g -o servescanner servescanner.cpp -lACE

servedicoms: servedicoms.cpp
	g++ -g -o servedicoms servedicoms.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -I/usr/include/dcmtk  -D_REENTRANT -DHAVE_CONFIG_H -ldcmdata -ldcmsr -ldcmdsig -ldcmnet -ldcmtls -ldcmimage -ldcmpstat -ldcmwlm -ldcmimgle -ldcmqrdb -lofstd -lpng12 -lssl3 -lssl -ltiff -lxml2 -lwrap 

servenii: servenii.cpp
	g++ -g -o servenii servenii.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -lniftiio -I/usr/include/nifti -D_REENTRANT -lX11

