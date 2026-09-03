#include "playlist.h"

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
