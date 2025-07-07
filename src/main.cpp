#include <iostream>
#include "Gerenciador.h"

using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_grafo.txt>" << endl;
        return 1;
    }

    try {
        Grafo* grafo = Grafo::lerGrafoDoArquivo(argv[1]);
        Gerenciador::comandos(grafo);
        delete grafo;
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}