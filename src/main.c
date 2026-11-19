#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quicksort.h"
#include "gerador.h"

#define REPETICOES 5

static double obter_tempo_ms(struct timespec inicio, struct timespec fim) {
    return (fim.tv_sec - inicio.tv_sec) * 1000.0 +
           (fim.tv_nsec - inicio.tv_nsec) / 1e6;
}

// Etapa 1: Calibração de M em entradas próprias (separadas da comparação final)
int calibrar_M(void) {
    int candidatos_M[] = {5, 10, 20, 30, 50};
    int qtd_M = 5;
    int n_calibracao = 20000;

    int *orig = malloc(n_calibracao * sizeof(int));
    int *copia = malloc(n_calibracao * sizeof(int));

    FILE *csv = fopen("resultados/calibracao_M.csv", "w");
    fprintf(csv, "M,tempo_medio_ms,comparacoes,movimentacoes\n");

    int melhor_M = candidatos_M[0];
    double menor_tempo = 1e18;

    printf("=== ETAPA 1: CALIBRACAO DE M (N = %d) ===\n", n_calibracao);

    for (int i = 0; i < qtd_M; i++) {
        int M = candidatos_M[i];
        double soma_tempo = 0.0;
        uint64_t soma_comp = 0, soma_mov = 0;

        for (int r = 0; r < REPETICOES; r++) {
            // Seed diferente da usada nos testes finais
            gerar_vetor(orig, n_calibracao, PERFIL_ALEATORIO, 9000 + r);
            memcpy(copia, orig, n_calibracao * sizeof(int));

            Metricas m = {0, 0};
            struct timespec t0, t1;

            clock_gettime(CLOCK_MONOTONIC, &t0);
            quicksort_hibrido(copia, 0, n_calibracao - 1, M, &m);
            clock_gettime(CLOCK_MONOTONIC, &t1);

            if (!esta_ordenado(copia, n_calibracao)) {
                fprintf(stderr, "FALHA na ordenacao durante calibracao (M=%d)\n", M);
                exit(1);
            }

            soma_tempo += obter_tempo_ms(t0, t1);
            soma_comp += m.comparacoes;
            soma_mov += m.movimentacoes;
        }

        double media_tempo = soma_tempo / REPETICOES;
        printf("M = %2d | Tempo medio: %8.4f ms | Comp: %lu | Mov: %lu\n",
               M, media_tempo, soma_comp / REPETICOES, soma_mov / REPETICOES);
        fprintf(csv, "%d,%.6f,%lu,%lu\n",
                M, media_tempo, soma_comp / REPETICOES, soma_mov / REPETICOES);

        if (media_tempo < menor_tempo) {
            menor_tempo = media_tempo;
            melhor_M = M;
        }
    }

    fclose(csv);
    free(orig);
    free(copia);
    printf("-> Melhor M selecionado: %d\n\n", melhor_M);
    return melhor_M;
}

// Etapa 2: Experimento principal comparando as 3 versões
void executar_experimentos(int M_escolhido) {
    int tamanhos[] = {1000, 5000, 10000, 50000};
    int qtd_tamanhos = 4;
    PerfilEntrada perfis[] = {
        PERFIL_ALEATORIO,
        PERFIL_ORDENADO,
        PERFIL_INVERSO,
        PERFIL_REPETIDOS,
        PERFIL_PIOR_CASO
    };
    int qtd_perfis = 5;

    FILE *csv = fopen("resultados/resultados_experimentos.csv", "w");
    fprintf(csv, "perfil,tamanho,versao,M,tempo_medio_ms,comparacoes_medias,movimentacoes_medias\n");

    printf("=== ETAPA 2: EXPERIMENTOS COMPARATIVOS (M = %d) ===\n", M_escolhido);

    for (int p = 0; p < qtd_perfis; p++) {
        for (int t = 0; t < qtd_tamanhos; t++) {
            int n = tamanhos[t];
            int *orig = malloc(n * sizeof(int));
            int *copia = malloc(n * sizeof(int));

            double tempo_acc[3] = {0.0, 0.0, 0.0};
            uint64_t comp_acc[3] = {0, 0, 0};
            uint64_t mov_acc[3] = {0, 0, 0};

            for (int r = 0; r < REPETICOES; r++) {
                gerar_vetor(orig, n, perfis[p], 100 + r);

                for (int v = 0; v < 3; v++) {
                    memcpy(copia, orig, n * sizeof(int));
                    Metricas m = {0, 0};
                    struct timespec t0, t1;

                    clock_gettime(CLOCK_MONOTONIC, &t0);
                    if (v == 0) quicksort_tradicional(copia, 0, n - 1, &m);
                    else if (v == 1) quicksort_hibrido(copia, 0, n - 1, M_escolhido, &m);
                    else quicksort_mediana3(copia, 0, n - 1, M_escolhido, &m);
                    clock_gettime(CLOCK_MONOTONIC, &t1);

                    if (!esta_ordenado(copia, n)) {
                        fprintf(stderr, "FALHA: vetor nao ordenado (Perfil=%s, N=%d, Versao=%d)\n",
                                nome_perfil(perfis[p]), n, v);
                        exit(1);
                    }

                    tempo_acc[v] += obter_tempo_ms(t0, t1);
                    comp_acc[v] += m.comparacoes;
                    mov_acc[v] += m.movimentacoes;
                }
            }

            const char *nomes_versoes[3] = {"Tradicional", "Hibrido", "Hibrido_Mediana3"};
            for (int v = 0; v < 3; v++) {
                double t_med = tempo_acc[v] / REPETICOES;
                uint64_t c_med = comp_acc[v] / REPETICOES;
                uint64_t m_med = mov_acc[v] / REPETICOES;

                printf("%-21s | N=%6d | %-16s | %8.4f ms\n",
                       nome_perfil(perfis[p]), n, nomes_versoes[v], t_med);
                fprintf(csv, "%s,%d,%s,%d,%.6f,%lu,%lu\n",
                        nome_perfil(perfis[p]), n, nomes_versoes[v], M_escolhido,
                        t_med, c_med, m_med);
            }

            free(orig);
            free(copia);
        }
    }

    fclose(csv);
    printf("\nConcluido! Arquivos salvos em 'resultados/'.\n");
}

int main(void) {
    int melhor_M = calibrar_M();
    executar_experimentos(melhor_M);
    return 0;
}