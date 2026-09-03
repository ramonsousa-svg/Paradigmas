#include "playlist.h"

#include <stdint.h>
#include <stdlib.h>

/*
 * Começa a playlist sem alocar memória.
 * O índice -1 representa que não existe música atual.
 */
void playlist_inicializar(Playlist *playlist)
{
    if (playlist == NULL) {
        return;
    }

    playlist->musicas = NULL;
    playlist->total_musicas = 0;
    playlist->indice_atual = -1;
}

/*
 * Encerra a playlist com segurança.
 * O free devolve o vetor para o sistema, e os campos são resetados para
 * evitar que a estrutura continue apontando para memória já liberada.
 */
void playlist_liberar(Playlist *playlist)
{
    if (playlist == NULL) {
        return;
    }

    free(playlist->musicas);
    playlist->musicas = NULL;
    playlist->total_musicas = 0;
    playlist->indice_atual = -1;
}

/*
 * Retorna true quando não há músicas cadastradas.
 * Uma playlist nula também é tratada como vazia para evitar acesso inválido.
 */
bool playlist_vazia(const Playlist *playlist)
{
    return playlist == NULL || playlist->total_musicas == 0;
}

/*
 * Adiciona uma música criando um vetor com uma posição a mais.
 * Primeiro montamos o novo vetor; só depois liberamos o antigo. Assim, se a
 * alocação falhar, a playlist original continua intacta.
 */
bool playlist_adicionar(Playlist *playlist, const Musica *musica)
{
    Musica *novo_vetor;
    size_t novo_total;
    size_t i;

    if (playlist == NULL || musica == NULL) {
        return false;
    }

    /* Evita que total + 1 estoure o maior valor de size_t. */
    if (playlist->total_musicas == SIZE_MAX) {
        return false;
    }

    novo_total = playlist->total_musicas + 1;
    novo_vetor = malloc(novo_total * sizeof *novo_vetor);
    if (novo_vetor == NULL) {
        return false;
    }

    /* Copia cada registro existente para o novo vetor. */
    for (i = 0; i < playlist->total_musicas; i++) {
        novo_vetor[i] = playlist->musicas[i];
    }

    /* A nova música entra na primeira posição livre. */
    novo_vetor[playlist->total_musicas] = *musica;

    /* O vetor antigo não é mais necessário depois da cópia. */
    free(playlist->musicas);
    playlist->musicas = novo_vetor;
    playlist->total_musicas = novo_total;

    /* A primeira música adicionada passa a ser a música atual. */
    if (playlist->indice_atual == -1) {
        playlist->indice_atual = 0;
    }

    return true;
}

/*
 * Tenta avançar para a próxima música.
 * Retornar false no limite permite que o menu mostre uma mensagem sem
 * alterar o índice atual.
 */
bool playlist_proxima(Playlist *playlist)
{
    if (playlist_vazia(playlist) || playlist->indice_atual < 0) {
        return false;
    }

    if ((size_t) playlist->indice_atual + 1 >= playlist->total_musicas) {
        return false;
    }

    playlist->indice_atual++;
    return true;
}

/*
 * Tenta voltar para a música anterior.
 * O índice nunca fica menor que zero.
 */
bool playlist_anterior(Playlist *playlist)
{
    if (playlist_vazia(playlist) || playlist->indice_atual <= 0) {
        return false;
    }

    playlist->indice_atual--;
    return true;
}

/*
 * Retorna um ponteiro para a música atual sem copiar o registro.
 * O ponteiro é somente para consulta: o chamador não deve liberá-lo.
 */
const Musica *playlist_atual(const Playlist *playlist)
{
    if (playlist_vazia(playlist) || playlist->indice_atual < 0) {
        return NULL;
    }

    if ((size_t) playlist->indice_atual >= playlist->total_musicas) {
        return NULL;
    }

    return &playlist->musicas[playlist->indice_atual];
}

/*
 * Mostra os campos da música atual.
 * %s é usado para imprimir texto; %d imprime um número inteiro decimal.
 * FILE *saida permite enviar o texto para a tela ou para um arquivo de teste.
 */
bool playlist_exibir_atual(const Playlist *playlist, FILE *saida)
{
    const Musica *musica = playlist_atual(playlist);

    if (musica == NULL || saida == NULL) {
        return false;
    }

    fprintf(saida, "Titulo: %s\n", musica->titulo);
    fprintf(saida, "Artista: %s\n", musica->artista);
    fprintf(saida, "Album: %s\n", musica->album);
    fprintf(saida, "Ano: %d\n", musica->ano);
    fprintf(saida, "Posicao: %d de %zu\n",
            playlist->indice_atual + 1, playlist->total_musicas);

    return true;
}

/*
 * Lista o vetor inteiro em sua ordem atual.
 * O marcador '>' aparece somente antes da música que está sendo tocada.
 * %zu imprime um valor do tipo size_t, usado para quantidades e posições.
 */
bool playlist_listar(const Playlist *playlist, FILE *saida)
{
    size_t i;

    if (playlist == NULL || saida == NULL) {
        return false;
    }

    if (playlist_vazia(playlist)) {
        fprintf(saida, "Playlist vazia.\n");
        return true;
    }

    fprintf(saida, "--- Playlist ---\n");
    for (i = 0; i < playlist->total_musicas; i++) {
        char marcador = i == (size_t) playlist->indice_atual ? '>' : ' ';

        /* %zu imprime a posição, e %s imprime cada campo textual. */
        fprintf(saida, "%c %zu. %s | %s | %s | %d\n",
                marcador,
                i + 1,
                playlist->musicas[i].titulo,
                playlist->musicas[i].artista,
                playlist->musicas[i].album,
                playlist->musicas[i].ano);
    }

    return true;
}
