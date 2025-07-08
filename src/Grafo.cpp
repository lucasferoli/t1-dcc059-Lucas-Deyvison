#include "Grafo.h"
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <limits>



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
     
for (No* no : lista_adj) {
    if (no->id == id_no) {
        no_inicial = no;
        break;
    }
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

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b) {
    // Define um valor grande para representar "infinito" (ausência de caminho)
    const int MAX_INT = 999999999;
    int total_vertices = lista_adj.size();

    // Mapeia cada id de vértice (char) para um índice inteiro e vice-versa
    unordered_map<char, int> char_para_int;
    unordered_map<int, char> int_para_char;
    
    // Preenche os mapas de conversão entre char <-> int para os vértices
    for (int indice = 0; indice < total_vertices; indice++) {
        char_para_int[lista_adj[indice]->id] = indice;
        int_para_char[indice] = lista_adj[indice]->id;
    }

    // Verifica se os vértices de origem e destino existem no grafo
    if (!char_para_int.count(id_no_a) || !char_para_int.count(id_no_b)) {
        cout << "Erro: Vertice(s) invalido(s)!" << endl;
        return vector<char>();
    }

    // Cria as matrizes de distâncias e de próximos vértices para reconstrução do caminho
    vector<vector<int>> distancias(total_vertices, vector<int>(total_vertices, MAX_INT));
    vector<vector<int>> proximo_no(total_vertices, vector<int>(total_vertices, -1));

    // Inicializa as matrizes:
    // - distância para si mesmo é 0
    // - distância para vizinhos diretos é o peso da aresta
    // - registra o próximo nó para reconstrução do caminho
    for (int i = 0; i < total_vertices; i++) {
        distancias[i][i] = 0;
        for (auto& conexao : lista_adj[i]->arestas) {
            int j = char_para_int[conexao->id_no_alvo];
            distancias[i][j] = conexao->peso;
            proximo_no[i][j] = j;
        }
    }

    // Algoritmo de Floyd-Warshall:
    // Para cada possível vértice intermediário, verifica se passar por ele reduz a distância entre pares de vértices
    for (int intermediario = 0; intermediario < total_vertices; intermediario++) {
        for (int partida = 0; partida < total_vertices; partida++) {
            for (int chegada = 0; chegada < total_vertices; chegada++) {
                int novo_caminho = distancias[partida][intermediario] + distancias[intermediario][chegada];
                if (distancias[partida][intermediario] != MAX_INT &&
                    distancias[intermediario][chegada] != MAX_INT &&
                    novo_caminho < distancias[partida][chegada]) {
                    distancias[partida][chegada] = novo_caminho;
                    proximo_no[partida][chegada] = proximo_no[partida][intermediario];
                }
            }
        }
    }

    // Reconstrói o caminho mínimo do vértice de origem até o destino usando a matriz de próximos vértices
    int inicio = char_para_int[id_no_a];
    int fim = char_para_int[id_no_b];
    if (proximo_no[inicio][fim] == -1) {
        cout << "Caminho inexistente entre " << id_no_a << " e " << id_no_b << endl;
        return vector<char>();
    }

    vector<char> trajeto;
    int atual = inicio;
    while (atual != fim) {
        trajeto.push_back(int_para_char[atual]);
        atual = proximo_no[atual][fim];
    }
    trajeto.push_back(int_para_char[fim]);

    return trajeto;
}


Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo* Grafo::arvore_caminhamento_profundidade(char id_no, vector<pair<char, char>>& arestas_retorno) {
    // Verifica se o vértice existe
    No* no_inicial = nullptr;
    for (No* no : lista_adj) {
        if (no->id == id_no) {
            no_inicial = no;
            break;
        }
    }
    if (!no_inicial) {
        cout << "Erro: Vértice '" << id_no << "' não encontrado!" << endl;
        return nullptr;
    }

    // Cria um novo grafo para a árvore DFS
    Grafo* arvore_dfs = new Grafo();
    arvore_dfs->in_direcionado = this->in_direcionado;
    arvore_dfs->in_ponderado_aresta = this->in_ponderado_aresta;
    arvore_dfs->in_ponderado_vertice = this->in_ponderado_vertice;

    // Estruturas auxiliares
    unordered_map<char, bool> visitados;
    unordered_map<char, char> pai;

    arestas_retorno.clear(); // Limpa o vetor antes de usar

    // DFS iterativa com pilha
    stack<char> pilha;
    pilha.push(id_no);
    visitados[id_no] = true;
    arvore_dfs->lista_adj.push_back(new No{id_no, 0, {}});

    while (!pilha.empty()) {
        char atual = pilha.top();
        pilha.pop();

        // Encontra o nó atual na lista de adjacência
        No* no_atual = nullptr;
        for (No* no : lista_adj) {
            if (no->id == atual) {
                no_atual = no;
                break;
            }
        }

        // Processa vizinhos
        for (Aresta* aresta : no_atual->arestas) {
            char vizinho = aresta->id_no_alvo;

            if (!visitados[vizinho]) {
                // Aresta de árvore
                visitados[vizinho] = true;
                pai[vizinho] = atual;
                pilha.push(vizinho);

                // Adiciona à árvore DFS
                arvore_dfs->lista_adj.push_back(new No{vizinho, 0, {}});
                Aresta* nova_aresta = new Aresta{vizinho, aresta->peso};
                for (No* no : arvore_dfs->lista_adj) {
                    if (no->id == atual) {
                        no->arestas.push_back(nova_aresta);
                        break;
                    }
                }
            } else if (vizinho != pai[atual]) {
                // Aresta de retorno (para grafos não direcionados)
                arestas_retorno.push_back({atual, vizinho});
            }
        }
    }

    return arvore_dfs;
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