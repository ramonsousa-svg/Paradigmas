# Roteiro da apresentação — Player de Músicas (5 min)

---

## Bloco 1 — Abertura (30 s)

> Bom dia/boa tarde. Eu vou apresentar o **Player de Músicas**, um projeto
> que a equipe construiu em C pra disciplina de Paradigmas.
>
> Basicamente é um programa de linha de comando: a gente cadastra as músicas,
> navega pela playlist com próximo e anterior, busca por trecho, e quando sai
> o programa libera toda a memória.

---

## Bloco 2 — Paradigma imperativo (1 min)

> Neste projeto, utilizamos o paradigma imperativo, mudando o estado passo a passo.
>
> Primeiro, a **sequência**: o `main` vai executando instrução uma após a outra.
> Inicializa a playlist, mostra o menu, lê a opção, executa o `case`, volta pro
> menu. Cada linha assume que a anterior já rodou.
>
> Aí vem a **seleção**: o `switch` da opção escolhe um caminho entre sete, e cada
> `case` chama uma função diferente. É o único jeito do programa decidir o que
> fazer com base no que a pessoa digitou.
>
> E por fim a **repetição**: o `while (continuar)` segura o menu em loop até a
> pessoa escolher Sair. Dentro das funções, o `for` percorre o vetor de músicas,
> e o `while` da validação fica lendo a entrada até ela ser aceitável.
>
> Sem essa tríade o player não vira playlist — vira um script que roda uma vez
> e morre.

---

## Bloco 3 — Struct e estado (1 min)

> O coração do programa é o `typedef struct Playlist`, que guarda um ponteiro
> `Musica *musicas`, o `total_musicas` e o `indice_atual`. Esse struct é o estado.
>
> Estado em C mora na memória que a gente aloca e libera. Aqui a gente tomou
> três decisões.
>
> **Playlist vazia:** ponteiro `NULL`, total zero, índice `-1`. Por que `-1`?
> Porque "sem música atual" é uma posição que não existe — e `indice_atual + 1`
> dá zero, que não confunde com "posição 1".
>
> **Primeira inserção:** quando o vetor é `NULL`, a gente aloca um vetor com 1
> elemento. Aí o índice atual vira `0` automaticamente.
>
> **Campos da música:** título, artista, álbum, ano. Tudo com `char[100]` pra
> texto e `int` pro ano. O limite de 99 bytes é o que cabe no buffer mais o
> terminador — testamos com um caso de 100 bytes e o programa recusa.

---

## Bloco 4 — Memória (1 min) [+ demo embutida]

> Aqui é a parte que o C obriga a gente a pensar: quem aloca, libera.
>
> A estratégia é simples. Cada `playlist_adicionar` aloca um vetor novo, um
> elemento maior que o anterior, copia os registros existentes, libera o vetor
> antigo e atualiza o ponteiro da struct. A gente nunca realoca no lugar — sempre
> vetor novo e troca de ponteiro. Isso evita dois problemas clássicos: realocação
> que falha silenciosamente e ponteiro solto.
>
> A validação de `malloc` é explícita. Se a alocação falhar, a função retorna
> `false` e o menu mostra uma mensagem de erro. Sem cair em comportamento
> indefinido.
>
> E o `playlist_liberar` é chamado sempre que a pessoa escolhe Sair. Percorre o
> vetor, chama `free`, devolve `NULL` ao ponteiro e zera os contadores. Sem
> double-free, sem leak conhecido.
>
> *(Nessa hora, roda `player.exe` ao vivo: adiciona duas músicas, usa próxima e
> anterior, lista. A turma vê a navegação funcionando. ~30s.)*

---

## Bloco 5 — Navegação, busca e testes (1 min)

> A navegação ficou isolada em `playlist_proxima` e `playlist_anterior`. A regra
> é a mesma nos dois: retorna `false` quando já estamos no limite, e o menu
> mostra "já está na última/primeira" sem mexer no estado. O programa não dá
> erro — só informa.
>
> A busca por trecho foi a extensão do projeto. Ignora maiúsculas, procura no
> título e no artista, e devolve até 64 resultados. Se vier um resultado único,
> vira música atual automaticamente; se vier vários, a pessoa escolhe. A regra
> do limite de 64 com aviso está documentada — não é bug, é contrato.
>
> Os testes são cinco: adicionar, navegar, exibir, buscar e ordenar, mais um de
> fronteira que valida o limite de 99 bytes. Todos passam. Os testes vivem em
> `tests/` e compilam separados do `main.c` — o que confirma que o núcleo é
> independente do menu.
>
> Então é isso: player de músicas em C, paradigma imperativo na essência,
> struct com estado bem definido, memória alocada e liberada em par, navegação
> com limites protegidos, busca parcial com contrato claro, e testes que provam
> que funciona.

