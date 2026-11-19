#ifndef GERADOR_H
#define GERADOR_H

typedef enum {
    PERFIL_ALEATORIO = 0,
    PERFIL_ORDENADO,
    PERFIL_INVERSO,
    PERFIL_REPETIDOS,
    PERFIL_PIOR_CASO
} PerfilEntrada;

const char* nome_perfil(PerfilEntrada perfil);
void gerar_vetor(int *arr, int n, PerfilEntrada perfil, unsigned int seed);
int esta_ordenado(const int *arr, int n);

#endif