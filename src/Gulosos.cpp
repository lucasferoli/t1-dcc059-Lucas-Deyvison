#include "Gulosos.h"

Gulosos::Gulosos(Grafo* grafo) : g(grafo) {
    for (int i = 0; i < g->lista_adj.size(); i++) {
        idParaIndice[g->lista_adj[i]->id] = i;
    }
}

std::vector<char> Gulosos::coberturaAte2Passos(char v) {
    std::unordered_set<char> visitados;
    std::queue<std::pair<char,int>> fila; // (vértice, distância)

    visitados.insert(v);
    fila.push(std::make_pair(v, 0));

    while (!fila.empty()) {
        std::pair<char,int> frente = fila.front();
        char atual = frente.first;
        int dist = frente.second;
        fila.pop();

        if (dist == 2) continue;

        int idx = idParaIndice[atual];
        for (Aresta* a : g->lista_adj[idx]->arestas) {
            if (!visitados.count(a->id_no_alvo)) {
                visitados.insert(a->id_no_alvo);
                fila.push(std::make_pair(a->id_no_alvo, dist+1));
            }
        }
    }

    return std::vector<char>(visitados.begin(), visitados.end());
}

void Gulosos::marcarDominados(std::vector<bool>& dominado, const std::vector<char>& vertices) {
    for (char v : vertices) {
        dominado[idParaIndice[v]] = true;
    }
}

bool Gulosos::todosDominados(const std::vector<bool>& dominado) {
    for (size_t i = 0; i < dominado.size(); i++) {
        if (!dominado[i]) return false;
    }
    return true;
}

std::vector<char> Gulosos::guloso2Dominating() {
    std::vector<bool> dominado(g->lista_adj.size(), false);
    std::vector<char> D;

    while (!todosDominados(dominado)) {
        char melhor = '\0';
        size_t melhorCobertura = 0;

        for (No* no : g->lista_adj) {
            if (std::find(D.begin(), D.end(), no->id) != D.end()) continue;

            std::vector<char> cobertos = coberturaAte2Passos(no->id);
            size_t novos = 0;
            for (char v : cobertos) {
                if (!dominado[idParaIndice[v]]) novos++;
            }

            if (novos > melhorCobertura) {
                melhorCobertura = novos;
                melhor = no->id;
            }
        }

        D.push_back(melhor);
        marcarDominados(dominado, coberturaAte2Passos(melhor));
    }

    return D;
}

std::vector<char> Gulosos::gulosoRandomizado(double alpha) {
    std::vector<bool> dominado(g->lista_adj.size(), false);
    std::vector<char> D;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (!todosDominados(dominado)) {
        struct Candidato { char id; size_t cobertura; };
        std::vector<Candidato> candidatos;

        // Calcula a cobertura de cada candidato
        for (No* no : g->lista_adj) {
            if (std::find(D.begin(), D.end(), no->id) != D.end()) continue;

            std::vector<char> cobertos = coberturaAte2Passos(no->id);
            size_t novos = 0;
            for (char v : cobertos) {
                if (!dominado[idParaIndice[v]]) novos++;
            }

            candidatos.push_back({no->id, novos});
        }

        // Ordena candidatos pela cobertura (decrescente)
        std::sort(candidatos.begin(), candidatos.end(), [](const Candidato& a, const Candidato& b) {
            return a.cobertura > b.cobertura;
        });

        size_t melhorCobertura = candidatos[0].cobertura;
        size_t piorCobertura = candidatos.back().cobertura;

        // Calcula limite para RCL baseado na diferença entre melhor e pior
        double limite = melhorCobertura - alpha * (melhorCobertura - piorCobertura);

        std::vector<char> RCL;
        for (const auto& c : candidatos) {
            if (c.cobertura >= limite) {
                RCL.push_back(c.id);
            }
        }

        char escolhido;
        if (alpha == 0.0) {
            // Escolhe o melhor candidato (determinístico)
            escolhido = candidatos[0].id;
        } else {
            // Escolha aleatória dentro da RCL
            std::uniform_int_distribution<> dist(0, (int)RCL.size() - 1);
            escolhido = RCL[dist(gen)];
        }

        marcarDominados(dominado, coberturaAte2Passos(escolhido));
        D.push_back(escolhido);
    }

    return D;
}



std::vector<char> Gulosos::gulosoRandomizadoReativo(int maxIter, const std::vector<double>& alphas, int bloco) {
    std::vector<double> prob(alphas.size(), 1.0 / alphas.size());
    std::vector<double> somaQualidade(alphas.size(), 0.0);
    std::vector<int> contagem(alphas.size(), 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> distProb(prob.begin(), prob.end());

    std::vector<char> melhorSol;
    size_t melhorTam = SIZE_MAX;

    for (int iter = 0; iter < maxIter; iter++) {
        int idxAlpha = distProb(gen);
        double alpha = alphas[idxAlpha];

        std::vector<char> sol = gulosoRandomizado(alpha);

        if (sol.size() < melhorTam) {
            melhorTam = sol.size();
            melhorSol = sol;
        }

        somaQualidade[idxAlpha] += 1.0 / sol.size();
        contagem[idxAlpha]++;

        if ((iter+1) % bloco == 0) {
            for (int i = 0; i < (int)alphas.size(); i++) {
                if (contagem[i] > 0) {
                    prob[i] = somaQualidade[i] / contagem[i];
                }
            }
            double somaP = 0.0;
            for (size_t i = 0; i < prob.size(); i++) somaP += prob[i];
            for (size_t i = 0; i < prob.size(); i++) prob[i] /= somaP;

            distProb = std::discrete_distribution<>(prob.begin(), prob.end());
        }
    }

    return melhorSol;
}
