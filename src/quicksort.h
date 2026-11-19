#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdint.h>

typedef struct {
    uint64_t comparacoes;
    uint64_t movimentacoes;
} Metricas;

// Versão (a): Quicksort tradicional (pivô = último elemento / Lomuto)
void quicksort_tradicional(int *arr, int low, int high, Metricas *m);

// Versão (b): Quicksort híbrido com Insertion Sort para subvetores < M
void quicksort_hibrido(int *arr, int low, int high, int M, Metricas *m);

// Versão (c): Quicksort híbrido com Mediana de Três
void quicksort_mediana3(int *arr, int low, int high, int M, Metricas *m);

#endif