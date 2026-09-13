#include "io.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*
 * io.c
 *
 * Leitura segura de terminal compartilhada por todo o programa.
 * Nenhuma funcao aqui conhece playlist ou menu; apenas texto e numeros.
 */

/*
 * Verifica se o usuario digitou mais caracteres do que cabiam no buffer.
 *
 * O fgets guarda o '\n' quando consegue ler a linha inteira. Se nao existe
 * '\n' e ainda nao chegamos no fim da entrada, significa que sobrou texto
 * pendente no teclado/terminal.
 */
int linha_estourou_buffer(const char *texto)
{
    return strchr(texto, '\n') == NULL && !feof(stdin);
}

/*
 * Descarta o restante da linha quando uma entrada passa do limite.
 *
 * Exemplo: se o campo aceita 99 caracteres e o usuario digita 200, os
 * caracteres extras precisam ser removidos para nao atrapalhar a proxima
 * leitura do menu.
 */
void limpar_resto_da_linha(void)
{
    int caractere;

    do {
        caractere = getchar();
    } while (caractere != '\n' && caractere != EOF);
}

/*
 * Remove o '\n' deixado pelo fgets no final da string.
 *
 * Isso evita que o titulo, artista ou album sejam salvos com uma quebra de
 * linha no final.
 */
void remover_quebra_de_linha(char *texto)
{
    texto[strcspn(texto, "\n")] = '\0';
}

/*
 * Retorna verdadeiro quando o texto esta vazio ou contem apenas espacos.
 *
 * Assim, entradas como "" ou "    " sao recusadas nos campos obrigatorios.
 */
int texto_vazio(const char *texto)
{
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }
        texto++;
    }

    return 1;
}

/*
 * Le uma string com limite de tamanho e rejeita entrada vazia ou longa demais.
 *
 * A leitura de texto usa fgets porque ela recebe o tamanho do destino. Isso
 * evita o problema classico de scanf("%s", ...) sem limite, que pode causar
 * overflow quando o usuario digita uma palavra maior que o vetor.
 *
 * A leitura acontece num buffer maior que o campo. Assim, um texto de
 * tamanho - 1 bytes e aceito, porque o '\n' ainda cabe na leitura; passar
 * disso e recusado antes de copiar para o destino.
 */
int ler_texto(const char *rotulo, char *destino, size_t tamanho)
{
    /*
     * ponytail: buffer fixo de 256 bytes, suficiente para os campos atuais
     * (char[100]) com folga para detectar excesso. Se um campo futuro exigir
     * mais que 255 bytes, trocar por alocacao dinamica com malloc.
     */
    char linha[256];
    int entrada_valida = 0;

    while (!entrada_valida) {
        size_t comprimento;

        printf("%s", rotulo);

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            /* NULL indica fim de arquivo, erro de leitura ou entrada interrompida. */
            printf("\nFim inesperado da entrada. Encerrando leitura.\n");
            return 0;
        }

        if (linha_estourou_buffer(linha)) {
            limpar_resto_da_linha();
            printf("Entrada muito longa. Use no maximo %zu caracteres.\n", tamanho - 1);
            continue;
        }

        remover_quebra_de_linha(linha);

        if (texto_vazio(linha)) {
            printf("Entrada vazia. Informe um texto valido.\n");
            continue;
        }

        comprimento = strlen(linha);
        if (comprimento > tamanho - 1) {
            /* Nao caberia no campo com o terminador; pedir um texto menor. */
            printf("Entrada muito longa. Use no maximo %zu caracteres.\n", tamanho - 1);
            continue;
        }

        memcpy(destino, linha, comprimento + 1);
        entrada_valida = 1;
    }

    return 1;
}

/*
 * Le um numero inteiro dentro de um intervalo.
 *
 * A conversao usa strtol em vez de scanf. Com strtol conseguimos verificar:
 * - se o usuario realmente digitou um numero;
 * - se sobrou lixo depois do numero;
 * - se o numero esta dentro do intervalo permitido;
 * - se houve estouro de valor numerico.
 */
int ler_numero_intervalo(const char *rotulo, int minimo, int maximo, int *valor)
{
    char linha[64];
    char *fim = NULL;
    long numero;

    while (1) {
        printf("%s", rotulo);

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            printf("\nFim inesperado da entrada. Encerrando leitura.\n");
            return 0;
        }

        if (linha_estourou_buffer(linha)) {
            limpar_resto_da_linha();
            printf("Entrada muito longa. Informe um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        remover_quebra_de_linha(linha);

        if (texto_vazio(linha)) {
            printf("Entrada vazia. Informe um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        errno = 0;
        numero = strtol(linha, &fim, 10);

        /*
         * fim == linha: nenhum numero foi lido.
         * !texto_vazio(fim): existe texto depois do numero.
         */
        if (fim == linha || !texto_vazio(fim)) {
            printf("Entrada invalida. Informe apenas numeros.\n");
            continue;
        }

        /* ERANGE indica que o numero digitado estourou o limite do tipo long. */
        if (errno == ERANGE || numero < minimo || numero > maximo) {
            printf("Valor fora do intervalo. Informe um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        if (numero < INT_MIN || numero > INT_MAX) {
            printf("Valor fora do intervalo permitido para inteiro.\n");
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}
