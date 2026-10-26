#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNUMVERTICE 100
#define ARESTANULA -1
#define VERTICEINVALIDO -1

typedef float Peso;

typedef struct {
    Peso mat[MAXNUMVERTICE][MAXNUMVERTICE];
    int Numvertice;
} GRAFO;

typedef int ApontadorVerticeAdjacente;

bool inicializaGrafo(GRAFO* grafo, int Nvert){
    if(Nvert > MAXNUMVERTICE){
        fprintf(stderr, "Erro no InicializaGrafo(Mvert > Max)");
        return false;
    }
     if (Nvert <= 0) {
        fprintf(stderr, "ERRO no inicializaGrafo(Nvert < 0)");
        return false;
    }
    grafo->Numvertice = Nvert;
    for(int i = 0; i < grafo->Numvertice; i++){
        for(int j = 0; j < grafo->Numvertice; j++){
            grafo->mat[i][j] = ARESTANULA;
        }
    }
    return true;
}

int obtemNrVertices(GRAFO* grafo){
    return(grafo->Numvertice);
}

bool verticeValido(int v, GRAFO *grafo){ //verifica_validade_vertice
    if(v >= grafo->Numvertice){
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
    grafo->mat[v1][v2] = peso;
    grafo->mat[v2][v1] = peso;

}

Peso obtemPeso(int v1, int v2, GRAFO* grafo){
    verticeValido(v1, grafo);
    verticeValido(v2, grafo);
    return(grafo->mat[v1][v2]);
}

bool existeAresta(int v1, int v2, GRAFO *grafo){
    if(verticeValido(v1, grafo) == false){return false;}
    if(verticeValido(v2, grafo) == false){return false;}
    if(grafo->mat[v1][v2] == ARESTANULA){return false;}
    return true;
}

bool removeAresta(int v1, int v2, Peso* peso, GRAFO *grafo){
    verticeValido(v1, grafo);
    verticeValido(v2, grafo);
    if(grafo->mat[v1][v2] == ARESTANULA){return false;}
    *peso = grafo->mat[v1][v2];
    grafo->mat[v1][v2] = ARESTANULA;
    return true;
}

bool listaAdjVazia(int v, GRAFO* grafo){
    verticeValido(v, grafo);
    int i = 0;
    while((i < grafo->Numvertice) && (grafo->mat[v][i] == ARESTANULA)){
        i++;
    }
    if(i == grafo->Numvertice){
        return true;}else return false; 
}

ApontadorVerticeAdjacente proxListaAdj(int v, GRAFO* grafo, ApontadorVerticeAdjacente atual){
    verticeValido(v, grafo);
    atual++; //para o proximo teste nao sair
    while((atual < grafo->Numvertice) && (grafo->mat[v][atual] == ARESTANULA)){
        atual++;
    }
    if(atual == grafo->Numvertice){return VERTICEINVALIDO;}
    else return atual;
}

ApontadorVerticeAdjacente primeiroListaAdj(int v, GRAFO* grafo){
    int primeiro = proxListaAdj(v, grafo, -1);
    if(primeiro == VERTICEINVALIDO){ return VERTICEINVALIDO;} 
    return primeiro;    
}

int VertDestino(ApontadorVerticeAdjacente p, GRAFO* g){
    return (int)p;
}

void liberaGrafo(GRAFO* grafo){
    grafo->Numvertice = 0;
    for(int i = 0; i < MAXNUMVERTICE; i++){
        for(int j = 0; j < MAXNUMVERTICE; j++){
            grafo->mat[i][j] = ARESTANULA;
        }
    }
}


