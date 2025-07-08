#include "Grafo.h"
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <limits>

struct Subconjunto {
    int pai;
    int rank;
};




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
    vector<char> caminho;

    int inicio = indice_no(id_no_a);
    int destino = indice_no(id_no_b);


    if(inicio == -1 || destino == -1){
        cout << "Não foi possivel verificar. Um ou ambos os vertices nao existem." << endl;
        return caminho;
    }
    const int INF = 1000;
    int aux = lista_adj.size();

    vector<int> distancia(aux, INF);
    vector<int> anterior(aux, -1);
    vector<bool> jaVisitado(aux, false);

    distancia[inicio] = 0;


    for(int i = 0; i < aux; i++){
        int noNaoVisitado = -1;
        int menorDistancia = INF;

        for(int j = 0; j< aux; j++){
            if(!jaVisitado[j] && distancia[j] < menorDistancia){
                menorDistancia = distancia[j];
                noNaoVisitado = j;
            }
        }

        if (noNaoVisitado == -1){
            break;
        }

        jaVisitado[noNaoVisitado] = true;


        for (Aresta* aresta : lista_adj[noNaoVisitado]->arestas){
            int vizinho = indice_no(aresta->destino->id);
            int peso = aresta->peso;

            if(!jaVisitado[vizinho] && distancia[noNaoVisitado] + peso < distancia[vizinho]){
                distancia[vizinho] = distancia[noNaoVisitado] + peso;
                anterior[vizinho] = noNaoVisitado; 
            }
        }

        if(distancia[destino] == INF){
            cout << "Não há caminho para este nó"<< endl;
            return caminho;
        }

        for (int at = destino; at!= -1; at = anterior[at]){
            caminho.push_back(lista_adj[at]->id);
        }
        reverse(caminho.begin(), caminho.end());
        return caminho;
}
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
    if(ids_nos.empty()) return nullptr;

    Grafo* mst = new Grafo();
    mst->in_direcionado = false;
    mst->in_ponderado_aresta = true;
    mst->ordem = ids_nos.size();

    for(char id : ids_nos){
        mst->lista_adj.push_back(new No(id));
    }

    const int INF = 1000;
    vector<bool> visitado(ids_nos.size(), false);
    vector<int> chave(ids_nos.size(), INF);
    vector<int> pai(ids_nos.size(), -1);

    chave[0] = 0;

    for(int aux = 0; aux < (int)ids_nos.size() - 1; ++aux){
        int naoVisitado = -1;
        int chave_minima = INF;
        for(int i = 0; i < (int)ids_nos.size(); i++){
            if(!visitado[i] && chave[i] < chave_minima){
                chave_minima = chave[i];
                naoVisitado = i;
            }
        }
        if(naoVisitado == -1) break;
        visitado[naoVisitado] = true;
        int indiceNV = indice_no(ids_nos[naoVisitado]);
        No* no_NV = lista_adj[indiceNV];

        for(auto* aresta : no_NV->arestas){
            char v_id = aresta->destino->id;
            int v_sub_idx = -1;
            for (int j = 0; j < (int)ids_nos.size(); ++j) {
                if (ids_nos[j] == v_id) {
                    v_sub_idx = j;
                    break;
                }
            }
            if (v_sub_idx == -1 || visitado[v_sub_idx]) continue;
            if (aresta->peso < chave[v_sub_idx]) {
                chave[v_sub_idx] = aresta->peso;
                pai[v_sub_idx] = naoVisitado;
            }

        }
    }

    for (int v = 1; v < (int)ids_nos.size(); ++v) {
        int u = pai[v];
        if (u != -1) {
            No* no_u_mst = mst->lista_adj[u];
            No* no_v_mst = mst->lista_adj[v];
            int peso = chave[v];
            no_u_mst->arestas.push_back(new Aresta(peso, no_v_mst));
            no_v_mst->arestas.push_back(new Aresta(peso, no_u_mst));
        }
    }
    return mst;

    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}


//Faz parte da Kruskal

int Grafo::encontrar(vector<Subconjunto>& subconjuntos, int i) {
    if (subconjuntos[i].pai != i)
        subconjuntos[i].pai = encontrar(subconjuntos, subconjuntos[i].pai);
    return subconjuntos[i].pai;
}

void Grafo::unir(vector<Subconjunto>& subconjuntos, int x, int y) {
    int raiz_x = encontrar(subconjuntos, x);
    int raiz_y = encontrar(subconjuntos, y);

    if (subconjuntos[raiz_x].rank < subconjuntos[raiz_y].rank) {
        subconjuntos[raiz_x].pai = raiz_y;
    } else if (subconjuntos[raiz_x].rank > subconjuntos[raiz_y].rank) {
        subconjuntos[raiz_y].pai = raiz_x;
    } else {
        subconjuntos[raiz_y].pai = raiz_x;
        subconjuntos[raiz_x].rank++;
    }
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    Grafo* mst = new Grafo();
    mst->in_direcionado = false;
    mst->in_ponderado_aresta = true;
    mst->ordem = ids_nos.size();
    for (char id : ids_nos) {
        mst->lista_adj.push_back(new No(id));
    }
    unordered_map<char, int> id_para_indice;
    for (int i = 0; i < ids_nos.size(); ++i) {
        id_para_indice[ids_nos[i]] = i;
    }

    struct ArestaInfo {
        int origem;
        int destino;
        int peso;
    };
    vector<ArestaInfo> arestas;

    for (No* no : lista_adj) {
        if (id_para_indice.find(no->id) == id_para_indice.end()) continue;

        for (Aresta* aresta : no->arestas) {
            if (id_para_indice.find(aresta->destino->id) == id_para_indice.end()) continue;

            int u = id_para_indice[no->id];
            int v = id_para_indice[aresta->destino->id];
            if (u < v) {  // evita duplicatas em grafo não-direcionado
                arestas.push_back({u, v, aresta->peso});
            }
        }
    }
    sort(arestas.begin(), arestas.end(),
         [](const ArestaInfo& a, const ArestaInfo& b) {
             return a.peso < b.peso;
         });

    vector<Subconjunto> subconjuntos(ids_nos.size());
    for (int i = 0; i < ids_nos.size(); ++i) {
        subconjuntos[i].pai = i;
        subconjuntos[i].rank = 0;
    }

    for (const ArestaInfo& aresta : arestas) {
        int raiz_u = encontrar(subconjuntos, aresta.origem);
        int raiz_v = encontrar(subconjuntos, aresta.destino);

        if (raiz_u != raiz_v) {
            No* u = mst->lista_adj[aresta.origem];
            No* v = mst->lista_adj[aresta.destino];
            u->arestas.push_back(new Aresta(aresta.peso, v));
            v->arestas.push_back(new Aresta(aresta.peso, u));

            unir(subconjuntos, raiz_u, raiz_v);
        }
    }

    return mst;
    cout<<"Metodo nao implementado"<<endl;
    
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}


//Faz parte do calculo de raio, diametrio, centro e periferia

vector<int> Grafo::dijkstra_distancias(char origem) {
    const int INF = 1000;
    vector<int> distancias(lista_adj.size(), INF);
    vector<bool> visitado(lista_adj.size(), false);

    int inicio = indice_no(origem);
    distancias[inicio] = 0;

    for (int i = 0; i < lista_adj.size(); ++i) {
        int u = -1;
        int menor = INF;

        for (int j = 0; j < lista_adj.size(); ++j) {
            if (!visitado[j] && distancias[j] < menor) {
                menor = distancias[j];
                u = j;
            }
        }

        if (u == -1) break;
        visitado[u] = true;

        for (Aresta* aresta : lista_adj[u]->arestas) {
            int v = indice_no(aresta->destino->id);
            if (!visitado[v] && distancias[u] + aresta->peso < distancias[v]) {
                distancias[v] = distancias[u] + aresta->peso;
            }
        }
    }

    return distancias;
}


int Grafo::raio() {
    vector<int> excentricidades;
    for (No* no : lista_adj) {
        int exc = 0;
        vector<int> distancias = dijkstra_distancias(no->id);
        for (int d : distancias) {
            if (d != 1000) exc = max(exc, d);
        }
        excentricidades.push_back(exc);
    }

    int raio = *min_element(excentricidades.begin(), excentricidades.end());
    return raio;
    cout<<"Metodo nao implementado"<<endl;
    
}

int Grafo::diametro() {
    vector<int> excentricidades;
    for (No* no : lista_adj) {
        int exc = 0;
        vector<int> distancias = dijkstra_distancias(no->id);
        for (int d : distancias) {
            if (d != 1000) exc = max(exc, d);
        }
        excentricidades.push_back(exc);
    }

    int diametro = *max_element(excentricidades.begin(), excentricidades.end());
    return diametro;
    cout<<"Metodo nao implementado"<<endl;
    
}

vector<char> Grafo::centro() {
     vector<char> centro;
    vector<int> excentricidades;

    for (No* no : lista_adj) {
        int exc = 0;
        vector<int> distancias = dijkstra_distancias(no->id);
        for (int d : distancias) {
            if (d != 1000) exc = max(exc, d);
        }
        excentricidades.push_back(exc);
    }

    int raio = *min_element(excentricidades.begin(), excentricidades.end());
    for (int i = 0; i < excentricidades.size(); ++i) {
        if (excentricidades[i] == raio) {
            centro.push_back(lista_adj[i]->id);
        }
    }
    cout<<"Metodo nao implementado"<<endl;
}

vector<char> Grafo::periferia() {
    vector<char> periferia;
    vector<int> excentricidades;

    for (No* no : lista_adj) {
        int exc = 0;
        vector<int> distancias = dijkstra_distancias(no->id);
        for (int d : distancias) {
            if (d != 1000) exc = max(exc, d);
        }
        excentricidades.push_back(exc);
    }

    int diametro = *max_element(excentricidades.begin(), excentricidades.end());
    for (int i = 0; i < excentricidades.size(); ++i) {
        if (excentricidades[i] == diametro) {
            periferia.push_back(lista_adj[i]->id);
        }
    }
    return periferia;
    cout<<"Metodo nao implementado"<<endl;
    
}