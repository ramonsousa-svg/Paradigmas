#include <assert.h>
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

    return 0;
}
