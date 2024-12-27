CXX := g++
CXX_FLAGS := -Wall -Wextra -g
LIB_FLAGS := -lglfw -lGL -lGLEW -ltinygltf

EXECUTABLE := bin/my_program
SRC_DIR := src
SRC_FILES := main.cpp files.cpp model.cpp
OBJ_FILES := $(addprefix obj/, $(SRC_FILES:.cpp=.o))
SRC_FILES := $(addprefix $(SRC_DIR)/, $(SRC_FILES))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) -o $@ $^ $(LIB_FLAGS)

obj/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@