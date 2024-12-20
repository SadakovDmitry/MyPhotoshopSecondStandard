CXX = g++ -std=c++20
CXXFLAGS = -O3 -std=c++20 -fsanitize=address -fPIC -I/usr/local/include
LDFLAGS = -L/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation -limplementation -lsfml-graphics -lsfml-window -lsfml-system

SRC_PATH = /Users/dima/MIPT/SecondSem/MyPaint2.0
PLUGIN_PATH = $(SRC_PATH)/plugins
IMPLEMENTATION_PATH = $(SRC_PATH)/implementation

PLUGINS = pencil sprayer eraser scrollbar_hor scrollbar_vert filters color_palette thickness opacity_bar files
PLUGIN_SRCS = $(PLUGINS:%=$(PLUGIN_PATH)/%.cpp)
PLUGIN_DYLIBS = $(PLUGINS:%=$(PLUGIN_PATH)/lib%.dylib)

TARGET = ps
MAIN_SRCS = $(SRC_PATH)/main.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

IMPLEMENTATION_DYLIB = $(IMPLEMENTATION_PATH)/libimplementation.dylib
IMPLEMENTATION_SRCS = $(IMPLEMENTATION_PATH)/photoshop.cpp $(IMPLEMENTATION_PATH)/my_sfm.cpp \
                      $(IMPLEMENTATION_PATH)/canvas.cpp $(IMPLEMENTATION_PATH)/plugin_example.cpp \
                      $(IMPLEMENTATION_PATH)/toolbar.cpp $(IMPLEMENTATION_PATH)/menubar.cpp \
                      $(IMPLEMENTATION_PATH)/optionsbar.cpp $(SRC_PATH)/Standard/api_system.cpp \
                      $(IMPLEMENTATION_PATH)/actions.cpp $(IMPLEMENTATION_PATH)/buttons.cpp

# Сборка всего
all: $(IMPLEMENTATION_DYLIB) $(PLUGIN_DYLIBS) $(TARGET)

# Сборка главного исполняемого файла
$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJS) -o $@ $(LDFLAGS)
	@echo "Собран главный исполняемый файл $(TARGET)"
	@./$(TARGET)

# Сборка динамической библиотеки implementation
$(IMPLEMENTATION_DYLIB): $(IMPLEMENTATION_SRCS)
	$(CXX) -shared -o $@ $^ -std=c++20 -fPIC -lsfml-graphics -lsfml-window -lsfml-system
	@echo "Собрана библиотека $(IMPLEMENTATION_DYLIB)"

# Сборка каждого плагина как динамической библиотеки
$(PLUGIN_PATH)/lib%.dylib: $(PLUGIN_PATH)/%.cpp $(IMPLEMENTATION_DYLIB)
	$(CXX) -shared -o $@ $< $(LDFLAGS)
	@echo "Собран плагин $@"

# Правило для объектных файлов
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(MAIN_OBJS) $(PLUGIN_PATH)/lib*.dylib $(TARGET)
	rm -f $(IMPLEMENTATION_DYLIB)
	@echo "Очистка завершена"
