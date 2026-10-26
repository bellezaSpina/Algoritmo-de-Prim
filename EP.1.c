#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "EP.1.heap.c"

#define PesoMax FLT_MAX 
   
#ifdef MATRIZ
#include "EP.1.matriz.c"
#else
#include "EP.1.lista.c"
#endif


void VisitaBP(int v, GRAFO* g, int nvr1, int cor[]);
void buscaProfundidade(GRAFO* g, int v1, int nvr1, int cor[]);
bool GrafoConectado(GRAFO* g);
bool LeGrafo(char** nomearq, GRAFO *grafo, FILE* fsaida);
void Prim(GRAFO* g, int S[], int raiz);
void VisitaMD(GRAFO* g, int v, int Nv, float dist[], int cor[], int S[], int n);
int verticeMaisDistante(GRAFO* g, int S[], int n, int raiz);


bool LeGrafo(char** nomearq, GRAFO *grafo, FILE* fsaida){ //le_grafo
    int Numvert, Narest;
    int aviso = 0; //se aviso = 1 tem algo errado

    FILE* fentrada = fopen(nomearq[0], "r");
    if(!fentrada){
        printf("erro na entrada");
        exit(1);
    }

    if(fscanf(fentrada, "%d %d", &Numvert, &Narest) != 2){
        printf("erro na leitura do numero de vertices e arestas");
                fclose(fentrada);
                exit(1);
    }

    int* Ventra1 = (int*)malloc(sizeof(int)*(Narest));
    float* Vpesos = (float*)malloc(sizeof(float)*Narest);
    int* Ventra2 = (int*)malloc(sizeof(int)*Narest);
    int valorLido1, valorLido2;
    float valorPeso;
    for(int i = 0; i < Narest; i++){
        if(fscanf(fentrada, "%d %d %f", &valorLido1, &valorLido2, &valorPeso) != 3){
                printf("erro na leitura do arquivo");
                fclose(fentrada);
                exit(1);
            }

        Ventra1[i] = valorLido1;
        Ventra2[i] = valorLido2;
        Vpesos[i] = valorPeso;
    }

    fclose(fentrada);

    inicializaGrafo(grafo, Numvert);
    //teste por cada aresta
    for(int i = 0; i < Narest; i++){
        if(verticeValido(Ventra1[i], grafo) == false){
            fprintf(fsaida, "ERRO: VERTICE INVALIDO (%d)\n", Ventra1[i]);
            aviso = 1;
        }
        if(verticeValido(Ventra2[i], grafo) == false){
            fprintf(fsaida, "ERRO: VERTICE INVALIDO (%d)\n", Ventra2[i]);
            aviso = 1;
        }
        if(Ventra1[i] == Ventra2[i]){
            fprintf(fsaida, "ERRO: AUTO-LACO (%d,%d)\n", Ventra1[i], Ventra1[i]);
            aviso = 1;
        }
        if(existeAresta(Ventra1[i], Ventra2[i], grafo) == true){
            fprintf(fsaida, "ERRO: ARESTA PARALELA (%d,%d)\n", Ventra1[i], Ventra2[i]);
            aviso = 1;
        }
        if(Vpesos[i] < 0){
            fprintf(fsaida, "ERRO: PESO INVALIDO (%.1f)\n", Vpesos[i]);
            aviso = 1;
        }
        insereAresta(Ventra1[i], Ventra2[i], Vpesos[i], grafo);
    }
    free(Ventra1);
    free(Ventra2);
    free(Vpesos);
    if(aviso == 1){
        return false;
    }else{
        //teste do grafo conectado
        if(GrafoConectado(grafo) == false){
            fprintf(fsaida, "ERRO: GRAFO NAO CONECTADO\n");
            return false;
        }
        return true;}   
}

void VisitaBP(int v, GRAFO* g, int nvr1, int cor[]){
    cor[v] = 1;
    int v2i = 0;
    ApontadorVerticeAdjacente v2;
    v2 = primeiroListaAdj(v, g);
    while(v2 != VERTICEINVALIDO){
        v2i = VertDestino(v2, g);
        if(cor[v2i] == 0){
            VisitaBP(v2i, g, nvr1, cor);
        }
        v2 = proxListaAdj(v, g, v2);
    }
    cor[v] = 2;
}

void buscaProfundidade(GRAFO* g, int v1, int nvr1, int cor[]){//v1 origem | usado para verificar se e conectado
    for(int i = 0; i < nvr1; i++){
        cor[i] = 0;
    }
    VisitaBP(v1, g, nvr1, cor);
} 

bool GrafoConectado(GRAFO* g){ //teste se o grafo conectado
    int nvr = obtemNrVertices(g);
    int cor[nvr]; //branco = 0, cinza = 1, preto = 2
    buscaProfundidade(g, 0, nvr, cor);
    for(int i = 0; i < nvr; i++){
        if(cor[i] == 0){
            return false;
        }
    }
    return true;
}

void Prim(GRAFO* g, int S[], int raiz){
    int NumVert = obtemNrVertices(g);
    int tam = NumVert;
    int k = NumVert - 1;

    float CHmin_Peso[NumVert]; //Peso dos vertices
    int atual[NumVert];        //vetor pra indicar qual indice do vertice no vetor dos Pesos
    int vertice[NumVert];      //vetor para indicar qual vertice esta em determinada posicao do vetor dos Pesos
    int ant[NumVert];          //vetor pra anteriores

    for(int i = 0; i < NumVert; i++){ //inicializacao
        CHmin_Peso[i] = PesoMax;
        atual[i] = i;
        vertice[i] = i;
        ant[i] = -1;
    }
    CHmin_Peso[raiz] = 0;
    int p = raiz; //primeiro
    ApontadorVerticeAdjacente w;
    int wi = 0;
    int j = 0;
    while(k != 0){
        removeMin(CHmin_Peso, &tam, atual, vertice); //tam diminiu
        w = primeiroListaAdj(p, g);
        while(w != VERTICEINVALIDO){ //passo por todos adjacentes de p
            wi = VertDestino(w, g);
            if((obtemPeso(p, wi, g) < CHmin_Peso[atual[wi]]) && (atual[wi] < tam)){
                DiminuiChave(CHmin_Peso, tam, wi, atual, obtemPeso(p, wi, g), vertice);
                ant[wi] = p;
            }
            w = proxListaAdj(p, g, w);
        }
        k--;
        p = vertice[0];
        S[j] = ant[p];
        S[j+1] = p;
        j = j + 2;
        
    }
}

//funcao de visita para a maior distancia | busca em profundidade
void VisitaMD(GRAFO* g, int v, int Nv, float dist[], int cor[], int S[], int n){ 
    cor[v] = 1;
    int v2i;
    ApontadorVerticeAdjacente v2 = primeiroListaAdj(v, g);
    while(v2 != VERTICEINVALIDO){
        v2i = VertDestino(v2, g);
        if(cor[v2i] == 0){
            for(int j = 0; j < n; j = j + 2){
                if((S[j] == v && S[j+1] == v2i) || (S[j] == v2i && S[j+1] == v)){
                    dist[v2i] = dist[v] + obtemPeso(v, v2i, g);
                    VisitaMD(g, v2i, Nv, dist, cor, S, n);
                }
            }
        }
        v2 = proxListaAdj(v, g, v2);
    }
    cor[v] = 2;
}

int verticeMaisDistante(GRAFO* g, int S[], int n, int raiz){ //n = tamamnho de S[]
    int Nv = obtemNrVertices(g);
    int cor[Nv];
    float dist[Nv];
    //incializacao
    for(int i = 0; i < Nv; i++){
        cor[i] = 0;
        dist[i] = 0;
    }
    VisitaMD(g, raiz, Nv, dist, cor, S, n);
    int maisDistante = 0;
    for(int i = 0; i < Nv; i++){
        if(dist[i] > dist[maisDistante]){
            maisDistante = i;
        }
    }
    return maisDistante;

} 


int main(int argc, char* argv[]){
     if (argc != 3){ //nome entrada saida
        printf("Este programa exige dois argumentos: entrada e saida\n");
        exit(1);
    }

    GRAFO g1;
     FILE* fsaida = fopen(argv[2], "w");
    if(!fsaida){
        printf("erro na saida");
        exit(1);
    }


    if((LeGrafo(argv + 1, &g1, fsaida)) == false){
        fclose(fsaida);
        return 0;
    }
    
    int NumVert = obtemNrVertices(&g1);

    int Sf[(NumVert - 1)*2]; //vetor com as arestas minimas
    for(int i = 0; i < (NumVert - 1)*2; i++){
        Sf[i] = 0;
    }

    Prim(&g1, Sf, 0);

    float Soma = 0;
    for(int i = 0; i < (NumVert - 1)*2; i = i + 2){
        Soma = Soma + obtemPeso(Sf[i], Sf[i + 1], &g1);
    }

    fprintf(fsaida, "%.1f\n", Soma); //saida da soma

    int Vmd = verticeMaisDistante(&g1, Sf, (NumVert - 1)*2, 0); //saida do mais distante da origem
    fprintf(fsaida, "%d\n", Vmd);

    for(int i = 0; i < (NumVert - 1)*2; i = i + 2){ //organiza na horizontal
        if(Sf[i] > Sf[i+1]){
            TrocarI(&Sf[i], &Sf[i+1]);
        }
    }

    for (int i = 0; i < ((NumVert - 1)*2) - 2; i = i + 2) {  //organiza na vertical
        int menor = i;
        for (int j = i + 2; j < (NumVert - 1)*2; j = j + 2) {
            if (Sf[j] < Sf[menor] || (Sf[j] == Sf[menor] && Sf[j+1] < Sf[menor+1])) {
                menor = j;
            }
        }
        if (menor != i) {
            TrocarI(&Sf[i], &Sf[menor]);
            TrocarI(&Sf[i+1], &Sf[menor+1]);
        }
    }

    for(int i = 0; i < (NumVert - 1)*2; i = i + 2){ //saida dos vertices
        fprintf(fsaida, "%d %d %.1f\n", Sf[i], Sf[i+1], obtemPeso(Sf[i], Sf[i+1], &g1));
    }
    
    return 0;
}