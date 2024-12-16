#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM_SOLUCAO 280 /* MUDAR AQUI QNT DE NOS LIDOS */
int noInicial = 100;

unsigned long int distTotal, distAux;

// Estrutura para guardar as cidades do arquivo
typedef struct{
    float x, y;
    int visitado;
}Cidades;



// int tamDiagSup = (TAM_SOLUCAO * (TAM_SOLUCAO-1))/2; 
/* QNT DE ELEMENTOS A SEREM PERCORRIDOS DO VETOR DE CUSTOS, NELE SÃO RETIRADAS AS 
   REDUNDANCIAS DA MATRIZ DE CUSTOS Ci,i E Ci,j == Cj,i (economizar memória!) */
// int *vetorDistancias;
/* PONTEIRO PARA O VETOR QUE FICARÃO AS DISTANCIAS */



//////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCOES AUXILIARES ///////////////////////////////////////////////////////////////////////////////


/* FUNÇÃO QUE CONVERTE PARA A POSICAO CORRESPONDENTE DO vetorDistancias ONDE SE ENCONTRA
   A DISTANCIA CORRETA ENTRE 2 NÓS/CIDADES i,j  */
/*
int converteIndice(float i, float j)
{
    int indice;
    if(i == j)
    {
        indice = -1;
    }
    else if(i > j) // então troca i com j 
        {
            indice = ((j * (2 * TAM_SOLUCAO - j - 1)) / 2) + (i - j - 1);
        }
        else
            {
                indice = ((i * (2 * TAM_SOLUCAO - i - 1)) / 2) + (j - i - 1);
            }
    return indice;
}
*/


/*
void mostrarCidades(Cidades *cidades)
{
    for(int i = 0; i < TAM_SOLUCAO; i++)
    {
        printf("%d: (%d,%d)\n", i+1, (int) cidades[i].x, (int) cidades[i].y);
    }
}
*/

/*
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
*/

/*
void calcularDistancias(Cidades *cidades)
{
    int dist, i, j, z=0; // indices, z para o vetor de distancias 
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
*/

// REINICIA O INDICE DOS NOS JA VISITADOS PARA NÃO VISITADOS
void limparVisitados(Cidades *cidades)
{
    for (int i = 0; i < TAM_SOLUCAO; i++)
    {
        cidades[i].visitado = 0;
    }
}

// CALCULA DISTANCIA ENTRE CIDADE a(x,y) E b(x',y')
int calculaDistancia(Cidades a, Cidades b) {
    int xa = (int)a.x, ya = (int)a.y;
    int xb = (int)b.x, yb = (int)b.y;
    return sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));
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

// CONSTRUTIVA VIZINHO MAIS PROXIMO
void vizinhoMaisProximo(Cidades *cidades, int *conjuntoSolucao)
{
    distTotal = 0, distAux = 0;
    int i = 0, distancia;

    /* Primeira cidade escolhida para iniciar o algoritmo, ESCOLHA ALTERANDO 'noAtual' */
    int noAtual = noInicial, vizMaisProx;
    cidades[noAtual].visitado = 1;
    conjuntoSolucao[0] = noAtual+1; /* OBS: cidades vão de 1 até TAMSOLUCAO */

    printf("\n\nIniciando vizinho mais proximo ...\n");
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
    
    printf("\nDistancia total (vizinho mais proximo): %d\n", distTotal);
}
    // LAÇO PARA IMPRIMIR O CONJUNTO SOLUÇÃO
    /*   
    i = 0;

    printf("Conjunto solucao: (");
    while(i < TAM_SOLUCAO)
        printf(" %d,", conjuntoSolucao[i++]);
    printf(" %d)\n", conjuntoSolucao[i]);
    */


// CONSTRUTIVA INSERCAO MAIS PROXIMA
// CONSTRUTIVA INSERCAO MAIS PROXIMA
void insercaoMaisProxima(Cidades *cidades, int *conjuntoSolucao) 
{
    distTotal = 0; // Inicia a distância total em 0
    int i, j, k;
    int numCidadesNaSolucao = 3; // Começa com três cidades no ciclo inicial
    int cidadeMaisProxima, menorDistancia, custoInsercao, melhorInsercao;
    int no1, no2;

    printf("\n\nIniciando insercao mais proxima ...\n");

    // Passo 1: Inicializar o ciclo com três cidades
    conjuntoSolucao[0] = noInicial; 
    cidades[noInicial].visitado = 1; // Marca a cidade inicial como visitada

    // Escolher a segunda cidade (a mais próxima da inicial)
    menorDistancia = __INT_MAX__;
    for (i = 0; i < TAM_SOLUCAO; i++) {
        if (!cidades[i].visitado) {
            int dist = calculaDistancia(cidades[noInicial], cidades[i]);
            if (dist < menorDistancia) {
                menorDistancia = dist;
                cidadeMaisProxima = i;
            }
        }
    }

    conjuntoSolucao[1] = cidadeMaisProxima; 
    cidades[cidadeMaisProxima].visitado = 1; // Marca a segunda cidade como visitada
    distTotal += menorDistancia;

    // Escolher a terceira cidade (a mais próxima de qualquer uma das duas primeiras)
    menorDistancia = __INT_MAX__;
    for (i = 0; i < TAM_SOLUCAO; i++) {
        if (!cidades[i].visitado) {
            int dist1 = calculaDistancia(cidades[conjuntoSolucao[0]], cidades[i]);
            int dist2 = calculaDistancia(cidades[conjuntoSolucao[1]], cidades[i]);
            int menorDist = dist1 < dist2 ? dist1 : dist2;
            if (menorDist < menorDistancia) {
                menorDistancia = menorDist;
                cidadeMaisProxima = i;
            }
        }
    }

    conjuntoSolucao[2] = cidadeMaisProxima; 
    cidades[cidadeMaisProxima].visitado = 1; // Marca a terceira cidade como visitada
    distTotal += menorDistancia;

    // Fechar o ciclo inicial com as três cidades
    conjuntoSolucao[3] = noInicial;
    distTotal += calculaDistancia(cidades[cidadeMaisProxima], cidades[noInicial]);


    // 
    printf("Ciclo inicial com 3 cidades: ");
    for (i = 0; i < 3; i++) {
        printf("%d -> ", conjuntoSolucao[i] + 1);
    }
    printf("%d\n", conjuntoSolucao[3] + 1);

    // Passo 2: Expandir a solução iterativamente
    while (numCidadesNaSolucao < TAM_SOLUCAO) {
        menorDistancia = __INT_MAX__;
        // Encontrar o próximo nó a ser inserido (cidade mais próxima de qualquer nó no ciclo)
        for (i = 0; i < TAM_SOLUCAO; i++) {
            if (!cidades[i].visitado) {
                for (j = 0; j < numCidadesNaSolucao; j++) {
                    int dist = calculaDistancia(cidades[conjuntoSolucao[j]], cidades[i]);
                    if (dist < menorDistancia) {
                        menorDistancia = dist;
                        cidadeMaisProxima = i;
                        melhorInsercao = j;
                    }
                }
            }
        }

        // Inserir a cidade no melhor lugar no ciclo
        no1 = conjuntoSolucao[melhorInsercao];
        no2 = conjuntoSolucao[(melhorInsercao + 1) % numCidadesNaSolucao];
        custoInsercao = calculaDistancia(cidades[no1], cidades[cidadeMaisProxima]) +
                        calculaDistancia(cidades[cidadeMaisProxima], cidades[no2]) -
                        calculaDistancia(cidades[no1], cidades[no2]);

        // Insere a cidade na posição correta
        for (k = numCidadesNaSolucao; k > melhorInsercao + 1; k--) {
            conjuntoSolucao[k] = conjuntoSolucao[k - 1];
        }
        conjuntoSolucao[melhorInsercao + 1] = cidadeMaisProxima;
        cidades[cidadeMaisProxima].visitado = 1; // Marca a cidade como visitada
        distTotal += custoInsercao; // Atualiza a distância total
        numCidadesNaSolucao++;
    }

    printf("\nDistancia Total (insercao mais proxima): %d\n", distTotal);
}







//////////////////////////////////////////////////////////////////////////////////
// MAIN

void main(int argc, char *argv[])
{
    int sair = 0, opcao;
    Cidades cidades[TAM_SOLUCAO]; /* vetor contendo as cidades */
    int conjuntoSolucao[TAM_SOLUCAO+1]; /* vetor para conter a solução (sequência de n+1 vértices de noorigem, ..., noorigem) */
    // vetorDistancias = (int*) malloc(tamDiagSup * sizeof(int)); /* aloca memória para o vetor de distancias de acordo com o TAM_SOL */

    lerArquivo(argv[1], cidades);
    // mostrarCidades(cidades);
    // mostrarDistancias(vetorDistancias);
    while(!sair)
    {
        printf("\nEscolha qual heuristica construtiva voce deseja utilizar:");
        printf("\n[1] - para vizinho mais proximo");
        printf("\n[2] - para insercao mais proxima");
        printf("\n[outro] - encerrar\nDIGITE: ");
        scanf("%d", &opcao);
        if(opcao == 1)
        {
            vizinhoMaisProximo(cidades, conjuntoSolucao);

            printf("Iniciar heuristica de melhoramento 2-Opt?\n[1] - sim \n[outro] - nao\nDIGITE: ");
            scanf("%d", &opcao);
            if(opcao == 1)
            {
                // melhorativa2Opt(cidades, conjuntoSolucao) 
            }

        }
        else if(opcao == 2)
            {
                insercaoMaisProxima(cidades, conjuntoSolucao);

                printf("Iniciar heuristica de melhoramento 2-Opt?\n[1] - sim \n[outro] - nao\nDIGITE: ");
                scanf("%d", &opcao);
                if(opcao == 1)
                {
                    // melhorativa2Opt(cidades, conjuntoSolucao) 
                }
            }else
                {
                    printf("\nEncerrando...");
                    sair = 1;
                }    

        limparVisitados(cidades);
    }

}

// 1° TESTE DE MELHORAMENTO - VERIFICAR TODOS OS NÓS COMO INICIAL, RETORNAR MELHOR SOLUÇÃO
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