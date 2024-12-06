# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# SFML libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Directories
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin

# Source files and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Target executable
TARGET = $(BIN_DIR)/game

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

# Compiling source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the program
run: all
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g
debug: clean all
