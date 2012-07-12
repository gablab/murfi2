MATLAB  = /software/matlab_versions/2010b/bin/

NAME 	=		infoclient

SRC	=		infoclientLib.cpp \
	                tinyxml/tinyxml.cpp \
	                tinyxml/tinyxmlerror.cpp \
	                tinyxml/tinyxmlparser.cpp 

INCL  	=		infoclientLib.h 

OBJDIR	= obj

################
IPATH   = 
OBJOPT  = 
EXEOPT  = -lACE
LPATH   = 
DEFS	= -DTIXML_USE_STL


#####################
CC      = g++
MEX 	= $(MATLAB)mex
MAKE 	= make
SHELL	= /bin/sh
TMPOBJS = $(SRC:.cpp=.o) 
OBJS 	= $(TMPOBJS:%/=) 
RM 	= /bin/rm -f 	

CFLAGS  = -g $(OBJOPT) $(IPATH) $(DEFS) -fPIC
LDFLAGS = $(EXEOPT) $(LPATH)

##############################

all:	mex python $(NAME)

$(NAME): $(OBJS) $(SRC) $(INCL)
	$(CC) -o $(NAME) test_infoclient.cpp $(OBJDIR)/*.o $(LDFLAGS)

mex: $(OBJS)
	$(MEX) infoclientMex.cpp $(OBJDIR)/*.o $(LDFLAGS)

python: $(OBJS)
	ar rcs lib$(NAME).a $(OBJDIR)/infoclientLib.o $(OBJDIR)/tinyxml*.o
	python setup.py build_ext -i

clean:
	-$(RM) $(NAME) $(OBJS) $(OBJDIR)/*.o *.so *.mexa64 *~ *.a

.cpp.o:
	$(CC) $(CFLAGS) -c $<
	mv *.o $(OBJDIR)
