# target file (.so)
TARGET1 = ./SenseOfSelfController.so
TARGET2 = ./SenseOfSelfController_lowerbody.so

# sources
SRCS1 = SenseOfSelfController.cpp \

SRCS2 = SenseOfSelfController_lowerbody.cpp \

# compiler
CXX = g++

# compile option
CFLAGS  = -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIGVERSE_PATH)/include  -fPIC -shared 
LDFLAGS = -Wl,-rpath=$(SIGVERSE_PATH)/lib -L$(SIGVERSE_PATH)/lib -lsigplugin

# generate intermediate object file from sources.
OBJS1 := $(SRCS1:.cpp=.o)
OBJS2 := $(SRCS2:.cpp=.o)

# -- compile rule --

all: $(TARGET1) $(TARGET2) 

# intermediate output file
%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

# target file(.so)
$(TARGET1): $(OBJS1)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#	rm -f $(OBJS1) # Remove intermediate files.

$(TARGET2): $(OBJS2)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


# remove object file
clean:
	rm -f $(TARGET1) $(OBJS1)
	rm -f $(TARGET2) $(OBJS2)
	