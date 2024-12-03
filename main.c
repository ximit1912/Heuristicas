#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM_SOLUCAO 10 /* MUDAR AQUI QNT DE NOS LIDOS */




typedef struct{
    int x, y;
}Cidades;

Cidades cidades[TAM_SOLUCAO];

float matrizDistancias[TAM_SOLUCAO][TAM_SOLUCAO];

Cidades conjuntoSolucao[TAM_SOLUCAO];


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
            printf("%d%d: %.2f\n", i+1, j+1, matrizDistancias[i][j]);
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

// int vizinho_mais_proximo():
// matriz   M 280x280 contendo as distancias entre os elementos
// iniciar com uma cidade (vertice) arbitrário como atual
// percorrer a linha correspondente da cidade procurando a menor distancia
    // escolher a cidade c/ menor distancia (e somar a distancia total atual)
// percorrer a partir dessa cidade da mesma forma, e assim por diante até visitar todas
    // salvar a d


//////////////////////////////////////////////////////////////////////////////////
// MAIN

void main(int argc, char *argv[])
{

    printf("\nArquivo aberto com sucesso!\nIniciando solucao VIZINHO MAIS PROXIMO para PCV:\n\n");

    lerArquivo(argv[1]);
    mostrarCidades();
    calcularDistancias();
    mostrarDistancias();

    // vizinho_mais_proximo(;

}