#include "Pattern.h"

const std::map<std::string, Grid>& Patterns::getPatterns() {
    static const std::map<std::string, Grid> patterns = {
        {"Glider", {
            {0, 1, 0},
            {0, 0, 1},
            {1, 1, 1}
        }},
        {"Small Exploder", {
            {0, 1, 0},
            {1, 1, 1},
            {1, 0, 1}
        }},
        {"Spaceship", {
            {0, 1, 1, 1, 0},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0}
        }},
        {"Gosper Glider Gun", {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            // Ajoutez les autres lignes ici pour compléter le motif Gosper Glider Gun
        }}
    };
    return patterns;
}
