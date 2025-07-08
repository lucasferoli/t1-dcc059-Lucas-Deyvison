#include "No.h"

No::No() : id('\0'), peso(0) {
    // Inicialização padrão
}

No::No(char id, int peso, vector<Aresta*> arestas) 
    : id(id), peso(peso), arestas(arestas) {}

No::~No() {
    // Liberar memória das arestas
    for (Aresta* aresta : arestas) {
        delete aresta;
    }
}