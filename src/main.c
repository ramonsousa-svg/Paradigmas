#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "playlist.h"

/*
 * main.c
 *
 * Responsabilidade da Pessoa 3:
 * - montar o fluxo do menu;
 * - exibir mensagens para o usuario;
 * - validar entradas antes de chamar as funcoes da playlist.
 *
 * A leitura segura de texto e numeros fica em io.c/io.h.
 * O vetor dinamico e a logica interna da playlist ficam em playlist.c/playlist.h.
 */

/*
 * O enum troca "numeros soltos" por nomes claros.
 * Assim, quando aparece OPCAO_ADICIONAR no codigo, fica mais facil entender
 * que o valor 1 representa a opcao de adicionar musica.
 */
enum {
    OPCAO_ADICIONAR = 1,
    OPCAO_PROXIMA = 2,
    OPCAO_ANTERIOR = 3,
    OPCAO_ATUAL = 4,
    OPCAO_LISTAR = 5,
    OPCAO_BUSCAR = 6,
    OPCAO_SAIR = 7,
    BUSCA_MAX_RESULTADOS = 64,
    ANO_MINIMO = 1900,
    ANO_MAXIMO = 2100
};

/*
 * Mostra o menu principal com as sete opcoes do programa.
 * Esta funcao apenas imprime o menu; ela nao le nem executa nenhuma opcao.
 */
static void exibir_menu(void)
{
    printf("\n===== Player de Músicas =====\n");
    printf("1. Adicionar música\n");
    printf("2. Próxima música\n");
    printf("3. Música anterior\n");
    printf("4. Exibir música atual\n");
    printf("5. Listar playlist\n");
    printf("6. Buscar música\n");
    printf("7. Sair\n");
}

/*
 * Le e valida a opcao do menu.
 *
 * Usa strtol com as mesmas regras de ler_numero_intervalo, mas com faixa
 * 1..OPCAO_SAIR e mensagens proprias de "Opção invalida".
 */
static int ler_opcao_menu(int *opcao)
{
    char linha[64];
    char *fim = NULL;
    long numero;

    while (1) {
        printf("Escolha uma opção: ");

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            printf("\nFim inesperado da entrada. Encerrando leitura.\n");
            return 0;
        }

        if (linha_estourou_buffer(linha)) {
            limpar_resto_da_linha();
            printf("Entrada muito longa. Informe uma opção entre %d e %d.\n",
                   OPCAO_ADICIONAR,
                   OPCAO_SAIR);
            continue;
        }

        remover_quebra_de_linha(linha);

        if (texto_vazio(linha)) {
            printf("Entrada vazia. Informe uma opção entre %d e %d.\n",
                   OPCAO_ADICIONAR,
                   OPCAO_SAIR);
            continue;
        }

        errno = 0;
        numero = strtol(linha, &fim, 10);

        if (fim == linha || !texto_vazio(fim)) {
            printf("Entrada inválida. Informe apenas números.\n");
            continue;
        }

        if (errno == ERANGE || numero < OPCAO_ADICIONAR || numero > OPCAO_SAIR) {
            printf("Opção inválida. Escolha uma opção entre %d e %d.\n",
                   OPCAO_ADICIONAR,
                   OPCAO_SAIR);
            continue;
        }

        *opcao = (int)numero;
        return 1;
    }
}

/*
 * Pergunta os dados de uma musica usando as funcoes de leitura segura.
 *
 * Se qualquer campo falhar, a funcao retorna 0 e a musica nao e enviada para
 * a playlist. Isso evita cadastrar registros incompletos.
 */
static int ler_dados_musica(Musica *musica)
{
    printf("\n--- Adicionar Música ---\n");

    if (!ler_texto("Titulo: ", musica->titulo, sizeof musica->titulo)) {
        return 0;
    }

    if (!ler_texto("Artista: ", musica->artista, sizeof musica->artista)) {
        return 0;
    }

    if (!ler_texto("Album: ", musica->album, sizeof musica->album)) {
        return 0;
    }

    if (!ler_numero_intervalo("Ano: ", ANO_MINIMO, ANO_MAXIMO, &musica->ano)) {
        return 0;
    }

    return 1;
}

/*
 * Exibe os quatro campos basicos de uma musica.
 *
 * A funcao recebe const Musica * porque ela apenas mostra os dados e nao deve
 * alterar a musica.
 */
static void exibir_musica(const Musica *musica)
{
    printf("Título: %s\n", musica->titulo);
    printf("Artista: %s\n", musica->artista);
    printf("Álbum: %s\n", musica->album);
    printf("Ano: %d\n", musica->ano);
}

/*
 * Coordena a opcao de adicionar musica.
 *
 * O menu coleta e valida os dados. Depois, chama playlist_adicionar, que e a
 * funcao responsavel por alocar o vetor maior, copiar registros e liberar o
 * vetor antigo.
 */
static void adicionar_pelo_menu(Playlist *playlist)
{
    Musica musica;
    size_t total_anterior = playlist->total_musicas;

    if (!ler_dados_musica(&musica)) {
        return;
    }

    /*
     * Mensagens exigidas pelos testes.
     * A alocacao real acontece dentro de playlist_adicionar.
     */
    printf("\nAlocando novo vetor (capacidade: %zu)...\n", total_anterior + 1);

    if (total_anterior > 0) {
        printf("Copiando %zu música(s) existente(s)...\n", total_anterior);
        printf("Liberando vetor anterior...\n");
    }

    if (!playlist_adicionar(playlist, &musica)) {
        printf("\nErro ao adicionar música. Verifique a memória disponível.\n");
        return;
    }

    /* Depois da insercao, mostramos um resumo para confirmar a operacao. */
    printf("\nMúsica adicionada com sucesso!\n");
    printf("Total de músicas: %zu\n", playlist->total_musicas);

    /* Caso especial: a primeira musica cadastrada vira a musica atual. */
    if (total_anterior == 0 && playlist->indice_atual == 0) {
        printf("Música atual definida automaticamente (índice 0).\n");
    }
}

/*
 * Coordena a opcao de avancar para a proxima musica.
 *
 * A funcao playlist_proxima retorna false quando nao e possivel avancar. O
 * menu usa esse retorno para mostrar a mensagem correta ao usuario.
 */
static void avancar_pelo_menu(Playlist *playlist)
{
    const Musica *musica;

    if (playlist_vazia(playlist)) {
        printf("\nNenhuma música na playlist.\n");
        return;
    }

    if (!playlist_proxima(playlist)) {
        printf("\nVocê já está na última música da playlist.\n");
        printf("Música atual permanece:\n");
    } else {
        printf("\n--- Próxima Música ---\n");
    }

    musica = playlist_atual(playlist);
    if (musica != NULL) {
        exibir_musica(musica);
    }
}

/*
 * Coordena a opcao de voltar para a musica anterior.
 *
 * Antes de tentar voltar, o menu verifica se a playlist esta vazia. Depois,
 * deixa playlist_anterior cuidar do limite da primeira musica.
 */
static void retroceder_pelo_menu(Playlist *playlist)
{
    const Musica *musica;

    if (playlist_vazia(playlist)) {
        printf("\nNenhuma música na playlist.\n");
        return;
    }

    if (!playlist_anterior(playlist)) {
        printf("\nVocê já está na primeira música da playlist.\n");
        printf("Música atual permanece:\n");
    } else {
        printf("\n--- Música Anterior ---\n");
    }

    musica = playlist_atual(playlist);
    if (musica != NULL) {
        exibir_musica(musica);
    }
}

/*
 * Exibe a musica atual com a posicao dentro da playlist.
 *
 * A posicao mostrada para o usuario comeca em 1, mas o indice interno do
 * vetor comeca em 0. Por isso imprimimos indice_atual + 1.
 */
static void exibir_atual_pelo_menu(const Playlist *playlist)
{
    const Musica *musica;

    if (playlist_vazia(playlist)) {
        printf("\nNenhuma música na playlist.\n");
        return;
    }

    musica = playlist_atual(playlist);
    if (musica == NULL) {
        printf("\nNenhuma música na playlist.\n");
        return;
    }

    printf("\n--- Música Atual ---\n");
    exibir_musica(musica);
    printf("Posição: %d de %zu\n", playlist->indice_atual + 1, playlist->total_musicas);
}

/*
 * Lista todas as musicas e marca com '>' qual e a musica atual.
 *
 * O for percorre o vetor de 0 ate total_musicas - 1. Na tela, somamos 1 para
 * exibir as posicoes em formato mais natural para o usuario.
 */
static void listar_pelo_menu(const Playlist *playlist)
{
    size_t i;

    printf("\n--- Playlist ---\n");

    if (playlist_vazia(playlist)) {
        printf("Playlist vazia.\n");
        return;
    }

    for (i = 0; i < playlist->total_musicas; i++) {
        const Musica *musica = &playlist->musicas[i];
        char marcador = i == (size_t)playlist->indice_atual ? '>' : ' ';

        /*
         * Os tamanhos negativos em %-20s, %-13s e %-22s alinham as colunas
         * pela esquerda, deixando a listagem mais legivel no terminal.
         */
        printf("%c %zu. %-20s | %-13s | %-22s | %d\n",
               marcador,
               i + 1,
               musica->titulo,
               musica->artista,
               musica->album,
               musica->ano);
    }

    printf("\n(> indica a música atual)\n");
}

/*
 * Coordena a opcao de buscar musicas na playlist.
 *
 * A busca ignora maiusculas/minusculas e aceita um trecho do titulo ou do
 * artista. Quando existe apenas um resultado, ele vira a musica atual; quando
 * existem varios, o usuario escolhe a posicao. Nenhum resultado e informado
 * sem alterar a playlist.
 */
static void buscar_pelo_menu(Playlist *playlist)
{
    char termo[128];
    size_t posicoes[BUSCA_MAX_RESULTADOS];
    size_t encontradas;
    size_t i;
    size_t capacidade = sizeof posicoes / sizeof posicoes[0];
    int opcao_resultado;

    if (playlist_vazia(playlist)) {
        printf("\nNenhuma música na playlist.\n");
        return;
    }

    printf("\n--- Buscar Música ---\n");

    /* ler_texto já rejeita termo vazio. */
    if (!ler_texto("Termo de busca: ", termo, sizeof termo)) {
        return;
    }

    encontradas = playlist_buscar_parcial(playlist, termo, posicoes, capacidade);

    if (encontradas == 0) {
        printf("\nNenhuma música encontrada para \"%s\".\n", termo);
        return;
    }

    printf("\n%d resultado(s) para \"%s\":\n", (int)encontradas, termo);
    if (encontradas == capacidade) {
        printf("(pode haver mais resultados; refinar a busca para ver todos)\n");
    }
    for (i = 0; i < encontradas; i++) {
        const Musica *musica = &playlist->musicas[posicoes[i]];
        printf("  [%zu] %s | %s\n", i + 1, musica->titulo, musica->artista);
    }

    if (encontradas == 1) {
        playlist_definir_atual(playlist, posicoes[0]);
        printf("\nMúsica atual definida automaticamente.\n");
        return;
    }

    if (!ler_numero_intervalo("\nEscolha o resultado (0 cancela): ",
                              0, (int)encontradas, &opcao_resultado)) {
        return;
    }

    if (opcao_resultado == 0) {
        printf("Busca cancelada. A música atual não mudou.\n");
        return;
    }

    if (!playlist_definir_atual(playlist, posicoes[(size_t)opcao_resultado - 1])) {
        printf("Não foi possível definir a música atual.\n");
    }
}

/*
 * Funcao principal.
 *
 * Aqui fica o fluxo geral do programa:
 * 1. inicializar a playlist;
 * 2. repetir o menu;
 * 3. executar a opcao escolhida;
 * 4. liberar a memoria antes de encerrar.
 */
int main(void)
{
    Playlist playlist;
    int opcao = 0;
    int continuar = 1;

    playlist_inicializar(&playlist);
    setlocale(LC_ALL, "pt_BR.UTF-8");
    printf("Playlist inicializada (ponteiro NULL, total = 0, índice atual = -1).\n");

    /*
     * O programa continua repetindo o menu ate o usuario escolher sair.
     * Se a leitura da opcao falhar por fim inesperado da entrada, o loop tambem
     * termina de forma controlada.
     */
    while (continuar) {
        exibir_menu();

        if (!ler_opcao_menu(&opcao)) {
            break;
        }

        /* Cada case chama uma funcao pequena, mantendo o main organizado. */
        switch (opcao) {
        case OPCAO_ADICIONAR:
            adicionar_pelo_menu(&playlist);
            break;
        case OPCAO_PROXIMA:
            avancar_pelo_menu(&playlist);
            break;
        case OPCAO_ANTERIOR:
            retroceder_pelo_menu(&playlist);
            break;
        case OPCAO_ATUAL:
            exibir_atual_pelo_menu(&playlist);
            break;
        case OPCAO_LISTAR:
            listar_pelo_menu(&playlist);
            break;
        case OPCAO_BUSCAR:
            buscar_pelo_menu(&playlist);
            break;
        case OPCAO_SAIR:
            continuar = 0;
            break;
        default:
            printf("\nOpção inválida.\n");
            break;
        }
    }

    /*
     * A limpeza real da memoria pertence ao modulo da playlist.
     * Mesmo assim, o menu mostra as mensagens de encerramento esperadas.
     */
    printf("\nLiberando vetor da playlist...\n");
    playlist_liberar(&playlist);
    printf("Memória liberada com sucesso.\n");
    printf("Encerrando o programa.\n");

    return 0;
}
