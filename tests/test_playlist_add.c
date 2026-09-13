#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "playlist.h"

int main(void)
{
    Playlist playlist;
    Musica primeira = {"Bohemian Rhapsody", "Queen", "A Night at the Opera", 1975};
    Musica segunda = {"Hotel California", "Eagles", "Hotel California", 1976};

    playlist_inicializar(&playlist);

    /* A playlist começa vazia e sem uma música atual. */
    assert(playlist_vazia(&playlist));
    assert(playlist.indice_atual == -1);

    /* A primeira inserção cria o vetor e escolhe o índice 0. */
    assert(playlist_adicionar(&playlist, &primeira));
    assert(playlist.total_musicas == 1);
    assert(playlist.indice_atual == 0);
    assert(strcmp(playlist.musicas[0].titulo, "Bohemian Rhapsody") == 0);

    /* A segunda inserção preserva a primeira e adiciona a nova música. */
    assert(playlist_adicionar(&playlist, &segunda));
    assert(playlist.total_musicas == 2);
    assert(strcmp(playlist.musicas[0].artista, "Queen") == 0);
    assert(strcmp(playlist.musicas[1].artista, "Eagles") == 0);

    playlist_liberar(&playlist);
    assert(playlist_vazia(&playlist));

    /* Stress: 1000 insercoes crescem o vetor copiando a cada vez. */
    for (int i = 0; i < 1000; i++) {
        Musica m;
        snprintf(m.titulo, sizeof m.titulo, "Faixa %d", i);
        snprintf(m.artista, sizeof m.artista, "Artista %d", i);
        snprintf(m.album, sizeof m.album, "Album %d", i);
        m.ano = 1900 + (i % 201);
        assert(playlist_adicionar(&playlist, &m));
        assert(playlist.total_musicas == (size_t)(i + 1));
    }
    assert(playlist.total_musicas == 1000);
    assert(playlist.indice_atual == 0);
    assert(strcmp(playlist.musicas[999].titulo, "Faixa 999") == 0);

    playlist_liberar(&playlist);

    return 0;
}
