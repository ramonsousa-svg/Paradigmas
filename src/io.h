#ifndef IO_H
#define IO_H

#include <stddef.h>
#include <stdio.h>

/* Funcoes utilitarias de leitura segura compartilhadas pelo programa. */

/* Verdadeiro quando o fgets nao leu a linha inteira (sobrou texto). */
int linha_estourou_buffer(const char *texto);

/* Descarta o restante da linha atual da entrada padrao. */
void limpar_resto_da_linha(void);

/* Remove o '\n' final deixado pelo fgets, se existir. */
void remover_quebra_de_linha(char *texto);

/* Verdadeiro quando o texto esta vazio ou contem apenas espacos. */
int texto_vazio(const char *texto);

/* Le texto com limite; rejeita vazio e excesso. Retorna 1 em sucesso. */
int ler_texto(const char *rotulo, char *destino, size_t tamanho);

/* Le inteiro em [minimo, maximo] via strtol; rejeita o resto. Retorna 1 em sucesso. */
int ler_numero_intervalo(const char *rotulo, int minimo, int maximo, int *valor);

#endif
