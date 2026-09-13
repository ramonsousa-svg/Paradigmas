
# Glossário

## Conceitos

- **Musica** — struct com titulo, artista, album e ano.
- **Playlist** — vetor de `Musica` + `total_musicas` + `indice_atual`.
- **Vetor dinâmico** — memória alocada com `malloc` e ajustada a cada inserção.
- **índice_atual** — posição da música atual; `-1` significa nenhuma música.
- **musica atual** — música indicada por `>` na listagem e exibida na opção 4.
- **busca parcial** — encontra um trecho no título ou no artista, ignorando caixa.

## Contratos decididos (Pessoa 2, 12/09/2026)

Decisões da Pessoa 2 conforme atribuição de "decisões de projeto" (`README.md`). Encerram as perguntas abertas levantadas em brainstorm para a AP1.

### Busca (opção 6)

- **Termo inválido:** vazio ou só espaços é rejeitado, pede nova entrada.
- **Sem correspondência:** mensagem `Nenhuma musica encontrada para "<termo>"`; a música atual não muda.
- **Resultados:** lista numerada `[n] titulo | artista`, com a posição original preservada. Sem limite prático: máximo de 64 posições por busca; ao atingir, exibe aviso de truncamento e pede refinamento do termo.
- **Maiúsculas/minúsculas:** ignoradas (ASCII).
- **Acentos:** **não** tratados — `Joao` **não** encontra `João` (comparação byte a byte; normalização UTF-8 fora do escopo da AP1).
- **Seleção:** 1 resultado → vira música atual automaticamente. Vários → usuário digita o número (0 cancela, nada muda). Faixa validada: `0..encontradas`, nunca posição fora dos resultados.
- **Campos pesquisados:** `titulo` e `artista` apenas.

### Ordenação

- Critério: `strcmp` por título — case-sensitive e byte a byte (acentos não normalizados).
- Preserva a música atual: após `qsort`, a mesma música continua marcada com `>`.
- Sem opção no menu na AP1 (função do núcleo, testada em `tests/test_playlist_search_sort.c`).

### Busca exata

- `playlist_buscar_titulo`: título completo, case-sensitive, retorna a primeira posição ou `-1`. Sem opção no menu.

## Funções do núcleo (`src/playlist.h`)

| Função | Contrato |
|---|---|
| `playlist_inicializar` | Prepara estado vazio: vetor `NULL`, total `0`, índice `-1`. |
| `playlist_liberar` | Libera o vetor e reseta o estado inicial. |
| `playlist_vazia` | `true` quando não há músicas (playlist nula também é vazia). |
| `playlist_adicionar` | Aloca vetor maior, copia existentes, adiciona ao fim; primeira inserção define índice `0`. Retorna `false` se alocação falhar. |
| `playlist_proxima` | Avança o índice; retorna `false` na última música (índice intacto). |
| `playlist_anterior` | Recua o índice; retorna `false` na primeira (índice intacto). |
| `playlist_atual` | Ponteiro somente leitura para a música atual; `NULL` se inválida. |
| `playlist_buscar_titulo` | Posição do título exato (case-sensitive) ou `-1`. |
| `playlist_buscar_parcial` | Trecho em título/artista, case-insensitive; preenche vetor de posições e retorna a contagem (máx. `maximo`). |
| `playlist_ordenar_por_titulo` | `qsort` alfabético por título; repõe a música atual na nova posição. |
| `playlist_definir_atual` | Define a atual por posição; `false` se posição inválida. |

## Funções de entrada (`src/io.h`)

| Função | Contrato |
|---|---|
| `linha_estourou_buffer` | Verdadeiro quando o `fgets` não leu a linha inteira. |
| `limpar_resto_da_linha` | Descarta o restante da linha atual. |
| `remover_quebra_de_linha` | Remove o `\n` final deixado pelo `fgets`. |
| `texto_vazio` | Verdadeiro para texto vazio ou só espaços (também valida o resto após um número). |
| `ler_texto` | Lê string até `tamanho - 1` bytes; rejeita vazio e excesso. |
| `ler_numero_intervalo` | Lê inteiro via `strtol`; rejeita não-numérico, fora da faixa e estouro. |

## Funções da interface (`src/main.c`)

| Função | Contrato |
|---|---|
| `ler_opcao_menu` | Lê a opção do menu (1–7) com mensagens específicas. |
| `exibir_menu` | Imprime as 7 opções (6 do enunciado + Buscar). |
| `adicionar_pelo_menu` / `avancar_pelo_menu` / `retroceder_pelo_menu` / `exibir_atual_pelo_menu` / `listar_pelo_menu` | Coordenam cada opção do menu com as mensagens esperadas pelos testes. |
| `buscar_pelo_menu` | Lê o termo, lista resultados e define a atual (1 resultado = automático; vários = usuário escolhe; 0 = cancela). |

## Paradigma imperativo

O programa é uma sequência de comandos que **mudam o estado** passo a passo:

- **Estado:** a `Playlist` (vetor + total + índice) é o estado vivo do programa; cada função lê ou altera esse estado.
- **Sequência:** o `main` repete um laço (`while`) — imprimir menu, ler opção, executar, até `OPCAO_SAIR`.
- **Decisão:** `switch`/`if` escolhem o caminho conforme a opção e os limites (índice em `0`..`total-1`).
- **Mutação:** não há funções puras; `playlist_proxima`, por exemplo, altera `indice_atual` diretamente. Efeitos colaterais são o mecanismo principal.
- **Memória manual:** `malloc`/`free` controlados pelo programador — crescer o vetor a cada inserção e liberar ao encerrar, sem coletor de lixo.
- **Sem objetos/métodos:** dados (`struct`) e operações (funções livres) são separados; a operação recebe o dado por ponteiro.
