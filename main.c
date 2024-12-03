#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM_SOLUCAO 280 /* MUDAR AQUI QNT DE NOS LIDOS */




typedef struct{
    int x, y;
    int visitado;
}Cidades;

Cidades cidades[TAM_SOLUCAO];

float matrizDistancias[TAM_SOLUCAO][TAM_SOLUCAO];

int conjuntoSolucao[TAM_SOLUCAO+1];
float distTotal, distAux;


//////////////////////////////////////////////////////////////////////////////////
// FUNCOES AUXILIARES

void mostrarCidades()
{
    for(int i = 0; i < TAM_SOLUCAO; i++)
    {
        printf("%d: (%d,%d)\n", i+1, cidades[i].x, cidades[i].y);
    }
}

void mostrarDistancias()
{
    for (int i = 0; i < TAM_SOLUCAO; i++)
    {
        printf("\nDistancias cidade %d\n", i+1);

        for (int j = 0; j < TAM_SOLUCAO; j++)
        {
            printf("%d->%d: %.2f\n", i+1, j+1, matrizDistancias[i][j]);
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

        int i = 0, idAux;
        char linhaAux[50];

        // pula as 6 primeiras linhas 
        while (i < 6)
        {
            fgets(linhaAux, 50, arq);     
            i++;
        }
        

        i = 0;
        while(i < TAM_SOLUCAO){
            fscanf(arq, "%d %d %d", &idAux, &cidades[i].x, &cidades[i].y);
            cidades[i].visitado = 0;

            i++;
        }

        fclose(arq);
    }
}

void calcularDistancias()
{
    float dist;
    int  xa, ya,
         xb, yb;

    for (int i = 0; i < TAM_SOLUCAO; i++)
    {
        xa = cidades[i].x; ya = cidades[i].y;
        
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
    int i = 0, j;
    int noAtual = 0, vizMaisProx, aux = 0;

    /* Primeira cidade escolhida para iniciar o algoritmo */
    cidades[0].visitado = 1;
    conjuntoSolucao[0] = 1; /* OBS: cidades vão de 1 até TAMSOLUCAO */

    while (i < TAM_SOLUCAO-1)
    {
        distAux = __FLT_MAX__;

        for (j = 1; j < TAM_SOLUCAO; j++)
        {
            //printf("|| Para noh %d: \n", noAtual);
            //printf("%.2f\n", distAux);
            //printf("%d\n", cidades[j].visitado);
            //printf("%.2f para noh - %d||\n", matrizDistancias[noAtual][j], j+1);

            if(distAux > matrizDistancias[noAtual][j] && cidades[j].visitado == 0)
            {
                distAux = matrizDistancias[noAtual][j];
                vizMaisProx = j; 
            }
        }
        cidades[vizMaisProx].visitado = 1;
        distTotal += distAux;
        //printf("viz escolhido: %d & distancia total: %.2f \n\n", vizMaisProx+1, distTotal);

        conjuntoSolucao[++i] = vizMaisProx+1;
        noAtual = vizMaisProx; 
    }

    i = 0;
    printf("\n\nDistancia total: %.2f\n", distTotal);
    printf("Conjunto solucao: (");
    while(i < TAM_SOLUCAO-1)
        printf(" %d,", conjuntoSolucao[i++]);
    printf(" %d)\n", conjuntoSolucao[i]);
}









//////////////////////////////////////////////////////////////////////////////////
// MAIN

void main(int argc, char *argv[])
{

    printf("\nArquivo aberto com sucesso!\nIniciando solucao VIZINHO MAIS PROXIMO para PCV:\n\n");

    lerArquivo(argv[1]);
    mostrarCidades();
    calcularDistancias();
    //mostrarDistancias();

    vizinho_mais_proximo();

}