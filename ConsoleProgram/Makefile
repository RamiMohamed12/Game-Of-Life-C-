# Nom de l'exécutable
TARGET = GameOfLife

# Dossiers des fichiers source
SRC_DIRS = Composants Interface Service

# Extensions et variables
CXX = g++
CXXFLAGS = -Wall -std=c++17 -IComposants -IService -IInterface
LDFLAGS = -lstdc++fs

# Recherche automatique des fichiers source
SRC = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))

# Génération des fichiers objets
OBJ = $(SRC:.cpp=.o)

# Règle par défaut : construire l'exécutable
all: $(TARGET)

# Lien final de l'exécutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Compilation des fichiers objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ) $(TARGET)

# Reconstruction complète
rebuild: clean all

