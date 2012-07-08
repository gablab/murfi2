MATLAB  = /software/matlab_versions/2010b/bin/

NAME 	=		infoclient

SRC	=		infoclientLib.cpp \
	                infoclientMex.cpp \
	                tinyxml/tinyxml.cpp \
	                tinyxml/tinyxmlerror.cpp \
	                tinyxml/tinyxmlparser.cpp 

INCL  	=		infoclientLib.h 

OBJDIR	= obj

################
IPATH   = 
OBJOPT  = 
EXEOPT  = -output $(NAME) -lACE
LPATH   = 
DEFS	= -DTIXML_USE_STL


#####################
CXX 	= $(MATLAB)mex
MAKE 	= make
SHELL	= /bin/sh
TMPOBJS = $(SRC:.cpp=.o) 
OBJS 	= $(TMPOBJS:%/=) 
RM 	= /bin/rm -f 	

CFLAGS  = -g $(OBJOPT) $(IPATH) $(DEFS)
LDFLAGS = $(EXEOPT) $(LPATH)

##############################

all:	$(NAME)
$(NAME): $(OBJS) $(SRC) $(INCL)  
	mv *.o $(OBJDIR)
	$(CXX) $(OBJDIR)/*.o $(LDFLAGS) 

clean:
	-$(RM) $(NAME) $(OBJS) *~

.cpp.o:
	$(CXX) $(CFLAGS) -c $< 
