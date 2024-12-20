CXX = g++
CXXFLAGS = -O3 -std=c++20 -fPIC -shared -I/Users/dima/MIPT/SecondSem/MyPaint2.0 -I$(QT_PATH)/include -I$(QT_PATH)/include/QtWidgets -I$(QT_PATH)/include/QtCore -I$(QT_PATH)/include/QtGui
LDFLAGS = -L/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation -limplementation -lsfml-graphics -lsfml-window -lsfml-system -L$(QT_PATH)/lib -lQt6Widgets -lQt6Core -lQt6Gui

# Путь к библиотеке implementation.dylib
IMPLEMENTATION_PATH = /Users/dima/MIPT/SecondSem/MyPaint2.0/implementation
IMPLEMENTATION_DYLIB = $(IMPLEMENTATION_PATH)/libimplementation.dylib

# Путь к Qt
QT_PATH = /usr/local/opt/qt

PLUGINS = pencil sprayer eraser scrollbar_hor scrollbar_vert filters color_palette thickness opacity_bar files

# Сборка всех плагинов
all: $(PLUGINS:%=lib%.dylib)

# Сборка динамической библиотеки плагина
lib%.dylib: %.cpp $(IMPLEMENTATION_DYLIB)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
	@echo "Собран плагин $@"

# Очистка
clean:
	rm -f lib*.dylib
	@echo "Удалены все скомпилированные плагины"
