#include "Gerenciador.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <limits>

using namespace std;

void Gerenciador::comandos(Grafo* grafo) {
    while (true) {
        cout << "Digite uma das opcoes abaixo e pressione enter:" << endl << endl;
        cout << "(a) Fecho transitivo direto de um no;" << endl;
        cout << "(b) Fecho transitivo indireto de um no;" << endl;
        cout << "(c) Caminho minimo (Djikstra);" << endl;
        cout << "(d) Caminho minimo (Floyd);" << endl;
        cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << endl;
        cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << endl;
        cout << "(g) Arvore de caminhamento em profundidade;" << endl;
        cout << "(h) Raio, diametro, centro e periferia do grafo;" << endl;
        cout << "(0) Sair;" << endl << endl;

        cout << "Opcao: ";
        char resp;
        cin >> resp;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer

        switch (resp) {
            case 'a': {
                char id_no = get_id_entrada();
                vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);

                cout << "Fecho transitivo direto a partir do no '" << id_no << "':" << endl;
                for (char id : fecho_transitivo_direto) {
                    cout << id << " ";
                }
                cout << endl << endl;

                if (pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                    cout << "Metodo de impressao em arquivo nao implementado" << endl << endl;
                }
                break;
            }

            case 'b': {
                char id_no = get_id_entrada();
                vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);

                cout << "\nFecho transitivo indireto do vertice '" << id_no << "':" << endl;
                if (fecho_transitivo_indireto.empty()) {
                    cout << "Nenhum vertice alcanca o vertice '" << id_no << "'" << endl;
                } else {
                    for (char vertice : fecho_transitivo_indireto) {
                        cout << vertice << " ";
                    }
                    cout << endl;
                    cout << "Total: " << fecho_transitivo_indireto.size() << " vertices" << endl;
                }
                cout << endl;

                if (pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                    ofstream arquivo_saida("fecho_trans_indir.txt");
                    if (arquivo_saida.is_open()) {
                        arquivo_saida << "Fecho transitivo indireto do vertice '" << id_no << "':\n";
                        for (char vertice : fecho_transitivo_indireto) {
                            arquivo_saida << vertice << " ";
                        }
                        arquivo_saida << "\nTotal: " << fecho_transitivo_indireto.size() << " vertices\n";
                        arquivo_saida.close();
                        cout << "Resultado salvo em 'fecho_trans_indir.txt'" << endl << endl;
                    } else {
                        cout << "Erro ao criar arquivo de saida!" << endl << endl;
                    }
                }
                break;
            }

            case 'c': {
                char id_no_1 = get_id_entrada();
                char id_no_2 = get_id_entrada();
                vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);
                cout << "Metodo de impressao em tela nao implementado" << endl << endl;

                if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                    cout << "Metodo de impressao em arquivo nao implementado" << endl;
                }
                break;
            }

            case 'd': {
                cout << "\nCaminho minimo por Floyd-Warshall" << endl;
                char de = get_id_entrada();
                char para = get_id_entrada();

                vector<char> rota = grafo->caminho_minimo_floyd(de, para);

                if (!rota.empty()) {
                    cout << "\nRota encontrada (" << rota.size() << " vertices):\n";
                    for (size_t passo = 0; passo < rota.size(); passo++) {
                        cout << rota[passo];
                        if (passo != rota.size() - 1) cout << " -> ";
                    }
                    cout << endl;
                }

                if (pergunta_imprimir_arquivo("floyd_path.txt")) {
                    ofstream saida("floyd_path.txt");
                    if (saida) {
                        saida << "De: " << de << "\nPara: " << para << "\nRota: ";
                        copy(rota.begin(), rota.end(), ostream_iterator<char>(saida, " "));
                        cout << "Arquivo salvo com sucesso!" << endl;
                    }
                }
                break;
            }

            case 'e': {
                int tam;
                cout << "Digite o tamanho do subconjunto: ";
                cin >> tam;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (tam > 0 && tam <= grafo->ordem) {
                    vector<char> ids = get_conjunto_ids(grafo, tam);
                    Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                    cout << "Metodo de impressao em tela nao implementado" << endl << endl;

                    if (pergunta_imprimir_arquivo("agm_prim.txt")) {
                        cout << "Metodo de impressao em arquivo nao implementado" << endl;
                    }

                    delete arvore_geradora_minima_prim;
                } else {
                    cout << "Valor invalido" << endl;
                }
                break;
            }

            case 'f': {
                int tam;
                cout << "Digite o tamanho do subconjunto: ";
                cin >> tam;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (tam > 0 && tam <= grafo->ordem) {
                    vector<char> ids = get_conjunto_ids(grafo, tam);
                    Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                    cout << "Metodo de impressao em tela nao implementado" << endl << endl;

                    if (pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                        cout << "Metodo de impressao em arquivo nao implementado" << endl;
                    }

                    delete arvore_geradora_minima_kruskal;
                } else {
                    cout << "Valor invalido" << endl;
                }
                break;
            }

            case 'g': {
                char id_no = get_id_entrada();
                vector<pair<char, char>> arestas_retorno;
                Grafo* arvore_dfs = grafo->arvore_caminhamento_profundidade(id_no, arestas_retorno);

                if (arvore_dfs) {
                    cout << "\nArvore DFS:\n";
                    for (No* no : arvore_dfs->lista_adj) {
                        cout << no->id << " -> ";
                        for (Aresta* aresta : no->arestas) {
                            cout << aresta->id_no_alvo << " ";
                        }
                        cout << endl;
                    }

                    if (!arestas_retorno.empty()) {
                        cout << "\nArestas de retorno:\n";
                        for (auto& par : arestas_retorno) {
                            cout << par.first << " -> " << par.second << endl;
                        }
                    }

                    if (pergunta_imprimir_arquivo("dfs_tree.txt")) {
                        ofstream arquivo("dfs_tree.txt");
                        if (arquivo.is_open()) {
                            arquivo << "Arvore DFS a partir de " << id_no << ":\n";
                            for (No* no : arvore_dfs->lista_adj) {
                                arquivo << no->id << " -> ";
                                for (Aresta* aresta : no->arestas) {
                                    arquivo << aresta->id_no_alvo << " ";
                                }
                                arquivo << "\n";
                            }
                            if (!arestas_retorno.empty()) {
                                arquivo << "\nArestas de retorno:\n";
                                for (auto& par : arestas_retorno) {
                                    arquivo << par.first << " -> " << par.second << "\n";
                                }
                            }
                            arquivo.close();
                            cout << "Arquivo salvo com sucesso!" << endl;
                        } else {
                            cout << "Erro ao criar arquivo de saida!" << endl;
                        }
                    }
                }

                delete arvore_dfs;
                break;
            }

            case 'h': {
                int raio = grafo->raio();
                int diametro = grafo->diametro();
                vector<char> centro = grafo->centro();
                vector<char> periferia = grafo->periferia();

                cout << "Metodo de impressao em tela nao implementado" << endl << endl;

                if (pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt")) {
                    cout << "Metodo de impressao em arquivo nao implementado" << endl;
                }
                break;
            }

            case '0': {
                exit(0);
            }

            default: {
                cout << "Opcao invalida" << endl;
            }
        }
    }
}

char Gerenciador::get_id_entrada() {
    cout << "Digite o id de um no: ";
    char id;
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo* grafo, int tam) {
    vector<char> ids = {};
    while ((int)ids.size() < tam) {
        char id_no = get_id_entrada();
        bool existe = false;
        for (No* no : grafo->lista_adj) {
            if (no->id == id_no) {
                existe = true;
                break;
            }
        }

        if (!existe) {
            cout << "Vertice nao existe" << endl;
        } else {
            bool repetido = find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido) {
                cout << "Valor repetido" << endl;
            } else {
                ids.push_back(id_no);
            }
        }
    }
    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {
    cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "(1) Sim;" << endl;
    cout << "(2) Nao." << endl;
    int resp;
    cin >> resp;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout << "Resposta invalida" << endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}
