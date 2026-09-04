# Player de Músicas — Paradigmas de Programação

Projeto integrador da disciplina **Paradigmas de Programação**, desenvolvido em C para praticar o paradigma imperativo, structs, vetores, funções, ponteiros e memória dinâmica.

## Objetivo

Construir uma aplicação de linha de comando que gerencie uma playlist de músicas. O programa deverá permitir:

- adicionar músicas;
- avançar e voltar na playlist;
- exibir a música atual;
- listar a playlist com indicação da música atual;
- validar entradas e tratar erros;
- buscar e ordenar registros, conforme a evolução do projeto;
- liberar toda a memória alocada ao encerrar.

## Escopo inicial

Cada música possui `titulo`, `artista`, `album` e `ano`. A playlist começa vazia, com ponteiro `NULL`, total igual a zero e índice atual igual a `-1`. Cada nova inserção deve criar um vetor maior, copiar os registros existentes e liberar o vetor anterior.

O menu de referência está documentado em [`docs/TESTES.md`](docs/TESTES.md). O passo a passo de cada integrante está em [`docs/GUIA-DA-EQUIPE.md`](docs/GUIA-DA-EQUIPE.md). Os requisitos completos e os exemplos de comportamento estão em [`docs/Paradigmas de Programação - AP1 - Testes para o projeto 3.md`](docs/Paradigmas%20de%20Programação%20-%20AP1%20-%20Testes%20para%20o%20projeto%203.md).

## Divisão do trabalho

Os nomes abaixo são papéis. Substituam `Pessoa 2`, `Pessoa 3` e `Pessoa 4` pelos nomes do time.

| Pessoa | Responsabilidade principal | Entregas |
|---|---|---|
| **Pessoa 1** | Implementação técnica e integração | modelagem em C, vetor dinâmico, `malloc`/`free`, navegação, busca/ordenação, integração final e correção de problemas de memória |
| **Pessoa 2** | Requisitos e documentação | regras do sistema, casos de uso, decisões de projeto, organização do README e roteiro da apresentação |
| **Pessoa 3** | Menu e experiência de uso | fluxo do menu, mensagens exibidas, leitura segura de strings/números e validação das entradas |
| **Pessoa 4** | Testes e qualidade | execução dos testes, casos de fronteira, registro de resultados, revisão dos critérios e apoio à apresentação |

### Como as atividades dependem umas das outras

1. Pessoa 2 consolida as regras e o comportamento esperado.
2. Pessoa 1 define a estrutura do código e a API das funções.
3. Pessoa 3 implementa/revisa o fluxo de entrada usando essa API.
4. Pessoa 4 transforma os requisitos em testes, registra falhas e valida as correções.
5. Pessoa 1 integra a versão final e faz a revisão de ponteiros, limites e memória.

Ninguém precisa trabalhar sozinho: dúvidas e mudanças de contrato devem ser registradas na issue ou no pull request correspondente.

## Organização sugerida

```text
.
├── README.md
├── docs/
│   ├── PLANO-DE-TRABALHO.md
│   ├── TESTES.md
│   ├── Paradigmas de Programação - AP1 - Testes para o projeto 3.md
│   └── apostila-linguagens-imperativas.pdf
└── src/
    ├── main.c
    ├── playlist.c
    └── playlist.h
```

## Fluxo obrigatório de contribuição

É proibido fazer push direto na `main`. Toda alteração, inclusive documentação, deve seguir este fluxo:

1. atualize a `main` local e crie uma branch própria;
2. faça as alterações e commits na branch;
3. publique a branch no GitHub;
4. abra uma pull request (PR) para `main`;
5. peça revisão de pelo menos uma pessoa, quando possível;
6. confira os testes e faça o merge pela PR;
7. mantenha a branch para preservar o histórico da tarefa e facilitar consultas ou revert futuros.

Exemplo:

```bash
git switch main
git pull
git switch -c nome-da-tarefa
# trabalhar, testar e fazer commits
git push -u origin nome-da-tarefa
```

Na PR, descreva o objetivo, os arquivos alterados, como testar e possíveis riscos. Se algo quebrar depois do merge, a equipe deve identificar o commit/PR responsável e usar o revert da própria PR para restaurar a versão anterior.

O histórico de branches, PRs e merges deve ser preservado no GitHub para facilitar auditoria e revert de qualquer alteração problemática.

Sugestão de commits: `docs: registra regras da playlist`, `feat: adiciona musica`, `test: cobre limite da playlist`.

## Critérios de pronto

- O comportamento dos dez testes de [`docs/TESTES.md`](docs/TESTES.md) foi verificado.
- Entradas inválidas não encerram o programa de forma inesperada.
- Não há acesso fora dos limites do vetor.
- Falhas de alocação são tratadas.
- Não há `double free`, `use-after-free` ou vazamento conhecido.
- As funções têm responsabilidades claras e nomes compreensíveis.
- A apresentação consegue explicar as decisões de estado, memória e navegação.

## Compilação e execução

Execute os comandos a partir da raiz do projeto, onde ficam `README.md`, `docs/` e `src/`.

No Windows PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -g src/main.c src/playlist.c -o player.exe
.\player.exe
```

Em Linux/macOS ou GitHub Codespaces:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g src/main.c src/playlist.c -o player
./player
```

Se o comando `gcc` não for reconhecido no Windows, é necessário instalar ou configurar um compilador C.
