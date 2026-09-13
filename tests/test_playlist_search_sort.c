#include <assert.h>
#include <string.h>

#include "playlist.h"

int main(void)
{
    Playlist playlist;
    Musica z = {"Ziggy Stardust", "David Bowie", "Aladdin Sane", 1973};
    Musica a = {"Across the Universe", "The Beatles", "Let It Be", 1970};
    Musica b = {"Blackstar", "David Bowie", "Blackstar", 2016};
    size_t posicoes[8];
    size_t n;

    playlist_inicializar(&playlist);
    playlist_adicionar(&playlist, &z);
    playlist_adicionar(&playlist, &a);
    playlist_adicionar(&playlist, &b);

    /* Busca exata por titulo. */
    assert(playlist_buscar_titulo(&playlist, "Ziggy Stardust") == 0);
    assert(playlist_buscar_titulo(&playlist, "Titulo inexistente") == -1);

    /* Busca parcial: "david" encontra David Bowie (case-insensitive). */
    n = playlist_buscar_parcial(&playlist, "david", posicoes, sizeof posicoes / sizeof posicoes[0]);
    assert(n == 2);
    assert(posicoes[0] == 0);
    assert(posicoes[1] == 2);

    /* Busca parcial: "across" encontra apenas The Beatles. */
    n = playlist_buscar_parcial(&playlist, "across", posicoes, sizeof posicoes / sizeof posicoes[0]);
    assert(n == 1);
    assert(posicoes[0] == 1);

    /* Busca parcial: termo vazio nao retorna resultados. */
    n = playlist_buscar_parcial(&playlist, "   ", posicoes, sizeof posicoes / sizeof posicoes[0]);
    assert(n == 0);

    /* Busca parcial: sem correspondencia. */
    n = playlist_buscar_parcial(&playlist, "xyz123", posicoes, sizeof posicoes / sizeof posicoes[0]);
    assert(n == 0);

    /* Busca parcial: truncamento no limite de posicoes. */
    n = playlist_buscar_parcial(&playlist, "a", posicoes, 1);
    assert(n == 1);
    assert(posicoes[0] == 0);
    n = playlist_buscar_parcial(&playlist, "david", posicoes, 1);
    assert(n == 1);
    n = playlist_buscar_parcial(&playlist, "a", posicoes, 0);
    assert(n == 0);

    /* Definir musica atual por posicao valida: Ziggy (indice 0 antes da ordenacao). */
    assert(playlist_definir_atual(&playlist, 0));
    assert(playlist.indice_atual == 0);

    /* Definir musica atual por posicao invalida nao altera indice. */
    assert(!playlist_definir_atual(&playlist, 999));
    assert(playlist.indice_atual == 0);

    /* Definir musica atual em playlist vazia retorna false. */
    Playlist vazia;
    playlist_inicializar(&vazia);
    assert(!playlist_definir_atual(&vazia, 0));

    /* Ordena e preserva musica atual: Ziggy vai para indice 2. */
    playlist_ordenar_por_titulo(&playlist);
    assert(strcmp(playlist.musicas[0].titulo, "Across the Universe") == 0);
    assert(strcmp(playlist.musicas[1].titulo, "Blackstar") == 0);
    assert(strcmp(playlist.musicas[2].titulo, "Ziggy Stardust") == 0);
    assert(playlist.indice_atual == 2);
    assert(strcmp(playlist_atual(&playlist)->titulo, "Ziggy Stardust") == 0);

    playlist_liberar(&playlist);
    playlist_liberar(&vazia);
    return 0;
}
