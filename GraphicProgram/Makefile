# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I. -Wno-switch
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR = .
OBJ_DIR = obj

# Executable
TARGET = GameOfLife

# Source files
SOURCES = $(SRC_DIR)/Main.cpp \
          $(SRC_DIR)/Game.cpp \
          $(SRC_DIR)/Grid.cpp \
          $(SRC_DIR)/Pattern.cpp \
          $(SRC_DIR)/AliveCell.cpp \
          $(SRC_DIR)/DeadCell.cpp \
          $(SRC_DIR)/Menu.cpp \
          $(SRC_DIR)/GridSizeSelector.cpp

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Create obj directory
$(shell mkdir -p $(OBJ_DIR))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(SFML_LIBS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild

