#include "quicksort.h"

static inline void trocar(int *a, int *b, Metricas *m) {
    int temp = *a;
    *a = *b;
    *b = temp;
    m->movimentacoes += 3; // Convenção: 1 swap = 3 movimentações
}

static void insertion_sort(int *arr, int low, int high, Metricas *m) {
    for (int i = low + 1; i <= high; i++) {
        int chave = arr[i];
        m->movimentacoes++;
        int j = i - 1;
        while (j >= low) {
            m->comparacoes++;
            if (arr[j] > chave) {
                arr[j + 1] = arr[j];
                m->movimentacoes++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = chave;
        m->movimentacoes++;
    }
}

// Particionamento de Lomuto (pivô no final: arr[high])
static int particionar(int *arr, int low, int high, Metricas *m) {
    int pivo = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        m->comparacoes++;
        if (arr[j] <= pivo) {
            i++;
            if (i != j) {
                trocar(&arr[i], &arr[j], m);
            }
        }
    }
    if (i + 1 != high) {
        trocar(&arr[i + 1], &arr[high], m);
    }
    return i + 1;
}

// Coloca a mediana entre low, mid e high na posição high (para reusar Lomuto)
static void preparar_mediana_tres(int *arr, int low, int high, Metricas *m) {
    int mid = low + (high - low) / 2;

    m->comparacoes++;
    if (arr[mid] < arr[low]) trocar(&arr[low], &arr[mid], m);

    m->comparacoes++;
    if (arr[high] < arr[low]) trocar(&arr[low], &arr[high], m);

    m->comparacoes++;
    if (arr[mid] < arr[high]) trocar(&arr[mid], &arr[high], m);
    // Agora arr[low] <= arr[high] (mediana) <= arr[mid]
}

void quicksort_tradicional(int *arr, int low, int high, Metricas *m) {
    if (low < high) {
        int pi = particionar(arr, low, high, m);
        quicksort_tradicional(arr, low, pi - 1, m);
        quicksort_tradicional(arr, pi + 1, high, m);
    }
}

void quicksort_hibrido(int *arr, int low, int high, int M, Metricas *m) {
    if (low < high) {
        if ((high - low + 1) < M) {
            insertion_sort(arr, low, high, m);
        } else {
            int pi = particionar(arr, low, high, m);
            quicksort_hibrido(arr, low, pi - 1, M, m);
            quicksort_hibrido(arr, pi + 1, high, M, m);
        }
    }
}

void quicksort_mediana3(int *arr, int low, int high, int M, Metricas *m) {
    if (low < high) {
        if ((high - low + 1) < M) {
            insertion_sort(arr, low, high, m);
        } else {
            preparar_mediana_tres(arr, low, high, m);
            int pi = particionar(arr, low, high, m);
            quicksort_mediana3(arr, low, pi - 1, M, m);
            quicksort_mediana3(arr, pi + 1, high, M, m);
        }
    }
}