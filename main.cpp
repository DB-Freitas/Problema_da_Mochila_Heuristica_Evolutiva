#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <iostream>
#include <algorithm>

using namespace std;

typedef struct {
    unsigned int peso;
    int beneficio;
} type_item;

// Função para gerar um indivíduo aleatório.
bool * gerar_individuo_aleatorio(int n, type_item * itens, int capacidade) {
    bool * individuo = (bool *)malloc(n * sizeof(bool));// Aloca espaço de memória para o vetor que representa o indivíduo.
    int peso_total = 0;// Variável para rastrear o peso total dos itens selecionados para a mochila.

    // Loop para percorrer os itens e decidir se eles devem ser incluídos ou não no indivíduo.
    for (int i = 0; i < n; i++) {// Loop para percorrer os itens e decidir se eles devem ser incluídos ou não no indivíduo.
        // Verifica se o item cabe na mochila (não excede a capacidade restante).
        if (peso_total + itens[i].peso <= capacidade) {
            individuo[i] = true; // O item é selecionado para a mochila.
            peso_total += itens[i].peso; // Atualiza o peso total da mochila.
        } else {
            individuo[i] = false; // O item não cabe na mochila.
        }
    }
    return individuo; // Retorna o ponteiro para o vetor do indivíduo gerado aleatoriamente.
}

// Função para calcular os pesos e benefícios totais de um indivíduo.
int * calcular_pesos_e_beneficios(const bool * individuo, const type_item * itens, int n) {
    int * pesos_e_beneficios = (int *)malloc(2 * sizeof(int)); // Aloca espaço de memória para o vetor que armazenará o peso total e o benefício total do indivíduo.
    
    // Inicializa as variáveis para armazenar o peso total e o benefício total do indivíduo como zero.
    pesos_e_beneficios[0] = 0;
    pesos_e_beneficios[1] = 0;
    // Loop para percorrer os itens do indivíduo.
    for (int i = 0; i < n; i++) {
        // Se o item estiver presente no indivíduo (valor booleano verdadeiro).
        if (individuo[i]) {            
            pesos_e_beneficios[0] += itens[i].peso; // Adiciona o peso do item ao peso total do indivíduo.
            pesos_e_beneficios[1] += itens[i].beneficio; // Adiciona o benefício do item ao benefício total do indivíduo.
        }
    }
    return pesos_e_beneficios; // Retorna o ponteiro para o vetor com o peso total e o benefício total.
}


// Função para realizar o cruzamento (crossover) de dois indivíduos.
bool * cruzar(const bool * pai1, const bool * pai2, int n) {
    // Aloca espaço de memória para o vetor que representa o filho gerado pelo cruzamento.
    bool * filho = (bool *)malloc(n * sizeof(bool));
    // Determina um ponto de corte aleatório para o cruzamento.
    int ponto_corte = rand() % n;
    // Loop para realizar o cruzamento dos pais e gerar o filho.
    for (int i = 0; i < n; i++) {
        if (i < ponto_corte) {
            filho[i] = pai1[i]; // Copia o gene do pai 1 para o filho.
        } else {
            filho[i] = pai2[i]; // Copia o gene do pai 2 para o filho.
        }
    }
    return filho; // Retorna o ponteiro para o vetor que representa o filho gerado.
}

// Função para realizar a mutação em um indivíduo.
void mutar(bool * individuo, int n, double taxa_mutacao) {
    // Loop para percorrer cada gene do indivíduo.
    for (int i = 0; i < n; i++) {
        double probabilidade = (double)rand() / RAND_MAX; // Gera uma probabilidade aleatória entre 0 e 1.
        
        if (probabilidade < taxa_mutacao) { // Se a probabilidade for menor que a taxa de mutação, realiza a mutação no gene.
            individuo[i] = !individuo[i]; // Inverte o valor do gene (de true para false, ou vice-versa).
        }
    }
}

// Função para encontrar o melhor indivíduo em uma população com base nos benefícios.
bool * encontrar_melhor_individuo(bool ** populacao, int * beneficios, int tamanho_populacao) {
    // Encontra o índice do melhor indivíduo com base no vetor de benefícios.
    int melhor_indice = distance(beneficios, max_element(beneficios, beneficios + tamanho_populacao));

    return populacao[melhor_indice]; // Retorna o ponteiro para o vetor que representa o melhor indivíduo.
}

// Função principal do algoritmo evolutivo para resolver o problema da mochila.
int algoritmo_evolutivo(type_item itens[], int n_itens, int capacidade, int tamanho_populacao, int num_geracoes, double taxa_cruzamento, double taxa_mutacao) {
    clock_t inicia_tempo, finaliza_tempo;
    inicia_tempo = clock();
    // Aloca espaço de memória para a matriz que representa a população de indivíduos.
    bool ** populacao = (bool **)malloc(tamanho_populacao * sizeof(bool *));
    // Aloca espaço de memória para o vetor que armazena os benefícios dos indivíduos.
    int * beneficios = (int *)malloc(tamanho_populacao * sizeof(int));

    // Inicializa a semente aleatória.
    srand(time(NULL));

    // Ordena os itens em ordem decrescente de benefício.
    sort(itens, itens + n_itens, [](const type_item &a, const type_item &b) {
        return a.beneficio > b.beneficio;
    });

    // Gera a população inicial de indivíduos.
    for (int i = 0; i < tamanho_populacao; i++) {
        populacao[i] = gerar_individuo_aleatorio(n_itens, itens, capacidade);
    }
    // Loop para as gerações do algoritmo evolutivo.
    for (int g = 0; g < num_geracoes; g++) {
        // Loop para cada indivíduo na população.
        for (int i = 0; i < tamanho_populacao; i++) {
            bool * individuo = populacao[i];
            
            // Verifica se o indivíduo será submetido ao cruzamento.
            if ((double)rand() / RAND_MAX < taxa_cruzamento) {
                // Seleciona aleatoriamente um parceiro para cruzamento.
                int indice_parceiro = rand() % tamanho_populacao;
                bool *parceiro = populacao[indice_parceiro];
                // Realiza o cruzamento entre o indivíduo e o parceiro.
                individuo = cruzar(individuo, parceiro, n_itens);
            }
            // Verifica se o indivíduo será submetido à mutação.
            if ((double)rand() / RAND_MAX < taxa_mutacao) {
                // Realiza a mutação no indivíduo.
                mutar(individuo, n_itens, taxa_mutacao);
            }
            // Calcula os pesos e benefícios do indivíduo após o cruzamento e a mutação.
            int * pesos_e_beneficios = calcular_pesos_e_beneficios(individuo, itens, n_itens);
            int peso_total = pesos_e_beneficios[0];
            int beneficio_total = pesos_e_beneficios[1];

            // Verifica se o indivíduo respeita a restrição de capacidade da mochila e é melhor que o pai.
            if (peso_total <= capacidade && beneficios[i] <= beneficio_total) {
                populacao[i] = individuo; // Atualiza o indivíduo na população.
                beneficios[i] = beneficio_total; // Atualiza os benefícios do indivíduo.
            }
        }
    }
    // Encontra o melhor indivíduo na população.
    bool * melhor_individuo = encontrar_melhor_individuo(populacao, beneficios, tamanho_populacao);
    // Calcula os pesos e benefícios do melhor indivíduo encontrado.
    int * pesos_e_beneficios_melhor = calcular_pesos_e_beneficios(melhor_individuo, itens, n_itens);

    finaliza_tempo = clock();
    printf("\n\nTempo de Execucaoo(HEURISTICA EVOLUTIVA): %f segundos\n", (float)(finaliza_tempo - inicia_tempo) / CLOCKS_PER_SEC);
    return pesos_e_beneficios_melhor[1]; // Retorna o benefício do melhor indivíduo encontrado.
}


void imprimir_items(type_item *items, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("Peso: %u, Beneficio: %d\n", items[i].peso, items[i].beneficio);
    }
}

type_item * ler_items(char *filename, int *n_items, int *capacidad) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        puts("Nao foi possível abrir o arquivo");
        exit(0);
    }
    fscanf(fp, "%d %d", n_items, capacidad);
    printf("Numero de elementos: %d, Capacidade: %d\n", *n_items, *capacidad);

    type_item *items = (type_item *)malloc(*n_items * sizeof(type_item));

    for (int i = 0; i < *n_items; i++) {
        fscanf(fp, "%d %d", &items[i].peso, &items[i].beneficio);
    }
    fclose(fp);

    return items;
}

int main() {
    int n_items, capacidad;

    char filename[100] = "arquivos_de_teste/knapPI_2_10000_1000_1";

    type_item *items = ler_items(filename, &n_items, &capacidad);
    imprimir_items(items, n_items);

    cout << algoritmo_evolutivo(items, n_items, capacidad, 100, 100, 0.7, 0.01) << endl;
    
    free(items);
    return 0;
}

//Teste Algoritmo Evolutivo: 
//parametros: população: 100, gerações: 100, taxa reprodução: 0.7, taxa Mut: 0.01



