CXX = g++
CXXFLAGS = -O3 -std=c++20 -shared -I$(SRC_PATH)
LDFLAGS = -L$(SRC_PATH) -limplementation -lsfml-graphics -lsfml-window -lsfml-system
SRC_PATH = /Users/dima/MIPT/SecondSem/MyPaint2.0/implementation
COMMON_SRCS = #$(SRC_PATH)/api_sfm.cpp $(SRC_PATH)/api_system.cpp
PLUGINS = pencil sprayer eraser scrollbar_hor scrollbar_vert

all: $(PLUGINS:%=%.dylib)

%.dylib: %.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $(COMMON_SRCS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.dylib
