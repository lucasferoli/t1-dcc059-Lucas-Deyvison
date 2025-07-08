#include "Aresta.h"
#include "No.h"

Aresta::Aresta() : id_no_alvo('\0'), peso(0) {
    // Inicialização padrão
} 



Aresta::Aresta(int peso, No* destino) : peso(peso), destino(destino) {

}

Aresta::~Aresta() {
    // Destruidor vazio (não há alocações dinâmicas para liberar)
}