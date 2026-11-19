#include "gerador.h"
#include <stdlib.h>

const char* nome_perfil(PerfilEntrada perfil) {
    switch (perfil) {
        case PERFIL_ALEATORIO: return "Aleatorio";
        case PERFIL_ORDENADO:  return "Ordenado";
        case PERFIL_INVERSO:   return "Inverso";
        case PERFIL_REPETIDOS: return "Repetidos";
        case PERFIL_PIOR_CASO: return "Pior_Caso_Tradicional";
        default:               return "Desconhecido";
    }
}

void gerar_vetor(int *arr, int n, PerfilEntrada perfil, unsigned int seed) {
    srand(seed);
    switch (perfil) {
        case PERFIL_ALEATORIO:
            for (int i = 0; i < n; i++) arr[i] = rand() % (n * 10);
            break;
        case PERFIL_ORDENADO:
            for (int i = 0; i < n; i++) arr[i] = i;
            break;
        case PERFIL_INVERSO:
            for (int i = 0; i < n; i++) arr[i] = n - i;
            break;
        case PERFIL_REPETIDOS:
            // Apenas 10 valores distintos para forçar muitos repetidos
            for (int i = 0; i < n; i++) arr[i] = rand() % 10;
            break;
        case PERFIL_PIOR_CASO:
            // Com pivô no último elemento (Lomuto), o vetor já ordenado
            // gera partições de tamanho (n-1) e 0 em todas as chamadas (O(n^2)).
            for (int i = 0; i < n; i++) arr[i] = i + 1;
            break;
    }
}

int esta_ordenado(const int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}