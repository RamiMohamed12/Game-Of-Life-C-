# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I. -IService -IInterface -IComposants
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Directories
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin
INTERFACE_DIR = Interface
COMPOSANTS_DIR = Composants
SERVICE_DIR = Service

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(INTERFACE_DIR)/*.cpp) \
       $(wildcard $(COMPOSANTS_DIR)/*.cpp) \
       $(wildcard $(SERVICE_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Binary name
TARGET = $(BIN_DIR)/game

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(INTERFACE_DIR)
	@mkdir -p $(BUILD_DIR)/$(COMPOSANTS_DIR)
	@mkdir -p $(BUILD_DIR)/$(SERVICE_DIR)
	@mkdir -p $(BIN_DIR)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run directories

# Dependencies
-include $(OBJS:.o=.d)
