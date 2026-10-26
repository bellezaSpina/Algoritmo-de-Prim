# Árvore Geradora Mínima com o Algoritmo de Prim

Implementação em **C** do algoritmo de **Prim** sobre grafos não direcionados e ponderados, com o grafo **modularizado como um TAD** que pode ser trocado entre duas representações (**lista de adjacência** ou **matriz de adjacência**) sem alterar o código do algoritmo, e com uma **fila de prioridade (min-heap) com índices** para a extração do vértice de menor chave.

> Exercício-programa (EP) desenvolvido na **USP Leste (EACH-USP)**, curso de **Sistemas de Informação**.
> Disciplina: Algoritmos e Estruturas de Dados II  · Autor: ERICK BELLEZA SPINA

---

## O que o programa faz

Dado um grafo em arquivo texto, o programa:

1. **Valida a entrada** (vértices inválidos, auto-laços, arestas paralelas, pesos negativos e grafo não conectado);
2. Calcula a **Árvore Geradora Mínima (AGM)** com Prim, partindo do vértice `0`;
3. Escreve no arquivo de saída:
   - o **peso total** da AGM;
   - o **vértice mais distante da raiz (0)** dentro da AGM;
   - as **arestas da AGM** em ordem canônica (`u < v`, ordenadas por `u` e depois por `v`).

---

## Estrutura do repositório

```
.
├── .gitignore
├── EP.1.c            # main, leitura/validação do grafo, Prim, DFS e saída
├── EP.1.heap.c       # min-heap com vetores auxiliares de posição (indice/vertice)
├── EP.1.lista.c      # TAD GRAFO com lista de adjacência
├── EP.1.matriz.c     # TAD GRAFO com matriz de adjacência
├── Makefile          # build das duas versões + teste
├── exemplos/         # entradas de exemplo e saída esperada
├── LICENSE
└── README.md
```

---

## Modularização

O ponto central do trabalho é a **separação entre o algoritmo e a representação do grafo**. `EP.1.c` só conhece a interface abaixo; quem a implementa é escolhido em tempo de compilação:

```c
#ifdef MATRIZ
#include "EP.1.matriz.c"   // compilado com -DMATRIZ
#else
#include "EP.1.lista.c"    // padrão
#endif
```

**Interface do TAD `GRAFO`** (idêntica nas duas implementações):

| Operação | Função |
|---|---|
| Criação | `inicializaGrafo`, `liberaGrafo` |
| Consulta | `obtemNrVertices`, `verticeValido`, `existeAresta`, `obtemPeso` |
| Modificação | `insereAresta`, `removeAresta` |
| Percurso de adjacentes | `primeiroListaAdj`, `proxListaAdj`, `VertDestino`, `listaAdjVazia` |

O tipo `ApontadorVerticeAdjacente` abstrai o "ponteiro" para um vizinho: na lista é um `Aresta*`, na matriz é o índice da coluna. Por isso o mesmo laço

```c
w = primeiroListaAdj(p, g);
while (w != VERTICEINVALIDO) {
    wi = VertDestino(w, g);
    /* ... */
    w = proxListaAdj(p, g, w);
}
```

funciona nas duas versões.

| | Lista de adjacência | Matriz de adjacência |
|---|---|---|
| Memória | O(V + E) | O(V²) (limite de **100 vértices**) |
| Percorrer vizinhos de `v` | O(grau(v)) | O(V) |
| `existeAresta` / `obtemPeso` | O(grau(v)) | O(1) |

---

## O algoritmo

### Prim com min-heap indexado

A fila de prioridade guarda a **chave** (menor peso de aresta que liga cada vértice à árvore). Como Prim precisa **diminuir a chave** de um vértice que já está no heap, o heap mantém dois vetores auxiliares:

- `vertice[i]` → qual vértice está na posição `i` do heap;
- `indice[v]` → em que posição do heap está o vértice `v`.

Assim `DiminuiChave` localiza o vértice em O(1) e só paga o custo da subida (`O(log V)`). A cada iteração:

1. `removeMin` extrai o vértice de menor chave (troca com o último, diminui o tamanho e executa `MinHeapify`);
2. para cada vizinho ainda no heap, se o peso da aresta é menor que a chave atual, chama `DiminuiChave` e atualiza o antecessor `ant[]`;
3. a aresta `(ant[p], p)` é registrada na AGM.

Complexidade teórica de Prim com heap binário: **O(E log V)**. Na prática, `obtemPeso` na versão em lista percorre a lista de adjacência, então o custo real depende do grau dos vértices.

### Vértice mais distante

Depois da AGM pronta, uma **busca em profundidade restrita às arestas da AGM** (`VisitaMD`) acumula a distância a partir do vértice `0`; o vértice de maior distância acumulada é impresso.

### Validação de entrada

Em `LeGrafo`, cada aresta é verificada antes da inserção. Se houver qualquer erro, a saída contém **somente as mensagens de erro** e o programa termina sem calcular a AGM:

| Mensagem | Significado |
|---|---|
| `ERRO: VERTICE INVALIDO (x)` | vértice fora do intervalo `[0, V-1]` |
| `ERRO: AUTO-LACO (x,x)` | aresta de um vértice para ele mesmo |
| `ERRO: ARESTA PARALELA (a,b)` | aresta repetida |
| `ERRO: PESO INVALIDO (p)` | peso negativo |
| `ERRO: GRAFO NAO CONECTADO` | não existe AGM (checado por busca em profundidade) |

---

## Formato dos arquivos

**Entrada**

```
V E
u1 v1 peso1
u2 v2 peso2
...
```

Vértices numerados de `0` a `V-1`; o grafo é **não direcionado**.

**Saída (grafo válido)**

```
<peso total da AGM>
<vértice mais distante de 0 na AGM>
<u> <v> <peso>      (uma linha por aresta da AGM, com u < v, em ordem crescente)
```

---

## Como compilar e executar

Requer `gcc` e `make`.

```bash
make                 # gera ep_lista e ep_matriz
make teste           # roda o exemplo nas duas versões e compara com a saída esperada
make clean
```

Ou manualmente:

```bash
gcc -Wall -O2 EP.1.c -o ep_lista                 # lista de adjacência
gcc -Wall -O2 -DMATRIZ EP.1.c -o ep_matriz       # matriz de adjacência

./ep_lista entrada.txt saida.txt
```

O programa recebe exatamente dois argumentos: arquivo de entrada e arquivo de saída.

---

## Exemplo

`exemplos/entrada_valida.txt`

```
5 7
0 1 2.0
0 3 6.0
1 2 3.0
1 3 8.0
1 4 5.0
2 4 7.0
3 4 9.0
```

Saída:

```
16.0
4
0 1 2.0
0 3 6.0
1 2 3.0
1 4 5.0
```

Peso total `2 + 6 + 3 + 5 = 16`; o vértice `4` é o mais distante de `0` na árvore (distância `2 + 5 = 7`).

Exemplo de entrada inválida (`exemplos/entrada_invalida.txt`) e sua saída:

```
ERRO: AUTO-LACO (1,1)
ERRO: ARESTA PARALELA (0,1)
```

---

## Limitações conhecidas

- A raiz do Prim é fixa em `0` (o heap inicial só é válido com a raiz na posição 0).
- A versão em **matriz** aceita no máximo **100 vértices** (`MAXNUMVERTICE`).
- `removeAresta` remove apenas um sentido da aresta (o grafo guarda cada aresta nos dois sentidos) e não é usada pelo programa principal.

## Ideias de evolução

- Separar interfaces em arquivos `.h` e compilar cada módulo em `.o` (hoje os `.c` são incluídos diretamente).
- Comparar o desempenho das duas representações em grafos esparsos e densos.

---

## Contexto acadêmico

Trabalho desenvolvido como exercício-programa na **Escola de Artes, Ciências e Humanidades (EACH) – USP Leste**, no curso de Sistemas de Informação, sobre **grafos, TADs/modularização e árvores geradoras mínimas**.
