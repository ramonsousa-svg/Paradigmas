#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdbool.h>
#include <stddef.h>

/* Representa uma música cadastrada na playlist. */
typedef struct {
    char titulo[100];
    char artista[100];
    char album[100];
    int ano;
} Musica;

/* Guarda as músicas e a posição que está sendo considerada como atual. */
typedef struct {
    Musica *musicas;
    size_t total_musicas;
    int indice_atual;
} Playlist;

/* Prepara uma playlist vazia para ser usada pelo programa. */
void playlist_inicializar(Playlist *playlist);

/* Libera o vetor da playlist e devolve o estado para o padrão inicial. */
void playlist_liberar(Playlist *playlist);

/* Informa se a playlist ainda não possui músicas. */
bool playlist_vazia(const Playlist *playlist);

#endif
