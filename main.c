#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM_SOLUCAO 1173 /* MUDAR AQUI QNT DE NOS LIDOS */
#define NOORIGEM 0 /* MUDAR AQUI O NO INICIAL ARBITRARIO */



typedef struct{
    float x, y;
    int visitado;
}Cidades;

Cidades cidades[TAM_SOLUCAO];

int matrizDistancias[TAM_SOLUCAO][TAM_SOLUCAO];

int conjuntoSolucao[TAM_SOLUCAO+1];
unsigned long int distTotal, distAux;


//////////////////////////////////////////////////////////////////////////////////
// FUNCOES AUXILIARES

void mostrarCidades()
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
            printf("%d->%d: %d\n", i+1, j+1, matrizDistancias[i][j]);
        }

        printf("\n");
    }
}


void lerArquivo(char *nome)
{
    FILE *arq = fopen(nome,"r");
    if(arq == NULL)
    {
        fprintf(stderr, "\nArquivo não foi aberto! Abortando programa! ");
        EXIT_FAILURE;
    }
    else
    {
        printf("\nArquivo aberto com sucesso!\nIniciando solucao VIZINHO MAIS PROXIMO para PCV:\n\n");

        int i = 0, idAux;
        char linhaAux[60];

        // pula as 6 primeiras linhas 
        while (i < 6)
        {
            fgets(linhaAux, sizeof(linhaAux), arq);     
            i++;
        }
        

        i = 0;
        while(i < TAM_SOLUCAO){
            fscanf(arq, "%d %f %f", &idAux, &cidades[i].x, &cidades[i].y);
            cidades[i].visitado = 0;

            i++;
        }

        fclose(arq);
    }
}

void calcularDistancias()
{
    int dist;
    int  xa, ya,
         xb, yb;

    for (int i = 0; i < TAM_SOLUCAO; i++)
    {
        xa = (int) cidades[i].x; ya = (int) cidades[i].y;
        
        for (int j = 0; j < TAM_SOLUCAO; j++)
        {
            if(i == j)
                dist = 0;
            else
            {
                xb = cidades[j].x; yb = cidades[j].y;
                dist = sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));
            }

            matrizDistancias[i][j] = dist;     
        }
        
    }
}


//////////////////////////////////////////////////////////////////////////////////
// FUNCOES PRINCIPAIS

void vizinho_mais_proximo()
{
    distTotal = 0, distAux = 0;
    int i = 0;

    /* Primeira cidade escolhida para iniciar o algoritmo, ESCOLHA ALTERANDO 'noAtual' */
    int noAtual = NOORIGEM, vizMaisProx;
    cidades[noAtual].visitado = 1;
    conjuntoSolucao[0] = noAtual+1; /* OBS: cidades vão de 1 até TAMSOLUCAO */

    while (i < TAM_SOLUCAO-1)
    {
        distAux = __INT_MAX__;

        for (int j = 0; j < TAM_SOLUCAO; j++)
        {
            //printf("|| Para noh %d: \n", noAtual);
            //printf("%d\n", distAux);
            //printf("%d\n", cidades[j].visitado);
            //printf("%d para noh - %d||\n", matrizDistancias[noAtual][j], j+1);

            if(distAux > matrizDistancias[noAtual][j] && cidades[j].visitado == 0)
            {
                distAux = matrizDistancias[noAtual][j];
                vizMaisProx = j; 
            }
        }
        cidades[vizMaisProx].visitado = 1;
        distTotal += distAux;
        //printf("viz escolhido: %d & distancia total: %d \n\n", vizMaisProx+1, distTotal);

        conjuntoSolucao[++i] = vizMaisProx+1;
        noAtual = vizMaisProx; 
    }

    i = 0;
    printf("\n\nDistancia total: %d\n", distTotal);
    printf("Conjunto solucao: (");
    while(i < TAM_SOLUCAO-1)
        printf(" %d,", conjuntoSolucao[i++]);
    printf(" %d)\n", conjuntoSolucao[i]);
}









//////////////////////////////////////////////////////////////////////////////////
// MAIN

void main(int argc, char *argv[])
{
    lerArquivo(argv[1]);
    //mostrarCidades();
    calcularDistancias();
    //mostrarDistancias();

    vizinho_mais_proximo();

}