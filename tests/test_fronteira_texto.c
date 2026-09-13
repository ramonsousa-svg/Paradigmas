#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Teste de regressao real: executa o player recompilado com
 * titulos de 98, 99 e 100 bytes e valida a mensagem de excesso
 * e o cadastro bem-sucedido.
 *
 * Uso: test_fronteira [caminho-do-player]
 * O padrao "player.exe" cobre o fluxo Windows do build.ps1.
 */
static void make_input(const char *title, const char *path)
{
    FILE *f = fopen(path, "w");
    assert(f);
    fprintf(f, "1\n%s\nQueen\nAlbum\n1975\n7\n", title);
    fclose(f);
}

static int run_player(const char *player, const char *input_path)
{
    char cmd[512];
    snprintf(cmd, sizeof cmd, "\"%s\" < %s > %s.out 2>&1", player, input_path, input_path);
    return system(cmd);
}

static int check_output(const char *out_path, const char *must, const char *must_not)
{
    FILE *f = fopen(out_path, "r");
    char buf[8192];
    size_t n;
    int ok = 1;

    assert(f);
    n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = '\0';
    fclose(f);

    if (must && !strstr(buf, must)) ok = 0;
    if (must_not && strstr(buf, must_not)) ok = 0;
    remove(out_path);
    return ok;
}

static void fill(char *buf, size_t n, char ch)
{
    memset(buf, ch, n);
    buf[n] = '\0';
}

static void run_case(const char *player, const char *label, const char *title,
                     const char *must, const char *must_not, int *ok)
{
    char tmp[64];
    char out[80];

    snprintf(tmp, sizeof tmp, "tmp_%s.in", label);
    make_input(title, tmp);
    if (run_player(player, tmp) != 0) *ok = 0;
    snprintf(out, sizeof out, "%s.out", tmp);
    if (!check_output(out, must, must_not)) *ok = 0;
}

int main(int argc, char **argv)
{
    const char *player = argc > 1 ? argv[1] : "player.exe";
    char title98[99];
    char title99[100];
    char title100[101];
    int ok = 1;

    fill(title98, 98, 'a');
    fill(title99, 99, 'b');
    fill(title100, 100, 'c');

    assert(strlen(title98) == 98);
    assert(strlen(title99) == 99);
    assert(strlen(title100) == 100);

    /* Casos validos: musica entra e o programa encerra pela opcao 7. */
    run_case(player, "98", title98, "Música adicionada com sucesso!",
             "Entrada muito longa", &ok);
    run_case(player, "99", title99, "Música adicionada com sucesso!",
             "Entrada muito longa", &ok);
    run_case(player, "100", title100, "Entrada muito longa", NULL, &ok);

    printf("fronteira: %s\n", ok ? "PASSOU" : "FALHOU");
    return ok ? 0 : 1;
}
