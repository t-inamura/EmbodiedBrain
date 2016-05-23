# target file (.so)
TARGET1 = ./LinkageController.so
TARGET2 = ./LinkageObjectController.so

# sources
SRCS1 =\
	LinkageController.cpp \
	LinkageControllerOnInit.cpp \
	LinkageControllerOnAction.cpp \
	LinkageControllerOnRecvMsg.cpp \

SRCS2 =\
	LinkageObjectController.cpp \

# compiler
CXX = g++

# compile option
CFLAGS = -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIGVERSE_PATH)/include -Wl,-rpath=$(SIGVERSE_PATH)/lib -L$(SIGVERSE_PATH)/lib -lsigplugin -fPIC -shared

# generate intermediate object file from sources.
OBJS1 := $(SRCS1:.cpp=.o)
OBJS2 := $(SRCS2:.cpp=.o)

# -- compile rule --

all: $(TARGET1) $(TARGET2) 

# intermediate output file
%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

# target file(.so)
$(TARGET1): $(OBJS1) $(SRCS1)
	$(CXX) $(CFLAGS) -o $@ $(OBJS1)
#	rm -f $(OBJS1) # Remove intermediate files.

$(TARGET2): $(OBJS2) $(SRCS2)
	$(CXX) $(CFLAGS) -o $@ $(OBJS2)
#	rm -f $(OBJS2) # Remove intermediate files.

# remove object file
clean:
	rm -f $(TARGET1) $(OBJS1)
	rm -f $(TARGET2) $(OBJS2)
