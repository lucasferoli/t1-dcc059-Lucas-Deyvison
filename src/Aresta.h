#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta {
public:
    Aresta();
    Aresta(int peso, No* destino); 
    ~Aresta();

    char id_no_alvo;
    int peso;
    No* destino;
    
};



#endif //ARESTA_H
