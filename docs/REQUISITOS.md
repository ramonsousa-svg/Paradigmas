
# Requisitos e matriz requisito–teste

Status: consolidado pela Pessoa 2 a partir do enunciado (`docs/Paradigmas de Programação - AP1 - Testes para o projeto 3.md`), do código atual (`src/`) e da bateria `docs/TESTES.md`. Contratos de busca/ordenação decididos em `docs/GLOSSARIO.md`.

## Requisitos obrigatórios (AP1)

| Req | Descrição | Teste |
|---|---|---|
| R-01 | Estado inicial: vetor `NULL`, total `0`, índice `-1` | T1 |
| R-02 | Exibir/listar playlist vazia informa "Nenhuma musica"/"Playlist vazia" sem travar | T1, C-06 |
| R-03 | Primeira inserção: capacidade 1, total 1, índice passa a 0 | T2 |
| R-04 | Segunda inserção: novo vetor, cópia do anterior, liberação do vetor antigo | T3 |
| R-05 | Próxima música: índice avança | T4 |
| R-06 | Avanço além da última: índice não muda; mensagem "já está na última" | T5 |
| R-07 | Música anterior: índice retrocede | T6 |
| R-08 | Recuo antes da primeira: índice não muda; mensagem "já está na primeira" | T7 |
| R-09 | Listagem com `>` marcando a música atual | T8 |
| R-10 | Exibição da atual: título, artista, álbum, ano e posição (1-based) | T9 |
| R-11 | Fluxo completo + saída: liberação de memória e mensagens de encerramento | T10 |
| R-12 | Entradas inválidas não encerram nem corrompem o programa | C-01 a C-06 |

## Casos de qualidade

| ID | Caso | Referência |
|---|---|---|
| C-01 | Opção de menu inexistente | `docs/TESTES.md:20` |
| C-02 | Texto vazio | `docs/TESTES.md:21` |
| C-03 | Texto maior que o campo | `docs/TESTES.md:22` |
| C-04 | Letras onde se espera número | `docs/TESTES.md:23` |
| C-05 | Ano fora de 1900–2100 | `docs/TESTES.md:24` |
| C-06 | Navegação em playlist vazia | `docs/TESTES.md:25` |
| C-07 | Falha de alocação (se houver mecanismo) | `docs/TESTES.md:26` |
| C-08 | Sanitizador/ferramenta equivalente | `docs/TESTES.md:27` |

## Extensões além do enunciado (exigem aprovação da equipe)

| Req | Descrição | Estado | Teste |
|---|---|---|---|
| E-01 | Busca parcial (título/artista, case-insensitive), opção 7 do menu | Implementada, fora do enunciado | `tests/test_playlist_search_sort.c` |
| E-02 | Seleção de resultado vira música atual; 1 resultado seleciona automático | Implementada | `tests/test_playlist_search_sort.c` |
| E-03 | Busca exata por título (núcleo, sem opção no menu) | Implementada | `tests/test_playlist_search_sort.c` |
| E-04 | Ordenação por título preservando a atual | Implementada, sem opção no menu | `tests/test_playlist_search_sort.c` |

## Contratos além do enunciado (decididos pela Pessoa 2)

Contratos de busca e ordenação fechados em `docs/GLOSSARIO.md` (seção "Contratos decididos").

## Propostas futuras (não requisitos)

Ideias levantadas em brainstorm com a Pessoa 2, sem aprovação nem compromisso de implementação:

| Ideia | Resumo da preferência levantada |
|---|---|
| Duplicatas | Avisar quando título+artista coincidirem; permitir confirmar (inclusive versões distintas). Exceção: UC-01/A4 já registra o aviso como requisito pendente de código |
| Remoção/edição | Remover a atual seleciona a próxima, senão a anterior; lista vazia volta ao estado inicial |
| Persistência | Salvar/carregar manual; avisar ao sair com alterações não salvas; informar falhas sem fingir sucesso |
| Navegação | Aleatório e repetição da lista como modos opcionais, desligados por padrão; sem reprodução de áudio |

## Casos de uso

Ator único: **usuário** no terminal. Formato compacto: pré-condição, fluxo principal, fluxos alternativos, resultado.

| UC | Pré-condição | Fluxo principal | Fluxos alternativos | Resultado |
|---|---|---|---|---|
| UC-01 Adicionar | nenhuma (qualquer estado) | 1. usuário escolhe Adicionar; 2. informa título, artista, álbum (não vazios, ≤99 bytes) e ano (1900–2100); 3. programa aloca vetor maior, copia, adiciona, libera o anterior | A1. Campo vazio → mensagem, pede de novo. A2. Texto >99 bytes → mensagem, pede de novo. A3. Ano inválido → mensagem, pede de novo. A4. Título+artista já cadastrados → **proposto, não implementado**: avisar e pedir confirmação | Música no fim; se 1ª, vira atual (índice 0) |
| UC-02 Próxima | nenhuma | 1. usuário escolhe Próxima | A1. Playlist vazia → "Nenhuma musica na playlist". A2. Já na última → "já está na última", índice intacto | Índice avança e exibe a nova música, ou mensagem de limite |
| UC-03 Anterior | nenhuma | 1. usuário escolhe Anterior | A1. Playlist vazia → "Nenhuma musica na playlist". A2. Já na primeira → "já está na primeira", índice intacto | Índice recua e exibe a anterior, ou mensagem de limite |
| UC-04 Exibir atual | nenhuma | 1. usuário escolhe Exibir | A1. Playlist vazia → "Nenhuma musica na playlist" | Título, artista, álbum, ano e posição exibidos |
| UC-05 Listar | nenhuma | 1. usuário escolhe Listar | A1. Playlist vazia → "Playlist vazia." | Todas listadas com `>` na atual |
| UC-06 Sair (opção 7) | nenhuma | 1. usuário escolhe Sair | — | Vetor liberado; mensagens de encerramento; programa termina |
| UC-07 Buscar (opção 6, extensão) | playlist não vazia (se vazia → "Nenhuma musica na playlist") | 1. usuário informa termo; 2. programa lista `[n] título \| artista` (máx. 64 + aviso) | A1. Termo vazio → rejeita. A2. Nada encontrado → mensagem, nada muda. A3. 1 resultado → define atual automaticamente. A4. Vários → usuário digita número (0 cancela, nada muda) | Música atual passa ao selecionado, ou permanece |

Nota: UC-01/A4 exige implementação futura; até lá, duplicatas são permitidas silenciosamente. Registrar como requisito pendente de código.
