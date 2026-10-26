#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void MinHeapify(float vetor[], int i, int n, int indice[], int vertice[]);
void removeMin(float vetor[], int *n, int indice[], int vertice[]);
void DiminuiChave(float vetor[], int n, int v, int indice[], float novo, int vertice[]);
void trocaNoHeap(float vetor[], int vertice[], int indice[], int i, int j);
void heapMin(float vetor[], int n, int indice[], int vertice[]);
void TrocarI(int *a, int *b);

int parent(int i)
{
    return ((i - 1) / 2);
}

int esquerdo(int i)
{
    return ((2 * i) + 1);
}

int direito(int i)
{
    return ((2 * i) + 2);
}

float Minimo(float vetor[])
{
    return (vetor[0]);
}

void removeMin(float vetor[], int *n, int indice[], int vertice[])
{
    if (*n == 1)
    {
        (*n)--;
        return;
    }
    float raiz = vetor[0];
    trocaNoHeap(vetor, vertice, indice, 0, (*n - 1));
    (*n)--;
    MinHeapify(vetor, 0, *n, indice, vertice);
}

void DiminuiChave(float vetor[], int n, int v, int indice[], float novo, int vertice[])
{
    int i = indice[v];
    vetor[i] = novo;
    while (i != 0 && vetor[parent(i)] > vetor[i])
    {
        trocaNoHeap(vetor, vertice, indice, i, parent(i));
        i = parent(i);
    }
}

void TrocarI(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void trocaNoHeap(float vetor[], int vertice[], int indice[], int i, int j)
{
    // vetor -> Pesos
    // vertice -> indica qual vertice que esta em determinada posicao do vetorPeso
    // indice -> indica em que posicao do vetorPeso esta o vertice
    // troca os pesos
    float auxPeso = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = auxPeso;

    // troca os vertices
    int auxVert = vertice[i];
    vertice[i] = vertice[j];
    vertice[j] = auxVert;

    // indica o indice real
    indice[vertice[i]] = i;
    indice[vertice[j]] = j;
}

void MinHeapify(float vetor[], int i, int n, int indice[], int vertice[])
{
    int left = esquerdo(i);
    int right = direito(i);
    int menor = i;
    if ((left < n) && (vetor[left] < vetor[menor]))
    {
        menor = left;
    }
    if ((right < n) && (vetor[right] < vetor[menor]))
    {
        menor = right;
    }
    if (menor != i)
    {
        trocaNoHeap(vetor, vertice, indice, i, menor);
        MinHeapify(vetor, menor, n, indice, vertice);
    }
}

void heapMin(float vetor[], int n, int indice[], int vertice[])
{ // organiza todo o vetor
    int i;
    for (i = (n / 2) - 1; i >= 0; i--)
    {
        MinHeapify(vetor, i, n, indice, vertice);
    }
}