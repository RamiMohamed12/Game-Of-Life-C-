# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I. -IService -IInterface -IComposants
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Project root directory (adjust this to your actual path)
PROJECT_ROOT = /home/ramimohamed/Github/Game-Of-Life-C-/GraphicProgram

# Directories
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin
INTERFACE_DIR = Interface
COMPOSANTS_DIR = Composants
SERVICE_DIR = Service
RESOURCE_DIR = $(PROJECT_ROOT)/assets/icons

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(INTERFACE_DIR)/*.cpp) \
       $(wildcard $(COMPOSANTS_DIR)/*.cpp) \
       $(wildcard $(SERVICE_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Binary name
TARGET = $(BIN_DIR)/game

# Icon paths (using your existing files)
ICON_SOURCE = $(RESOURCE_DIR)/GameOfLife.png
ICON_TARGET = $(RESOURCE_DIR)/logo.png

# Default target
all: directories setup-icon $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(INTERFACE_DIR)
	@mkdir -p $(BUILD_DIR)/$(COMPOSANTS_DIR)
	@mkdir -p $(BUILD_DIR)/$(SERVICE_DIR)
	@mkdir -p $(BIN_DIR)

# Setup icon for the application
setup-icon:
	@if [ ! -f "$(ICON_TARGET)" ]; then \
		echo "Setting up application icon..."; \
		cp "$(ICON_SOURCE)" "$(ICON_TARGET)"; \
	fi

# Link
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run 'make run' to start the game."

# Compile
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create desktop entry
install: all
	@echo "Installing Game of Life to application menu..."
	@mkdir -p ~/.local/share/applications
	@echo "[Desktop Entry]" > ~/.local/share/applications/game-of-life.desktop
	@echo "Version=1.0" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Type=Application" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Name=Game of Life" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Comment=Conway's Game of Life Implementation" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Exec=$(PROJECT_ROOT)/$(TARGET)" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Icon=$(ICON_SOURCE)" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Terminal=false" >> ~/.local/share/applications/game-of-life.desktop
	@echo "Categories=Game;Simulation;" >> ~/.local/share/applications/game-of-life.desktop
	@update-desktop-database ~/.local/share/applications
	@echo "Installation complete! You can now find Game of Life in your application menu."

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Clean everything including desktop entry
uninstall: clean
	@echo "Removing desktop entry..."
	rm -f ~/.local/share/applications/game-of-life.desktop
	update-desktop-database ~/.local/share/applications
	@echo "Uninstall complete!"

# Run the game
run: all
	@echo "Starting Game of Life..."
	./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  make       - Build the game"
	@echo "  make run   - Build and run the game"
	@echo "  make clean - Remove build files"
	@echo "  make install   - Install to application menu"
	@echo "  make uninstall - Remove from application menu"
	@echo "  make help      - Show this help message"

# Phony targets
.PHONY: all clean uninstall run directories setup-icon install help

# Dependencies
-include $(OBJS:.o=.d)
