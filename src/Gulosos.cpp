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
    const int n = (int)g->lista_adj.size();

    // contagem[v] = quantas incidências (v + vizinhos em D) já contam para v
    std::vector<int> contagem(n, 0);
    std::vector<bool> dominado(n, false);
    std::vector<char> D;
    std::unordered_set<char> emD;

    auto atualizaDominado = [&](const std::vector<int>& c, std::vector<bool>& dom) {
        dom.resize(n);
        for (int i = 0; i < n; ++i) dom[i] = (c[i] >= 2);
    };

    atualizaDominado(contagem, dominado);

    auto aplicarIncremento = [&](char v, std::vector<int>& c) {
        int idx = idParaIndice[v];
        c[idx]++;
        for (Aresta* a : g->lista_adj[idx]->arestas) {
            c[idParaIndice[a->id_no_alvo]]++;
        }
    };

    auto todos2Dominados = [&]() {
        for (bool d : dominado) if (!d) return false;
        return true;
    };

    while (!todos2Dominados()) {
        char melhor = '\0';
        size_t bestNovos2 = 0; // quantos passam a ficar >= 2
        int bestGanho = -1;    // ganho parcial rumo a 2 (cap em 2)
        int bestDeg = -1;      // desempate por grau

        for (No* no : g->lista_adj) {
            if (emD.count(no->id)) continue;

            std::vector<int> tmp = contagem;
            aplicarIncremento(no->id, tmp);

            size_t novos2 = 0;
            int ganho = 0;
            for (int i = 0; i < n; ++i) {
                int before = std::min(2, contagem[i]);
                int after  = std::min(2, tmp[i]);
                ganho += (after - before);
                if (!dominado[i] && tmp[i] >= 2) novos2++;
            }

            int deg = (int)g->lista_adj[idParaIndice[no->id]]->arestas.size();

            if (novos2 > bestNovos2 ||
               (novos2 == bestNovos2 && ganho > bestGanho) ||
               (novos2 == bestNovos2 && ganho == bestGanho && deg > bestDeg)) {
                melhor = no->id;
                bestNovos2 = novos2;
                bestGanho = ganho;
                bestDeg = deg;
            }
        }

        // Fallback defensivo (não deve ocorrer): pega qualquer vértice fora de D
        if (melhor == '\0') {
            for (No* no : g->lista_adj) {
                if (!emD.count(no->id)) { melhor = no->id; break; }
            }
        }

        // Aplica a escolha
        D.push_back(melhor);
        emD.insert(melhor);
        aplicarIncremento(melhor, contagem);

        // Atualiza dominados a partir da contagem incremental
        for (int i = 0; i < n; ++i) {
            if (contagem[i] >= 2) dominado[i] = true;
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
