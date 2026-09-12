#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Teste de regressao real: executa o player recompilado com
 * titulos de 98, 99 e 100 bytes e valida a mensagem de excesso
 * e o cadastro bem-sucedido.
 */
static void make_input(const char *title, const char *path)
{
    FILE *f = fopen(path, "w");
    assert(f);
    fprintf(f, "1\n%s\nQueen\nAlbum\n1975\n6\n", title);
    fclose(f);
}

static int run_player(const char *input_path)
{
    char cmd[512];
    snprintf(cmd, sizeof cmd, "player.exe < %s > %s.out 2>&1", input_path, input_path);
    return system(cmd);
}

static int check_output(const char *out_path, const char *must, const char *must_not)
{
    FILE *f = fopen(out_path, "r");
    char buf[4096];
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

static void run_case(const char *label, const char *title, const char *must, const char *must_not, int *ok)
{
    char tmp[64];
    char out[64];

    snprintf(tmp, sizeof tmp, "tmp_%s.in", label);
    make_input(title, tmp);
    if (run_player(tmp) != 0) *ok = 0;
    memset(out, 0, sizeof out);
    memcpy(out, tmp, strlen(tmp));
    memcpy(out + strlen(tmp), ".out", 4);
    if (!check_output(out, must, must_not)) *ok = 0;
}

int main(void)
{
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

    run_case("98", title98, "Música adicionada com sucesso!", "Entrada muito longa", &ok);
    run_case("99", title99, "Música adicionada com sucesso!", "Entrada muito longa", &ok);
    run_case("100", title100, "Entrada muito longa", NULL, &ok);

    printf("fronteira: %s\n", ok ? "PASSOU" : "FALHOU");
    return ok ? 0 : 1;
}
