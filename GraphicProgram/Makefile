CXX = g++
CXXFLAGS = -Wall -std=c++17
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SOURCES = Main.cpp Game.cpp Grid.cpp Pattern.cpp
EXECUTABLE = GameOfLife

all: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SOURCES) $(SFML_LIBS)

clean:
	rm -f $(EXECUTABLE)
