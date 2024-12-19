CXX = g++
CXXFLAGS = -O3 -std=c++20 -fsanitize=address -I/usr/local/include
LDFLAGS = -L/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation -limplementation -lsfml-graphics -lsfml-window -lsfml-system

TARGET = ps
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS) libimplementation.dylib
	$(CXX) -shared -o libimplementation.dylib photoshop.cpp my_sfm.cpp canvas.cpp plugin_example.cpp toolbar.cpp menubar.cpp optionsbar.cpp /Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_system.cpp actions.cpp buttons.cpp -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@./$(TARGET)
	# @rm -f $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)
	rm libimplementation.dylib

libimplementation.dylib:
	$(CXX) -shared -o libimplementation.dylib photoshop.cpp my_sfm.cpp canvas.cpp plugin_example.cpp toolbar.cpp menubar.cpp optionsbar.cpp /Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_system.cpp actions.cpp buttons.cpp -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system

dylib: libimplementation.dylib
