#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# Linux:
#   apt-get install freeglut3-dev
#

#CXX = g++
#CXX = clang++

SRC = ../src
RAYTRACING_DIR = $(SRC)/raytracing
GUI_SRCS := $(wildcard $(RAYTRACING_DIR)/gui/*cpp)
CONTROLLER_SRCS := $(wildcard $(RAYTRACING_DIR)/controller/*cpp)
MODEL_SRCS := $(wildcard $(RAYTRACING_DIR)/core/*.cpp)
MODEL_HITABLE_SRCS := $(wildcard $(RAYTRACING_DIR)/core/hittableObject/*.cpp)
UTILS_SRCS := $(wildcard $(RAYTRACING_DIR)/utils/*cpp)


EXE = MyApp
IMGUI_DIR = ../src/imgui/
SOURCES = $(SRC)/main.cpp $(GUI_SRCS) $(CONTROLLER_SRCS) $(MODEL_SRCS) $(UTILS_SRCS) \
	$(MODEL_HITABLE_SRCS)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glut.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp
OBJS = $(SOURCES:.cpp=.o)
UNAME_S := $(shell uname -s)

CXXFLAGS = -std=c++17 -fsanitize-recover=undefined -fopenmp -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(SRC)
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -lglut
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework GLUT
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32
ifeq ($(shell pkg-config freeglut --exists 2> /dev/null && echo yes || echo no),yes)
	CXXFLAGS += $(shell pkg-config freeglut --cflags)
	LIBS += $(shell pkg-config freeglut --libs)
else
	LIBS += -lglut
endif
	CFLAGS = $(CXXFLAGS)
endif
##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@.exe $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
