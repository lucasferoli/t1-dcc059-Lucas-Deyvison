#include "Grafo.h"
#include <set>
#include <stack>

Grafo::Grafo() {
}

Grafo::~Grafo() {
}




vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    set<char> visitados;
    stack<No*> pilha;
    vector<char> resultado;


     No* no_inicial = nullptr;
     
cout << "Buscando nó: " << id_no << endl;
for (No* no : lista_adj) {
    cout << "Nó na lista: " << no->id << endl;
    if (no->id == id_no) {
        no_inicial = no;
        break;
    }
}

cout << "Nos presentes no grafo:" << endl;

for (No* no : lista_adj) {
    cout << " - " << no->id << endl;
}


    if (!no_inicial) {
        cout << "Nó não encontrado: " << id_no << endl;
        return {};
    }

    pilha.push(no_inicial);
    visitados.insert(id_no);

    while (!pilha.empty()) {
        No* atual = pilha.top();
        pilha.pop();

        for (Aresta* aresta : atual->arestas) {
            char id_vizinho = aresta->id_no_alvo;

            if (visitados.find(id_vizinho) == visitados.end()) {
                // Procurar o nó correspondente na lista de adjacência
                No* vizinho = nullptr;
                for (No* no : lista_adj) {
                    if (no->id == id_vizinho) {
                        vizinho = no;
                        break;
                    }
                }

                if (vizinho) {
                    visitados.insert(id_vizinho);
                    pilha.push(vizinho);
                    resultado.push_back(id_vizinho);
                }
            }
        }
    }

    return resultado;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}