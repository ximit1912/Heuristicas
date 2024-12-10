#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM_SOLUCAO 33810 /* MUDAR AQUI QNT DE NOS LIDOS */

int noInicial = 0;
unsigned long int distTotal, distAux;

typedef struct{
    float x, y;
    int visitado;
}Cidades;

int tamDiagSup = (TAM_SOLUCAO * (TAM_SOLUCAO-1))/2; 
/* QNT DE ELEMENTOS A SEREM PERCORRIDOS DO VETOR DE CUSTOS, NELE SÃO RETIRADAS AS 
   REDUNDANCIAS DA MATRIZ DE CUSTOS Ci,i E Ci,j == Cj,i (economizar memória!) */
int *vetorDistancias;
/* PONTEIRO PARA O VETOR QUE FICARÃO AS DISTANCIAS */



//////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCOES AUXILIARES ///////////////////////////////////////////////////////////////////////////////


/* FUNÇÃO QUE CONVERTE PARA A POSICAO CORRESPONDENTE DO vetorDistancias ONDE SE ENCONTRA
   A DISTANCIA CORRETA ENTRE 2 NÓS/CIDADES i,j  */
int converteIndice(float i, float j)
{
    int indice;
    if(i == j)
    {
        indice = -1;
    }
    else if(i > j) /* então troca i com j */
        {
            indice = ((j * (2 * TAM_SOLUCAO - j - 1)) / 2) + (i - j - 1);
        }
        else
            {
                indice = ((i * (2 * TAM_SOLUCAO - i - 1)) / 2) + (j - i - 1);
            }
    return indice;
}

int calculaDistancia(Cidades a, Cidades b) {
    int xa = (int)a.x, ya = (int)a.y;
    int xb = (int)b.x, yb = (int)b.y;
    return sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));
}


void mostrarCidades(Cidades *cidades)
{
    for(int i = 0; i < TAM_SOLUCAO; i++)
    {
        printf("%d: (%d,%d)\n", i+1, (int) cidades[i].x, (int) cidades[i].y);
    }
}

void mostrarDistancias()
{
    for (int i = 0; i < TAM_SOLUCAO; i++)
    {
        printf("\nDistancias cidade %d\n", i+1);

        for (int j = 0; j < TAM_SOLUCAO; j++)
        {
            printf("%d->%d: %d\n", i+1, j+1, vetorDistancias[converteIndice(i, j)]);
        }

        printf("\n");
    }
}


void calcularDistancias(Cidades *cidades)
{
    int dist, i, j, z=0; /* indices, z para o vetor de distancias */
    int  xa, ya,
         xb, yb;

    for(i = 0; i < TAM_SOLUCAO - 1; i++)
    {
        xa = (int) cidades[i].x; ya = (int) cidades[i].y;
        
        for (j = i+1; j < TAM_SOLUCAO; j++)
        {
            xb = cidades[j].x; yb = cidades[j].y;
            dist = sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));

            vetorDistancias[z] = dist;
            z++;     
        }
        
    }
}


void lerArquivo(char *nome, Cidades *cidades)
{
    FILE *arq = fopen(nome,"r");
    if(arq == NULL)
    {
        fprintf(stderr, "\nArquivo não foi aberto! Abortando programa! ");
        EXIT_FAILURE;
    }
    else
    {
        printf("\nArquivo aberto com sucesso!\nLendo cidades e calculando distancias:\n\n");

        int i = 0, idAux;
        float x,y;
        char linhaAux[60];

        // pula as 6 primeiras linhas dos arquivos.tsp
        while (i < 6)
        {
            fgets(linhaAux, sizeof(linhaAux), arq);     
            i++;
        }
        
        // le as cidades (x, y, visitado=0)
        i = 0;
        while(i < TAM_SOLUCAO)
        {
            fscanf(arq, "%d %f %f", &idAux, &cidades[i].x, &cidades[i].y);
            cidades[i].visitado = 0;
            i++;
        }

        /* calcularDistancias(cidades);  PARA SALVAR AS DISTANCIAS NUM VETOR*/
    }

    fclose(arq);
}



//////////////////////////////////////////////////////////////////////////////////
// FUNCOES PRINCIPAIS

void vizinho_mais_proximo(Cidades *cidades, int *conjuntoSolucao)
{
    distTotal = 0, distAux = 0;
    int i = 0, distancia;

    /* Primeira cidade escolhida para iniciar o algoritmo, ESCOLHA ALTERANDO 'noAtual' */
    int noAtual = noInicial, vizMaisProx;
    cidades[noAtual].visitado = 1;
    conjuntoSolucao[0] = noAtual+1; /* OBS: cidades vão de 1 até TAMSOLUCAO */

    while (i < TAM_SOLUCAO - 1)
    {
        distAux = __INT_MAX__;
        // printf("|| Viz + prox do noh %d: \n", noAtual+1);

        for (int j = 0; j < TAM_SOLUCAO; j++)
        {
            // PRINTFS AUXILIARES
            /* 
            printf("dist atual: %d | ", distAux);
            printf("dist para no: %d -> %d | ", j+1, distancia);
            printf("vis?: %d \n", cidades[j].visitado);
            */

            distancia = calculaDistancia(cidades[noAtual], cidades[j]);

            if(distAux > distancia && cidades[j].visitado == 0)
            {
                distAux = distancia;
                vizMaisProx = j; 
            }
        }
        cidades[vizMaisProx].visitado = 1;
        distTotal += distAux;
        // printf("##viz escolhido: %d & distancia total: %d \n\n", vizMaisProx+1, distTotal);

        conjuntoSolucao[++i] = vizMaisProx+1;
        noAtual = vizMaisProx; 
    }
    distTotal += calculaDistancia(cidades[noAtual], cidades[noInicial]); // vetorDistancias[converteIndice(noAtual,noInicial)];
    conjuntoSolucao[++i] = noInicial+1;
    
    
    i = 0;
    printf("\nDistancia total: %d\n", distTotal);
    /*   
    printf("Conjunto solucao: (");
    while(i < TAM_SOLUCAO)
        printf(" %d,", conjuntoSolucao[i++]);
    printf(" %d)\n", conjuntoSolucao[i]);
    */
}









//////////////////////////////////////////////////////////////////////////////////
// MAIN

void main(int argc, char *argv[])
{
    Cidades cidades[TAM_SOLUCAO]; /* vetor contendo as cidades */
    int conjuntoSolucao[TAM_SOLUCAO+1]; /* vetor para conter a solução (sequência de vértices de noorigem, ..., n, noorigem) */
    vetorDistancias = (int*) malloc(tamDiagSup * sizeof(int)); /* aloca memória para o vetor de distancias de acordo com o TAM_SOL */

    lerArquivo(argv[1], cidades);
    // mostrarCidades(cidades);
    // mostrarDistancias(vetorDistancias);
    vizinho_mais_proximo(cidades, conjuntoSolucao);

     /*
    int noMenor, menor = __INT_MAX__;

    while(noInicial < TAM_SOLUCAO)
    {
        // printf("\n\nInicio em %d", NOORIGEM);
        vizinho_mais_proximo(cidades, conjuntoSolucao);
        if(menor > distTotal)
        {
            menor = distTotal;
            noMenor = noInicial;
        }
        lerDistanciasArquivo(argv[1], cidades);
        distTotal = 0;

        noInicial++;
    }

    printf("\n\nNO %d - MENOR DIST: %d", noMenor, menor);
    
    */
}