#ifndef GULOSOS_H
#define GULOSOS_H

#include "Grafo.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <random>

class Gulosos {
private:
    Grafo* g;
    std::unordered_map<char, int> idParaIndice; // Mapeia id -> índice no lista_adj

    // Busca vértices até distância 2 de um vértice v
    std::vector<char> coberturaAte2Passos(char v);

    // Marca como dominados
    void marcarDominados(std::vector<bool>& dominado, const std::vector<char>& vertices);

    // Checa se todos estão dominados
    bool todosDominados(const std::vector<bool>& dominado);

public:
    Gulosos(Grafo* grafo);

    std::vector<char> guloso2Dominating();
    std::vector<char> gulosoRandomizado(double alpha);
    std::vector<char> gulosoRandomizadoReativo(int maxIter, const std::vector<double>& alphas, int bloco);
};

#endif
