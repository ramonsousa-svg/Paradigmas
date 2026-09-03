#include <assert.h>
#include <string.h>

#include "playlist.h"

int main(void)
{
    Playlist playlist;
    Musica z = {"Ziggy Stardust", "David Bowie", "Aladdin Sane", 1973};
    Musica a = {"Across the Universe", "The Beatles", "Let It Be", 1970};

    playlist_inicializar(&playlist);
    playlist_adicionar(&playlist, &z);
    playlist_adicionar(&playlist, &a);

    /* A busca devolve o índice do título exato ou -1 quando não existe. */
    assert(playlist_buscar_titulo(&playlist, "Ziggy Stardust") == 0);
    assert(playlist_buscar_titulo(&playlist, "Titulo inexistente") == -1);

    /* A música atual era Ziggy e deve continuar sendo a atual após ordenar. */
    playlist_ordenar_por_titulo(&playlist);
    assert(strcmp(playlist.musicas[0].titulo, "Across the Universe") == 0);
    assert(strcmp(playlist.musicas[1].titulo, "Ziggy Stardust") == 0);
    assert(playlist.indice_atual == 1);
    assert(strcmp(playlist_atual(&playlist)->titulo, "Ziggy Stardust") == 0);

    playlist_liberar(&playlist);
    return 0;
}
