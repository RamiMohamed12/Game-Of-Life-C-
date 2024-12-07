#include "gtest/gtest.h"
#include "../Service/FileHandler.h"
#include "../Composants/Grid.h"
#include "../Composants/Cell.h"
#include "../Composants/AliveCell.h"
#include "../Composants/DeadCell.h"
#include <memory>
#include <fstream>

// Helper function to create test input file
void createTestInputFile(const string& filename) {
    std::ofstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Could not create test input file";
    
    // Write a simple 5x5 grid pattern
    file << "5 5\n";  // dimensions
    file << ".....\n";
    file << "..*..\n";
    file << "..*..\n";
    file << "..*..\n";
    file << ".....\n";
    file.close();
}

class GrilleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files before each test
        createTestInputFile("test_input.txt");
        FileHandler::createDirectory("test_output");
    }

    void TearDown() override {
        // Clean up after each test
        std::remove("test_input.txt");
        FileHandler::clearDirectory("test_output");
        std::remove("test_output");
    }
};

// Test case for grid initialization
TEST_F(GrilleTest, GridInitialization) {
    // Create a 5x5 grid
    Grid grid(5, 5);
    
    // Load the test pattern
    grid.loadFromFile("test_input.txt");
    
    // Test if the grid has changed after loading
    EXPECT_TRUE(grid.hasChanged()) << "Grid should change after loading from file";
}

// Test for grid update mechanism
TEST_F(GrilleTest, GridUpdate) {
    Grid grid(5, 5);
    
    // Load initial state from test file
    grid.loadFromFile("test_input.txt");
    
    // Store initial state
    grid.saveToFile("test_output", 0, 5, 5);
    
    // Update the grid
    grid.update();
    
    // Save updated state
    grid.saveToFile("test_output", 1, 5, 5);
    
    // Verify that the grid has changed
    EXPECT_TRUE(grid.hasChanged()) << "Grid should change after update";
}

// Test for file operations
TEST_F(GrilleTest, FileOperations) {
    const string testDir = "test_output";
    
    // Test directory operations
    EXPECT_TRUE(FileHandler::directoryExists(testDir));
    
    // Create a grid and save it
    Grid grid(5, 5);
    grid.loadFromFile("test_input.txt");
    grid.saveToFile(testDir, 0, 5, 5);
    
    // Clean up
    FileHandler::clearDirectory(testDir);
    EXPECT_TRUE(FileHandler::directoryExists(testDir));
}

// Test for grid dimensions
TEST_F(GrilleTest, GridDimensions) {
    const string filename = "test_input.txt";
    int width = 0, height = 0;
    
    // Read dimensions from file
    FileHandler::readGridDimensions(filename, width, height);
    
    // Verify dimensions match what we created
    EXPECT_EQ(width, 5) << "Width should be 5";
    EXPECT_EQ(height, 5) << "Height should be 5";
    
    // Create grid with read dimensions
    Grid grid(width, height);
    
    // Load the grid
    grid.loadFromFile(filename);
    
    // Verify grid loaded successfully
    EXPECT_TRUE(grid.hasChanged()) << "Grid should change after loading";
}

// Test toroidal indexing
TEST_F(GrilleTest, ToroidalIndexing) {
    Grid grid(5, 5);
    
    // Test some toroidal wrapping
    EXPECT_EQ(grid.toroidalIndex(-1, 5), 4);
    EXPECT_EQ(grid.toroidalIndex(5, 5), 0);
    EXPECT_EQ(grid.toroidalIndex(2, 5), 2);
}

// Entry point for Google Test
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
