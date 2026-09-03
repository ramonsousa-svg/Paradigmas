#include <assert.h>
#include <string.h>

#include "playlist.h"

int main(void)
{
    Playlist playlist;
    Musica primeira = {"Bohemian Rhapsody", "Queen", "A Night at the Opera", 1975};
    Musica segunda = {"Hotel California", "Eagles", "Hotel California", 1976};
    const Musica *atual;

    playlist_inicializar(&playlist);

    /* Em uma playlist vazia, nenhuma direção possui uma música válida. */
    assert(!playlist_proxima(&playlist));
    assert(!playlist_anterior(&playlist));
    assert(playlist_atual(&playlist) == NULL);

    playlist_adicionar(&playlist, &primeira);
    playlist_adicionar(&playlist, &segunda);

    atual = playlist_atual(&playlist);
    assert(atual != NULL);
    assert(strcmp(atual->titulo, "Bohemian Rhapsody") == 0);

    /* Avançar muda para a segunda música. */
    assert(playlist_proxima(&playlist));
    atual = playlist_atual(&playlist);
    assert(atual != NULL);
    assert(strcmp(atual->titulo, "Hotel California") == 0);

    /* No fim, o índice permanece na última posição. */
    assert(!playlist_proxima(&playlist));
    assert(playlist.indice_atual == 1);

    /* Voltar muda para a primeira música. */
    assert(playlist_anterior(&playlist));
    assert(playlist.indice_atual == 0);

    /* No início, o índice permanece na primeira posição. */
    assert(!playlist_anterior(&playlist));
    assert(playlist.indice_atual == 0);

    playlist_liberar(&playlist);
    return 0;
}
