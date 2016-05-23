# target file (.so)
TARGET1 = ./HeadController.so 
TARGET2 = ./MirrorTherapyController.so 
TARGET3 = ./LinkageController.so
TARGET4 = ./LinkageObjectController.so
TARGET5 = ./SenseOfSelfController.so
TARGET6 = ./SenseOfSelfController_lowerbody.so
TARGET7 = ./DummyController.so

# sources
SRCS1 = HeadController.cpp 
SRCS2 = MirrorTherapyController.cpp 
SRCS3 =\
	LinkageController.cpp \
	LinkageControllerOnInit.cpp \
	LinkageControllerOnAction.cpp \
	LinkageControllerOnRecvMsg.cpp 
SRCS4 = LinkageObjectController.cpp 
SRCS5 = SenseOfSelfController.cpp 
SRCS6 = SenseOfSelfController_lowerbody.cpp 
SRCS7 = DummyController.cpp


# compiler
CXX = g++

# compile option
CFLAGS  = -Wall -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIGVERSE_PATH)/include  -fPIC -shared 
LDFLAGS = -Wl,-rpath=$(SIGVERSE_PATH)/lib -L$(SIGVERSE_PATH)/lib -lsigplugin

# generate intermediate object file from sources.
OBJS1 := $(SRCS1:.cpp=.o)
OBJS2 := $(SRCS2:.cpp=.o)
OBJS3 := $(SRCS3:.cpp=.o)
OBJS4 := $(SRCS4:.cpp=.o)
OBJS5 := $(SRCS5:.cpp=.o)
OBJS6 := $(SRCS6:.cpp=.o)
OBJS7 := $(SRCS7:.cpp=.o)

# -- compile rule --
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7)

# intermediate output file
%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

# target file(.so)
$(TARGET1): $(OBJS1) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS1) # Remove intermediate files.

$(TARGET2): $(OBJS2) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS2) # Remove intermediate files.

$(TARGET3): $(OBJS3) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS3) # Remove intermediate files.

$(TARGET4): $(OBJS4) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS4) # Remove intermediate files.

$(TARGET5): $(OBJS5) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS5) # Remove intermediate files.

$(TARGET6): $(OBJS6) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS6) # Remove intermediate files.

$(TARGET7): $(OBJS7) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS7) # Remove intermediate files.

# remove object file
clean:
	rm -f $(TARGET1) $(OBJS1)
	rm -f $(TARGET2) $(OBJS2)
	rm -f $(TARGET3) $(OBJS3)
	rm -f $(TARGET4) $(OBJS4)
	rm -f $(TARGET5) $(OBJS5)
	rm -f $(TARGET6) $(OBJS6)
	rm -f $(TARGET7) $(OBJS7)
	
