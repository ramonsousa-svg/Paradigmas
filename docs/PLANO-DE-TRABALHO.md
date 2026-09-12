# Plano de trabalho da equipe

## Resultado esperado

Entregar um player de músicas em C, com menu de terminal, playlist representada por vetor dinâmico de `Musica`, navegação com índice atual, validação, testes demonstráveis e liberação completa da memória.

## Quadro de atividades

### Pessoa 1 — núcleo técnico e integração

- definir `typedef struct Musica` e o contrato da playlist;
- implementar inserção com novo vetor, cópia e `free` do vetor anterior;
- implementar índice atual e navegação com limites;
- implementar busca e ordenação quando os requisitos forem confirmados;
- revisar falhas de `malloc`, ponteiros, índices e encerramento;
- integrar as branches e preparar a versão final.

### Pessoa 2 — requisitos, decisões e apresentação

- transformar o enunciado em uma lista de requisitos verificáveis;
- manter o glossário de campos e funções;
- documentar as decisões: índice `-1`, playlist vazia, limites e estratégia de alocação;
- preparar o roteiro da apresentação e a explicação do paradigma imperativo.

#### Situação da entrega da Pessoa 2

Entrega parcial. `ARQUITETURA.md` e `GLOSSARIO.md` descrevem conceitos e contratos. Manter os identificadores Pessoa 1–4 nesta etapa.

- [x] Registrar estado inicial e estratégia de memória em `ARQUITETURA.md`.
- [x] Registrar tabela de campos abaixo.
- [x] Consolidar requisitos obrigatórios e relacionar cada um a um teste (`docs/REQUISITOS.md`).
- [x] Definir contratos de busca e ordenação (`docs/GLOSSARIO.md` — "Contratos decididos").
- [x] Completar o glossário de funções e a explicação do paradigma imperativo (`docs/GLOSSARIO.md`).
- [x] Preparar e ensaiar o roteiro da apresentação (`docs/APRESENTACAO.md`).
- [x] Revisar instruções do README frente à versão final validada (UTF-8 nos menus, link para o roteiro).

#### Campos de `Musica`

| Campo | Tipo em C | Capacidade/regra | Exemplo |
|---|---|---|---|
| `titulo` | `char[100]` | Até 99 bytes de texto mais `\0`; obrigatório, não apenas espaços. | `Imagine` |
| `artista` | `char[100]` | Até 99 bytes de texto mais `\0`; obrigatório, não apenas espaços. | `John Lennon` |
| `album` | `char[100]` | Até 99 bytes de texto mais `\0`; obrigatório, não apenas espaços. | `Imagine` |
| `ano` | `int` | Inteiro entre 1900 e 2100, inclusive, conforme validação atual do menu. | `1971` |

Referências: `src/playlist.h` e `src/main.c`. A faixa de anos é uma escolha da implementação a confirmar com a equipe, não uma exigência comprovada do enunciado. Capacidade em bytes não equivale necessariamente a quantidade de caracteres acentuados.

Pendência identificada no review: a leitura aceita 99 bytes + Enter (limite real), mas caracteres acentuados podem ocupar mais de um byte, reduzindo o limite efetivo de caracteres. Validar com Pessoas 3 e 4; nenhuma correção de código nesta etapa.

### Pessoa 3 — menu, entradas e mensagens

- organizar o menu principal;
- criar funções de leitura para textos e números;
- tratar entrada vazia, texto no lugar de número e ano inválido;
- conferir que as mensagens do programa correspondem aos testes;
- evitar `scanf("%s", ...)` sem limite e entradas que causem overflow.

### Pessoa 4 — testes e qualidade

- executar os dez testes de `TESTES.md`;
- criar casos de fronteira: playlist vazia, primeira/última música e falha de entrada;
- registrar resultado esperado versus resultado obtido;
- revisar clareza do código e da documentação;
- apoiar a demonstração final.

## Reuniões rápidas

Em cada encontro, cada pessoa responde:

1. O que concluí?
2. O que farei em seguida?
3. Estou bloqueado por alguma decisão ou dependência?

Bloqueios técnicos devem ser encaminhados à Pessoa 1; bloqueios de requisito devem ser registrados por Pessoa 2; falhas reproduzíveis devem ir para Pessoa 4.

## Definition of Done

Uma tarefa só é considerada concluída quando possui código ou documento entregue, instrução de teste, resultado registrado e revisão de outra pessoa.
