# Executable name
EXEC = JeuDeLaVie

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 \
           -I. \
           -IComposants \
           -IInterface \
           -IService \
           -Iinclude

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lstdc++fs

# Directories
SRC_DIRS = . Composants Interface Service
OBJ_DIR = obj
BIN_DIR = bin

# Find all source files
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Generate object file names
OBJS = $(foreach src,$(SRCS),$(OBJ_DIR)/$(src:.cpp=.o))

# Default target
all: $(BIN_DIR)/$(EXEC)

# Create bin directory and link
$(BIN_DIR)/$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: all
	./$(BIN_DIR)/$(EXEC)

# Make these targets not require files
.PHONY: all clean run

# Print variables for debugging
$(info Sources: $(SRCS))
$(info Objects: $(OBJS))
