# Nom de l'exécutable
TARGET = GameOfLife

# Fichiers source
SRC = Game.cpp Grid.cpp AliveCell.cpp DeadCell.cpp

# Fichiers objets générés
OBJ = $(SRC:.cpp=.o)

# Compilateur et options
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Règle par défaut pour construire l'exécutable
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# Compilation des fichiers objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle de nettoyage des fichiers générés
clean:
	rm -f $(OBJ) $(TARGET)

# Refaire les dépendances (optionnel)
rebuild: clean all

