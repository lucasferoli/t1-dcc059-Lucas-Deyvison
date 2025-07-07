#include "Grafo.h"
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>

Grafo::Grafo() {
}

Grafo::~Grafo() {
}

Grafo* Grafo::lerGrafoDoArquivo(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo");
    }

    Grafo* grafo = new Grafo();
    std::string linha;
    
    // 1a Linha: Flags do grafo (direcionado, ponderado arestas, ponderado vertices)
    std::getline(arquivo, linha);
    std::istringstream flagsStream(linha);
    flagsStream >> grafo->in_direcionado >> grafo->in_ponderado_vertice >> grafo->in_ponderado_aresta;

    // 2a Linha: Numero de vertices
    std::getline(arquivo, linha);
    grafo->ordem = std::stoi(linha);

    // Linhas seguintes: Vertices (IDs)
    for (int i = 0; i < grafo->ordem; ++i) {
        std::getline(arquivo, linha);
        No* novoNo = new No();
        novoNo->id = linha[0];  // Assume 1 char por vertice
        novoNo->peso = 0;       // Inicializa sem peso
        grafo->lista_adj.push_back(novoNo);
    }

    // Restante: Arestas (origem destino [peso])
    while (std::getline(arquivo, linha)) {
        std::istringstream arestaStream(linha);
        char origem, destino;
        int peso = 1;
        
        arestaStream >> origem >> destino;
        if (grafo->in_ponderado_aresta) {
            arestaStream >> peso;
        }
        
        grafo->adicionarAresta(origem, destino, peso);
    }

    return grafo;
}

void Grafo::adicionarAresta(char origem, char destino, int peso) {
    Aresta* novaAresta = new Aresta();
    novaAresta->id_no_alvo = destino;
    novaAresta->peso = peso;

    // Encontra no de origem e adiciona aresta
    for (No* no : lista_adj) {
        if (no->id == origem) {
            no->arestas.push_back(novaAresta);
            break;
        }
    }

    // Se grafo nao-direcionado, adiciona aresta inversa
    if (!in_direcionado) {
        Aresta* arestaInversa = new Aresta();
        arestaInversa->id_no_alvo = origem;
        arestaInversa->peso = peso;
        
        for (No* no : lista_adj) {
            if (no->id == destino) {
                no->arestas.push_back(arestaInversa);
                break;
            }
        }
    }
}


// Transitivo Direto
vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    set<char> visitados;
    stack<No*> pilha;
    vector<char> resultado;


     No* no_inicial = nullptr;
     
cout << "Buscando no: " << id_no << endl;
for (No* no : lista_adj) {
    cout << "No na lista: " << no->id << endl;
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
        cout << "No nao encontrado: " << id_no << endl;
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
                // Procurar o no correspondente na lista de adjacencia
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
    set<char> visitados;
    stack<char> pilha;
    vector<char> resultado;

    // Verifica se o vertice existe
    bool existe = false;
    for (No* no : lista_adj) {
        if (no->id == id_no) {
            existe = true;
            break;
        }
    }
    if (!existe) {
        cout << "Erro: Vertice '" << id_no << "' nao encontrado!" << endl;
        return {};
    }

    // DFS invertido (vizinhos que apontam para o vertice)
    pilha.push(id_no);
    visitados.insert(id_no);

    while (!pilha.empty()) {
        char atual = pilha.top();
        pilha.pop();

        // Percorre todos os nos para encontrar arestas que chegam no 'atual'
        for (No* no : lista_adj) {
            for (Aresta* aresta : no->arestas) {
                if (aresta->id_no_alvo == atual) {  // Se o no atual e alvo da aresta
                    if (visitados.find(no->id) == visitados.end()) {
                        visitados.insert(no->id);
                        pilha.push(no->id);
                        resultado.push_back(no->id);
                    }
                }
            }
        }
    }

    return resultado;
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