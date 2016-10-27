# target file (.so)
TARGET = ./MotionSwitchController.so

# sources
SRCS = MotionSwitchController.cpp \

# compiler
CXX = g++

# compile option
CFLAGS  = -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIGVERSE_PATH)/include  -fPIC -shared 
LDFLAGS = -Wl,-rpath=$(SIGVERSE_PATH)/lib -L$(SIGVERSE_PATH)/lib -lsigplugin

# generate intermediate object file from sources.
OBJS := $(SRCS:.cpp=.o)

# -- compile rule --
all: $(TARGET)

# intermediate output file
%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

# target file(.so)
$(TARGET): $(OBJS) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJS) # Remove intermediate files.

# remove object file
clean:
	rm -f $(TARGET) $(OBJS)
