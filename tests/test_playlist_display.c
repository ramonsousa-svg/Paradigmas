#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "playlist.h"

int main(void)
{
    Playlist playlist;
    Musica musica = {"Bohemian Rhapsody", "Queen", "A Night at the Opera", 1975};
    FILE *saida;
    char texto[512];

    playlist_inicializar(&playlist);

    /* Uma playlist vazia informa seu estado sem tentar acessar um vetor. */
    saida = tmpfile();
    assert(saida != NULL);
    assert(playlist_listar(&playlist, saida));
    rewind(saida);
    assert(fgets(texto, sizeof texto, saida) != NULL);
    assert(strcmp(texto, "Playlist vazia.\n") == 0);
    fclose(saida);

    playlist_adicionar(&playlist, &musica);

    /* A exibição atual deve conter os quatro campos e a posição. */
    saida = tmpfile();
    assert(saida != NULL);
    assert(playlist_exibir_atual(&playlist, saida));
    rewind(saida);
    assert(fgets(texto, sizeof texto, saida) != NULL);
    assert(strcmp(texto, "Titulo: Bohemian Rhapsody\n") == 0);
    fclose(saida);

    /* A listagem deve marcar a música atual com '>'. */
    saida = tmpfile();
    assert(saida != NULL);
    assert(playlist_listar(&playlist, saida));
    rewind(saida);
    assert(fgets(texto, sizeof texto, saida) != NULL);
    assert(strcmp(texto, "--- Playlist ---\n") == 0);
    assert(fgets(texto, sizeof texto, saida) != NULL);
    assert(strncmp(texto, "> 1. Bohemian Rhapsody", 21) == 0);
    fclose(saida);

    playlist_liberar(&playlist);
    return 0;
}
