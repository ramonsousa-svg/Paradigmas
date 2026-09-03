# Handoff técnico — Pessoa 1

Este documento explica o que já foi implementado no núcleo da playlist e como o restante do time deve usar essas funções. A ideia é permitir a integração do menu sem que outra pessoa precise entender todos os detalhes de ponteiros e memória.

## Arquivos entregues

- `src/playlist.h`: tipos e funções públicas.
- `src/playlist.c`: implementação da playlist.
- `tests/`: testes independentes do menu.

Para compilar um programa que use a playlist, inclua `src/playlist.h` e compile também `src/playlist.c`.

## Estado da playlist

```c
Playlist playlist;
playlist_inicializar(&playlist);
```

Depois da inicialização:

- `playlist.musicas == NULL`;
- `playlist.total_musicas == 0`;
- `playlist.indice_atual == -1`.

O índice `-1` significa que ainda não existe música atual. Ao adicionar a primeira música, ele passa automaticamente para `0`.

## Funções disponíveis

| Função | O que faz | Retorno |
|---|---|---|
| `playlist_inicializar` | prepara uma playlist vazia | nenhum |
| `playlist_liberar` | libera a memória e reseta o estado | nenhum |
| `playlist_vazia` | verifica se não há músicas | `true` ou `false` |
| `playlist_adicionar` | adiciona uma música ao final | `true` se conseguiu |
| `playlist_proxima` | avança uma posição | `true` se avançou |
| `playlist_anterior` | volta uma posição | `true` se voltou |
| `playlist_atual` | acessa a música atual | ponteiro ou `NULL` |
| `playlist_exibir_atual` | imprime os dados da música atual | `true` se exibiu |
| `playlist_listar` | imprime todas as músicas | `true` se listou |
| `playlist_buscar_titulo` | busca título exato | índice ou `-1` |
| `playlist_ordenar_por_titulo` | ordena por título | nenhum |

## Como o menu deve chamar as funções

```c
Playlist playlist;
playlist_inicializar(&playlist);

/* A opção 1 deve preencher uma Musica e passá-la por endereço. */
Musica musica = {"Imagine", "John Lennon", "Imagine", 1971};
if (!playlist_adicionar(&playlist, &musica)) {
    /* Informar que a música não pôde ser adicionada. */
}

/* As opções 2 e 3 usam o retorno para tratar os limites. */
if (!playlist_proxima(&playlist)) {
    /* Informar que a playlist está vazia ou já está no fim. */
}

if (!playlist_anterior(&playlist)) {
    /* Informar que a playlist está vazia ou já está no início. */
}

/* stdout representa a tela; a função também aceita um arquivo de teste. */
playlist_exibir_atual(&playlist, stdout);
playlist_listar(&playlist, stdout);

/* Sempre liberar antes de sair do programa. */
playlist_liberar(&playlist);
```

`stdout` é a saída padrão do programa, normalmente o terminal. Nas mensagens formatadas, `%s` imprime texto, `%d` imprime inteiro decimal e `%zu` imprime um valor de `size_t`, usado para quantidades e posições.

## Regra importante sobre memória

`playlist_atual` devolve um ponteiro emprestado para uma música que está dentro do vetor. O menu pode ler os campos, mas não deve chamar `free` nesse ponteiro.

Esse ponteiro pode deixar de ser válido depois de `playlist_adicionar`, porque a inserção cria um vetor novo, ou depois de `playlist_liberar`. Portanto, leia os dados logo após chamar `playlist_atual` e não guarde o ponteiro para uso posterior.

## Busca e ordenação

`playlist_buscar_titulo` faz comparação exata e diferencia maiúsculas de minúsculas. Por exemplo, `"Imagine"` e `"imagine"` são títulos diferentes. O resultado é o índice da música ou `-1` quando não encontrada.

`playlist_ordenar_por_titulo` reorganiza o vetor em ordem alfabética e atualiza `indice_atual` para que a mesma música continue marcada como atual.

## Validação já feita

Os testes abaixo foram compilados com GCC 16.2.0 e estes avisos:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic
```

- inserção da primeira e segunda música;
- navegação em playlist vazia;
- limites da primeira e última música;
- exibição e listagem;
- busca de título existente e inexistente;
- ordenação mantendo a música atual;
- liberação da playlist.

## O que ainda falta integrar

O núcleo técnico está pronto. Ainda falta ao menu:

1. criar `src/main.c`;
2. mostrar as seis opções;
3. ler textos e números com segurança;
4. chamar as funções deste handoff em cada opção;
5. tratar as mensagens para o usuário;
6. compilar o programa completo;
7. executar a bateria T1–T10 pelo menu.

Esses itens são a próxima frente da Pessoa 3, com apoio da Pessoa 4 nos testes. Alterações no contrato das funções devem ser combinadas antes de modificar `playlist.h`.
