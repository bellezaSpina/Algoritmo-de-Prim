#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARESTANULA -1
#define VERTICEINVALIDO NULL

typedef float Peso;

typedef struct str_aresta{
int vdest;
Peso peso;
struct str_aresta * prox;
} Aresta;

typedef struct {
    Aresta** listaAdj;
    int Nvertices;
} GRAFO;

typedef Aresta* ApontadorVerticeAdjacente;

void verificaGrafo(GRAFO* grafo){
    if(grafo == NULL){
        fprintf(stderr, "ERRO na validade do Grafo");
        exit(-1);
    }
}

bool inicializaGrafo(GRAFO* grafo, int Nvert){
    if (Nvert <= 0) {
        fprintf(stderr, "ERRO no inicializaGrafo(Nvert < 0)");
        return false;
    }
    verificaGrafo(grafo);
    grafo->Nvertices = Nvert;
    if(!(grafo->listaAdj = (Aresta**)calloc(Nvert, sizeof(Aresta*)))){
        fprintf(stderr, "ERRO na alocacao");
        return false;
    }
    return true;
}


int obtemNrVertices(GRAFO* grafo){
    verificaGrafo(grafo);
    return grafo->Nvertices;
}

bool verticeValido(int v, GRAFO *grafo){ //verifica_validade_vertice
    if(v >= grafo->Nvertices){
        return false;
    }
    if(v < 0){
        return false;
    }
    return true;
}

void insereAresta(int v1, int v2, Peso peso, GRAFO *grafo){
    if(verticeValido(v1, grafo) == false){return;}
    if(verticeValido(v2, grafo) == false){return;}
    Aresta* p;
    if(!(p = (Aresta*) calloc(1, sizeof(Aresta)))){
        fprintf(stderr,"ERRO na alocacao para iserir aresta");
        return;
    }
    p->vdest = v2;
    p->peso = peso;
    p->prox = grafo->listaAdj[v1];
    grafo->listaAdj[v1] = p;

    Aresta* q;
    if(!(q = (Aresta*) calloc(1, sizeof(Aresta)))){
        fprintf(stderr,"ERRO na alocacao para iserir aresta");
        return;
    }
    q->vdest = v1;
    q->peso = peso;
    q->prox = grafo->listaAdj[v2];
    grafo->listaAdj[v2] = q;
}

Peso obtemPeso(int v1, int v2, GRAFO* grafo){
    verticeValido(v1, grafo);
    verticeValido(v2, grafo);
    Aresta* p = grafo->listaAdj[v1];
    while(p != NULL){
        if(p->vdest == v2){
            return(p->peso);
        }
        p = p->prox;
    }
    return ARESTANULA; //Aresta Nula da matriz(-1)
}

bool existeAresta(int v1, int v2, GRAFO *grafo){
    Aresta* p;
    if(verticeValido(v1, grafo) == false){return false;}
    if(verticeValido(v2, grafo) == false){return false;}
    p = grafo->listaAdj[v1];
    while(p != NULL){
        if(p->vdest == v2){return true;}
        p = p->prox;
    }
    return false;
}

bool removeAresta(int v1, int v2, Peso* peso, GRAFO *grafo){
    verticeValido(v1, grafo);
    verticeValido(v2, grafo);
    Aresta* p;
    p = grafo->listaAdj[v1];
    Aresta* ant = NULL;
    while(p != NULL){
        if(p->vdest == v2){
            if(ant == NULL){
                grafo->listaAdj[v1] = p->prox;
                *peso = p->peso;
                p->prox = NULL;
                free(p);
                return true;
            }else{
                ant->prox = p->prox;
                *peso = p->peso;
                p->prox = NULL;
                free(p);
                return true;
            }
        }
        ant = p;
        p = p->prox;
    }
    return false;
}

bool listaAdjVazia(int v, GRAFO* grafo){
    verticeValido(v, grafo);
    if(grafo->listaAdj[v] == NULL){
        return true;
    }else{return false;}
}

ApontadorVerticeAdjacente primeiroListaAdj(int v, GRAFO* grafo){
    verticeValido(v, grafo);
    return(grafo->listaAdj[v]);
    
}

ApontadorVerticeAdjacente proxListaAdj(int v, GRAFO* grafo, ApontadorVerticeAdjacente atual){
    verticeValido(v, grafo);
    if(atual == NULL){
        return NULL;
    }
    return(atual->prox);
}

int VertDestino(ApontadorVerticeAdjacente p, GRAFO* g){
    if(p != NULL){
        return p->vdest;
    }else{
        fprintf(stderr, "Vert destino erro");
    exit(-1);}
}

void liberaGrafo(GRAFO* grafo){
    int v;
    ApontadorVerticeAdjacente p;
    verificaGrafo(grafo);
    for(v = 0; v < grafo->Nvertices; v++){
        while((p = grafo->listaAdj[v]) != NULL){
            grafo->listaAdj[v] = p->prox;
            p->prox = NULL;
            free(p);
        }
    }    
    grafo->Nvertices = 0;
    free(grafo->listaAdj);
    grafo->listaAdj = NULL;
}
