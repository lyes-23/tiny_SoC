LIB_DIR = -L/usr/local/systemc-2.3.3/lib-linux64
INC_DIR = -I/usr/local/systemc-2.3.3/include -I../common/ -I../../include/

## Adding systemc library
LIBS   = -lsystemc $(EXTRA_LIBS)

CXXFLAGS  = -Wall -Wno-deprecated -g

TARGET_EXEC= decode_tb
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY : all clean

all:	$(TARGET_EXEC).exe 

$(TARGET_EXEC).exe:	$(OBJS) 
	$(CXX) $(CXXFLAGS) $(INC_DIR) $(LIB_DIR) -o $@ $(OBJS) $(LIBS) 2>&1 | c++filt 


.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@ 

.cc.o:
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@ 

clean::
	rm -f $(OBJS) *~ $(TARGET_EXEC).exe *.vcd 
	




