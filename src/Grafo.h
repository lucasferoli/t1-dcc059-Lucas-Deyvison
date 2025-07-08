//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>

using namespace std;

class Grafo {
public:
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(char id_no);                // a
    vector<char> fecho_transitivo_indireto(char id_no);              // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b);     // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos);        // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos);     // f
    int encontrar(vector<Subconjunto>& subconjuntos, int i);//f
    void unir(vector<Subconjunto>& subconjuntos, int x, int y); //f
    Grafo* arvore_caminhamento_profundidade(char id_no);             // g
    vector<int> dijkstra_distancias(char origem); //h
    int raio();                                                      // h 1
    int diametro();                                                  // h 2
    vector<char> centro();                                           // h 3
    vector<char> periferia();                                        // h 4

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;

    static Grafo* lerGrafoDoArquivo(const std::string& caminhoArquivo);

private:
    // Método auxiliar para processar linhas de arestas
    void adicionarAresta(char origem, char destino, int peso = 1);
    int indice_no(char id_no);
};

#endif // GRAFO_H
