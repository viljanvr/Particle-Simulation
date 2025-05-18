INC_WINDOWS = include/windows
INC_LINUX   = include/linux
INC_COMMON  = include/common

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_S), Darwin)
	ifeq ($(UNAME_M), arm64) # Apple Silicon
		RM = rm -vf
		INCLUDE = $(INC_LINUX) -I/opt/homebrew/include
		#CXX_EXTRA_FLAGS = -lglut -lGLU -lGL -lpng
		CXX_EXTRA_FLAGS = -framework GLUT -framework OpenGL -L/opt/homebrew/lib -lpng
	else # Apple Intel
	endif
else ifeq ($(UNAME_M), Linux) # Linux
	RM = rm -vf
	INCLUDE = $(INC_LINUX)
	CXX_EXTRA_FLAGS = -lglut -lGLU -lGL -lpng
else ifeq ($(OS),Windows_NT) # Windows
	RM = rm -vf # might have to be adjusted to del for some Windows distros
	INCLUDE = $(INC_WINDOWS)
#	CXX_EXTRA_FLAGS = -Llib -lfreeglut -lglu32 -lopengl32 -lpng12
	CXX_EXTRA_FLAGS = -lfreeglut -lglu32 -lopengl32 -lpng
endif


#CXXFLAGS = --std=c++17 -g -O2 -Wall -Wno-sign-compare -Wno-deprecated-declarations -I$(INCLUDE) -I$(INC_COMMON) -DHAVE_CONFIG_H -DDEBUG
CXXFLAGS = --std=c++17 -g -O1 -Wall -Wno-sign-compare -Wno-deprecated-declarations -I$(INCLUDE) -I$(INC_COMMON) -DHAVE_CONFIG_H -DDEBUG
#CXXFLAGS += -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer
#CXX_EXTRA_FLAGS += -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer
CXX = g++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*cpp)
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SOURCES)))
EXECUTABLE = $(BIN_DIR)/project1.exe

all: $(OBJ_DIR) $(BIN_DIR) $(EXECUTABLE)

$(OBJ_DIR):
	mkdir -v $@

$(BIN_DIR):
	mkdir -v $@

# Special rule to suppress warnings from imageio.cpp
obj/imageio.o: $(SRC_DIR)/imageio.cpp
	$(CXX) -w $(CXXFLAGS) -o $@ -c $<

obj/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXX_EXTRA_FLAGS)

clean:
	@$(RM) $(OBJ_DIR)/*
	@$(RM) $(BIN_DIR)/*exe
