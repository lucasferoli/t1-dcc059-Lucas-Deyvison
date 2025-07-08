#include "Aresta.h"

Aresta::Aresta() : id_no_alvo('\0'), peso(0) {
    // Inicialização padrão
}

Aresta::Aresta(char id_no_alvo, int peso) 
    : id_no_alvo(id_no_alvo), peso(peso) {}
    
Aresta::~Aresta() {
    // Destruidor vazio (não há alocações dinâmicas para liberar)
}