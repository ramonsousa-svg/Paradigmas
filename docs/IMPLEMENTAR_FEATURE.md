# Como implementar uma feature nova

Guia da Pessoa 2 para levar uma ideia de proposta a entrega, sem quebrar o que já funciona.

## 0. Meça o tamanho antes de prometer

Toda feature nova cai num destes níveis — identifique o seu antes de comprometer prazo:

| Nível | Exemplo | Custo típico |
|---|---|---|
| Campo/validação | novo campo em `Musica`, nova regra de ano | 1 PR, sem dependências |
| Operação no núcleo | remover, editar, ordenar por outro critério | 1–2 PRs, testes unitários |
| Opção de menu | expor operação existente (ex.: busca foi assim) | 1 PR, renumeração do menu |
| Persistência/modos | salvar em arquivo, aleatório, repetição | 2+ PRs, novos estados e erros |
| Dependência externa | tocar áudio, rede, banco de dados | multiplica o escopo: lib, instalação, plataforma, testes manuais |

Regra: níveis 4–5 entram como **proposta futura**, nunca como compromisso de entrega corrente.

## Onde implementar

| Camada | Arquivos | Responsabilidade |
|---|---|---|
| Núcleo | `src/playlist.c`, `src/playlist.h` | Estado, vetor dinâmico, regras; sem `printf` de menu, sem leitura de teclado |
| Utilitários | `src/io.c`, `src/io.h` | Leitura segura de texto/números; sem conhecer playlist ou menu |
| Interface | `src/main.c` | Menu, mensagens e coordenação; sem `malloc` direto da playlist |
| Testes | `tests/test_*.c` | Um executável por área; `assert` para núcleo, execução do `player.exe` para fronteiras |
| Docs | `docs/` | `REQUISITOS.md` (o quê), `GLOSSARIO.md` (contratos), README (como usar) |

Fluxo de dependência: `main.c` chama `playlist.h`; nunca o contrário. Teste importa só `playlist.h` + `playlist.c`.

## Passo a passo

### 1. Registrar a proposta (Pessoa 2)

Uma linha na tabela "Propostas futuras" em `docs/REQUISITOS.md`:

| Ideia | Resumo da preferência |
|---|---|
| <nome> | <comportamento desejado em 1 frase> |

Sem código ainda.

### 2. Fechar o contrato (Pessoa 2 decide)

Responda por escrito em `docs/GLOSSARIO.md` (seção "Contratos decididos"):

1. Pré-condição? (ex.: playlist não vazia)
2. Entradas válidas e inválidas? (vazio, excesso, fora de faixa)
3. O que acontece em cada erro? (mensagem exata, estado intacto, sem travar)
4. Efeito no estado? (índice muda? vetor muda? nada muda?)
5. Limites? (máximo de resultados, formatos aceitos, plataformas)
6. Como testar sem o recurso real? (modo simulado que só imprime?)

Sem essas respostas, a Pessoa 1 não começa.

### 3. Quebrar em entregas mínimas (Pessoa 2 + Pessoa 1)

Ordem sugerida, uma PR por item:

1. **Dados** — campos/structs novos + validação, sem comportamento novo.
2. **Núcleo simulado ou real** — função em `playlist.c` + testes unitários.
3. **Menu** — opção nova (nunca reutilize número existente), mensagens, validação de entrada.
4. **Dependência externa (se houver)** — só depois do passo 2 funcionar; documentar instalação no README para Windows **e** Linux antes do merge.
5. **Regressão** — reexecutar T1–T10 + bateria completa.

Se qualquer etapa estourar, as anteriores continuam úteis (YAGNI: nunca instale dependência antes do fluxo funcionar sem ela).

### 4. Implementar (Pessoa 1 + Pessoa 3)

- Pessoa 1: função no núcleo retornando `false`/`NULL` em erro, sem travar, sem I/O de menu.
- Pessoa 3: opção no menu, leitura segura, mensagens do contrato.
- Pessoa 4: casos novos em `docs/TESTES.md` + reexecução total da bateria.

### 5. Documentar e fechar (Pessoa 2)

- Mover a linha de "Propostas futuras" para requisitos, com ID (ex.: R-13).
- Contrato da nova função no `GLOSSARIO.md`.
- Nova opção no README ("Como usar").
- Checklist em `docs/PLANO-DE-TRABALHO.md`.

## Checklist rápido

- [ ] Proposta em `REQUISITOS.md` (1 linha)
- [ ] Contrato fechado no `GLOSSARIO.md` (6 perguntas respondidas)
- [ ] Quebra em PRs mínimas (dados → núcleo → menu → dependência → regressão)
- [ ] Pessoa 4 reexecutou a bateria sem regressão
- [ ] README, REQUISITOS e GLOSSARIO atualizados
- [ ] Nenhum número de opção do menu reutilizado
