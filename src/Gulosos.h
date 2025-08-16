#ifndef GULOSOS_H
#define GULOSOS_H

#include "Grafo.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <unordered_set>


class Gulosos {
private:
    Grafo* g;
    std::unordered_map<char, int> idParaIndice; // Mapeia id -> índice no lista_adj
    std::vector<char> vizinhosDiretos(char v);

    
    void atualizarDominados(const std::vector<char>& D, std::vector<bool>& dominado);

    bool todosDominados(const std::vector<bool>& dominado);

public:
    Gulosos(Grafo* grafo);

    double tempoExecucaoGuloso2Dominating = 0.0;
    double tempoExecucaoGulosoRandomizado = 0.0;
    double tempoExecucaoGulosoRandomizadoReativo = 0.0;

    double getTempoExecucao() const { return tempoExecucaoGuloso2Dominating; }
    double getTempoExecucaoRandomizado() const { return tempoExecucaoGulosoRandomizado; }
    double getTempoExecucaoRandomizadoReativo() const { return tempoExecucaoGulosoRandomizadoReativo; }

    std::vector<char> guloso2Dominating();
    std::vector<char> gulosoRandomizado(double alpha);
    std::vector<char> gulosoRandomizadoReativo(int maxIter, const std::vector<double>& alphas, int bloco);
};

#endif