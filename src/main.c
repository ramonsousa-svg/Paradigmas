#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playlist.h"

/*
 * main.c
 *
 * Responsabilidade da Pessoa 3:
 * - montar o fluxo do menu;
 * - exibir mensagens para o usuario;
 * - ler textos e numeros com seguranca;
 * - validar entradas antes de chamar as funcoes da playlist.
 *
 * Este arquivo nao implementa o vetor dinamico nem a logica interna da
 * playlist. Essas partes ficam em playlist.c/playlist.h.
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
    OPCAO_SAIR = 6,
    ANO_MINIMO = 1900,
    ANO_MAXIMO = 2100
};

/*
 * Verifica se o usuario digitou mais caracteres do que cabiam no buffer.
 *
 * O fgets guarda o '\n' quando consegue ler a linha inteira. Se nao existe
 * '\n' e ainda nao chegamos no fim da entrada, significa que sobrou texto
 * pendente no teclado/terminal.
 */
static int linha_estourou_buffer(const char *texto)
{
    return strchr(texto, '\n') == NULL && !feof(stdin);
}

/*
 * Descarta o restante da linha quando uma entrada passa do limite.
 *
 * Exemplo: se o campo aceita 99 caracteres e o usuario digita 200, os
 * caracteres extras precisam ser removidos para nao atrapalhar a proxima
 * leitura do menu.
 */
static void limpar_resto_da_linha(void)
{
    int caractere;

    do {
        caractere = getchar();
    } while (caractere != '\n' && caractere != EOF);
}

/*
 * Remove o '\n' deixado pelo fgets no final da string.
 *
 * Isso evita que o titulo, artista ou album sejam salvos com uma quebra de
 * linha no final.
 */
static void remover_quebra_de_linha(char *texto)
{
    texto[strcspn(texto, "\n")] = '\0';
}

/*
 * Retorna verdadeiro quando o texto esta vazio ou contem apenas espacos.
 *
 * Assim, entradas como "" ou "    " sao recusadas nos campos obrigatorios.
 */
static int texto_vazio(const char *texto)
{
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }
        texto++;
    }

    return 1;
}

/*
 * Le uma string com limite de tamanho e rejeita entrada vazia ou longa demais.
 *
 * A leitura de texto usa fgets porque ela recebe o tamanho do destino. Isso
 * evita o problema classico de scanf("%s", ...) sem limite, que pode causar
 * overflow quando o usuario digita uma palavra maior que o vetor.
 */
static int ler_texto(const char *rotulo, char *destino, size_t tamanho)
{
    int entrada_valida = 0;

    while (!entrada_valida) {
        printf("%s", rotulo);

        if (fgets(destino, tamanho, stdin) == NULL) {
            /* NULL indica fim de arquivo, erro de leitura ou entrada interrompida. */
            printf("\nFim inesperado da entrada. Encerrando leitura.\n");
            return 0;
        }

        if (linha_estourou_buffer(destino)) {
            /* Limpa o excesso para a proxima chamada de fgets ler uma linha nova. */
            limpar_resto_da_linha();
            printf("Entrada muito longa. Use no maximo %zu caracteres.\n", tamanho - 1);
            continue;
        }

        remover_quebra_de_linha(destino);

        if (texto_vazio(destino)) {
            printf("Entrada vazia. Informe um texto valido.\n");
            continue;
        }

        entrada_valida = 1;
    }

    return 1;
}

/*
 * Confere se depois do numero digitado existem somente espacos.
 *
 * Isso impede que entradas como "12abc" sejam aceitas como se fossem apenas
 * o numero 12.
 */
static int texto_tem_apenas_espacos_finais(const char *texto)
{
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }
        texto++;
    }

    return 1;
}

/*
 * Le um numero inteiro dentro de um intervalo.
 *
 * A conversao usa strtol em vez de scanf. Com strtol conseguimos verificar:
 * - se o usuario realmente digitou um numero;
 * - se sobrou lixo depois do numero;
 * - se o numero esta dentro do intervalo permitido;
 * - se houve estouro de valor numerico.
 */
static int ler_numero_intervalo(const char *rotulo, int minimo, int maximo, int *valor)
{
    char linha[64];
    char *fim = NULL;
    long numero;

    while (1) {
        printf("%s", rotulo);

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            printf("\nFim inesperado da entrada. Encerrando leitura.\n");
            return 0;
        }

        if (linha_estourou_buffer(linha)) {
            limpar_resto_da_linha();
            printf("Entrada muito longa. Informe um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        remover_quebra_de_linha(linha);

        if (texto_vazio(linha)) {
            printf("Entrada vazia. Informe um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        errno = 0;
        numero = strtol(linha, &fim, 10);

        /*
         * fim == linha: nenhum numero foi lido.
         * !texto_tem_apenas_espacos_finais(fim): existe texto depois do numero.
         */
        if (fim == linha || !texto_tem_apenas_espacos_finais(fim)) {
            printf("Entrada invalida. Informe apenas numeros.\n");
            continue;
        }

        /* ERANGE indica que o numero digitado estourou o limite do tipo long. */
        if (errno == ERANGE || numero < minimo || numero > maximo) {
            printf("Valor fora do intervalo. Informe um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        if (numero < INT_MIN || numero > INT_MAX) {
            printf("Valor fora do intervalo permitido para inteiro.\n");
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}

/*
 * Mostra o menu principal exatamente com as seis opcoes do projeto.
 * Esta funcao apenas imprime o menu; ela nao le nem executa nenhuma opcao.
 */
static void exibir_menu(void)
{
    printf("\n===== Player de Musicas =====\n");
    printf("1. Adicionar musica\n");
    printf("2. Proxima musica\n");
    printf("3. Musica anterior\n");
    printf("4. Exibir musica atual\n");
    printf("5. Listar playlist\n");
    printf("6. Sair\n");
}

/*
 * Le e valida a opcao do menu.
 *
 * Esta funcao e parecida com ler_numero_intervalo, mas foi separada para
 * permitir mensagens mais especificas, como "Opcao invalida" quando o numero
 * nao esta entre 1 e 6.
 */
static int ler_opcao_menu(int *opcao)
{
    char linha[64];
    char *fim = NULL;
    long numero;

    while (1) {
        printf("Escolha uma opcao: ");

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            printf("\nFim inesperado da entrada. Encerrando leitura.\n");
            return 0;
        }

        if (linha_estourou_buffer(linha)) {
            limpar_resto_da_linha();
            printf("Entrada muito longa. Informe uma opcao entre %d e %d.\n",
                   OPCAO_ADICIONAR,
                   OPCAO_SAIR);
            continue;
        }

        remover_quebra_de_linha(linha);

        if (texto_vazio(linha)) {
            printf("Entrada vazia. Informe uma opcao entre %d e %d.\n",
                   OPCAO_ADICIONAR,
                   OPCAO_SAIR);
            continue;
        }

        errno = 0;
        numero = strtol(linha, &fim, 10);

        if (fim == linha || !texto_tem_apenas_espacos_finais(fim)) {
            printf("Entrada invalida. Informe apenas numeros.\n");
            continue;
        }

        if (errno == ERANGE || numero < OPCAO_ADICIONAR || numero > OPCAO_SAIR) {
            printf("Opcao invalida. Escolha uma opcao entre %d e %d.\n",
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
    printf("\n--- Adicionar Musica ---\n");

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
    printf("Titulo: %s\n", musica->titulo);
    printf("Artista: %s\n", musica->artista);
    printf("Album: %s\n", musica->album);
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
        printf("Copiando %zu musica(s) existente(s)...\n", total_anterior);
        printf("Liberando vetor anterior...\n");
    }

    if (!playlist_adicionar(playlist, &musica)) {
        printf("\nErro ao adicionar musica. Verifique a memoria disponivel.\n");
        return;
    }

    /* Depois da insercao, mostramos um resumo para confirmar a operacao. */
    printf("\nMusica adicionada com sucesso!\n");
    printf("Total de musicas: %zu\n", playlist->total_musicas);

    /* Caso especial: a primeira musica cadastrada vira a musica atual. */
    if (total_anterior == 0 && playlist->indice_atual == 0) {
        printf("Musica atual definida automaticamente (indice 0).\n");
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
        printf("\nNenhuma musica na playlist.\n");
        return;
    }

    if (!playlist_proxima(playlist)) {
        printf("\nVoce ja esta na ultima musica da playlist.\n");
        printf("Musica atual permanece:\n");
    } else {
        printf("\n--- Proxima Musica ---\n");
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
        printf("\nNenhuma musica na playlist.\n");
        return;
    }

    if (!playlist_anterior(playlist)) {
        printf("\nVoce ja esta na primeira musica da playlist.\n");
        printf("Musica atual permanece:\n");
    } else {
        printf("\n--- Musica Anterior ---\n");
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
        printf("\nNenhuma musica na playlist.\n");
        return;
    }

    musica = playlist_atual(playlist);
    if (musica == NULL) {
        printf("\nNenhuma musica na playlist.\n");
        return;
    }

    printf("\n--- Musica Atual ---\n");
    exibir_musica(musica);
    printf("Posicao: %d de %zu\n", playlist->indice_atual + 1, playlist->total_musicas);
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

        if (musica != NULL) {
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
    }

    printf("\n(> indica a musica atual)\n");
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
    printf("Playlist inicializada (ponteiro NULL, total = 0, indice atual = -1).\n");

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
        case OPCAO_SAIR:
            continuar = 0;
            break;
        default:
            printf("\nOpcao invalida.\n");
            break;
        }
    }

    /*
     * A limpeza real da memoria pertence ao modulo da playlist.
     * Mesmo assim, o menu mostra as mensagens de encerramento esperadas.
     */
    printf("\nLiberando vetor da playlist...\n");
    playlist_liberar(&playlist);
    printf("Memoria liberada com sucesso.\n");
    printf("Encerrando o programa.\n");

    return 0;
}
