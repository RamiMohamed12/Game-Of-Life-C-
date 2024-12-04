# Executable name
EXEC = JeuDeLaVie

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lstdc++fs

# Directories
SRC_DIRS = Composants Interface Service
OBJ_DIR = obj
BIN_DIR = bin

# Source and object files
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS = $(patsubst $(SRC_DIRS)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN_DIR)/$(EXEC)

# Link object files to create the executable
$(BIN_DIR)/$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIRS)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object and binary directories
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Clean all generated files
mrproper: clean
	rm -f $(EXEC)

# Run the program
run: all
	./$(BIN_DIR)/$(EXEC)
