#include "Gulosos.h"


Gulosos::Gulosos(Grafo* grafo) : g(grafo) {
    for (int i = 0; i < (int)g->lista_adj.size(); i++) {
        idParaIndice[g->lista_adj[i]->id] = i;
    }
}

// Vizinhos diretos de v (inclui v)
std::vector<char> Gulosos::vizinhosDiretos(char v) {
    std::vector<char> res;
    res.push_back(v);
    int idx = idParaIndice[v];
    for (Aresta* a : g->lista_adj[idx]->arestas) {
        res.push_back(a->id_no_alvo);
    }
    return res;
}

// Atualiza dominados a partir do conjunto D usando contagem incremental
void Gulosos::atualizarDominados(const std::vector<char>& D, std::vector<bool>& dominado) {
    std::vector<int> contagem(g->lista_adj.size(), 0);

    for (char v : D) {
        int idx = idParaIndice[v];
        contagem[idx]++;
        for (Aresta* a : g->lista_adj[idx]->arestas) {
            contagem[idParaIndice[a->id_no_alvo]]++;
        }
    }

    for (size_t i = 0; i < contagem.size(); i++) {
        dominado[i] = (contagem[i] >= 2);
    }
}

bool Gulosos::todosDominados(const std::vector<bool>& dominado) {
    for (bool d : dominado) {
        if (!d) return false;
    }
    return true;
}

std::vector<char> Gulosos::guloso2Dominating() {
    std::vector<int> contagem(g->lista_adj.size(), 0); // contagem de dominância
    std::vector<bool> dominado(g->lista_adj.size(), false);
    std::vector<char> D;
    std::unordered_set<char> emD;

    while (true) {
        if (std::all_of(dominado.begin(), dominado.end(), [](bool d){ return d; })) break;

        char melhor = '\0';
        size_t melhorCobertura = 0;

        for (No* no : g->lista_adj) {
            if (emD.count(no->id)) continue;

            int idx = idParaIndice[no->id];
            size_t novos = 0;

            std::vector<int> tempContagem = contagem;
            tempContagem[idx]++;
            if (tempContagem[idx] == 2) novos++;

            for (Aresta* a : g->lista_adj[idx]->arestas) {
                int vizIdx = idParaIndice[a->id_no_alvo];
                tempContagem[vizIdx]++;
                if (tempContagem[vizIdx] == 2) novos++;
            }

            if (novos > melhorCobertura) {
                melhorCobertura = novos;
                melhor = no->id;
            }
        }

        if (melhor == '\0') break;

        D.push_back(melhor);
        emD.insert(melhor);

        int idxMelhor = idParaIndice[melhor];
        contagem[idxMelhor]++;
        if (contagem[idxMelhor] == 2) dominado[idxMelhor] = true;

        for (Aresta* a : g->lista_adj[idxMelhor]->arestas) {
            int vizIdx = idParaIndice[a->id_no_alvo];
            contagem[vizIdx]++;
            if (contagem[vizIdx] == 2) dominado[vizIdx] = true;
        }
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

        for (No* no : g->lista_adj) {
            if (std::find(D.begin(), D.end(), no->id) != D.end()) continue;

            std::vector<char> tempD = D;
            tempD.push_back(no->id);

            std::vector<bool> tempDominado(g->lista_adj.size(), false);
            atualizarDominados(tempD, tempDominado);

            size_t novos = 0;
            for (size_t i = 0; i < dominado.size(); i++) {
                if (!dominado[i] && tempDominado[i]) novos++;
            }

            candidatos.push_back({no->id, novos});
        }

        std::sort(candidatos.begin(), candidatos.end(), [](const Candidato& a, const Candidato& b) {
            return a.cobertura > b.cobertura;
        });

        size_t melhorCobertura = candidatos[0].cobertura;
        size_t piorCobertura = candidatos.back().cobertura;

        double limite = melhorCobertura - alpha * (melhorCobertura - piorCobertura);

        std::vector<char> RCL;
        for (const auto& c : candidatos) {
            if (c.cobertura >= limite) {
                RCL.push_back(c.id);
            }
        }

        char escolhido;
        if (alpha == 0.0) {
            escolhido = candidatos[0].id;
        } else {
            std::uniform_int_distribution<> dist(0, (int)RCL.size() - 1);
            escolhido = RCL[dist(gen)];
        }

        D.push_back(escolhido);
        atualizarDominados(D, dominado);
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

        if ((iter + 1) % bloco == 0) {
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
