#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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

/* Adiciona uma música, aumentando o vetor sem perder os registros atuais. */
bool playlist_adicionar(Playlist *playlist, const Musica *musica);

/* Avança uma posição; retorna false quando já estamos na última música. */
bool playlist_proxima(Playlist *playlist);

/* Volta uma posição; retorna false quando já estamos na primeira música. */
bool playlist_anterior(Playlist *playlist);

/* Devolve a música atual ou NULL quando não há uma posição válida. */
const Musica *playlist_atual(const Playlist *playlist);

/* Exibe os dados da música atual; retorna false quando não há música. */
bool playlist_exibir_atual(const Playlist *playlist, FILE *saida);

/* Lista todas as músicas e marca a música atual com o símbolo '>'. */
bool playlist_listar(const Playlist *playlist, FILE *saida);

#endif
