# Player de Músicas — Paradigmas de Programação

Projeto integrador da disciplina Paradigmas de Programação, desenvolvido em C para praticar o paradigma imperativo, structs, vetores, funções, ponteiros e alocação dinâmica de memória.

Integrantes do Projeto:

Daniel Fernandes Santos 

Davi Patricio Gimenes 

Ramon Barbosa de Sousa

Welson Ferreira da Cruz 
## Objetivo

Construir uma aplicação de linha de comando que gerencie uma playlist de músicas. O programa permite:

- adicionar músicas;
- avançar e voltar na playlist;
- exibir a música atual;
- listar a playlist com indicação da música atual;
- buscar músicas por trecho de título ou artista (extensão);
- validar entradas e tratar erros;
- liberar toda a memória alocada ao encerrar.

A busca exata por título e a ordenação por título existem no núcleo, sem opção no menu. Detalhes em [`docs/REQUISITOS.md`](docs/REQUISITOS.md).

## Escopo inicial

Cada música possui `titulo`, `artista`, `album` e `ano`. A playlist começa vazia, com ponteiro `NULL`, total igual a zero e índice atual igual a `-1`. Cada nova inserção cria um vetor maior, copia os registros existentes e libera o vetor anterior.

## Como usar o programa

Ao iniciar, a playlist está vazia e o menu aparece em laço até a opção Sair:

```text
===== Player de Músicas =====
1. Adicionar música
2. Próxima música
3. Música anterior
4. Exibir música atual
5. Listar playlist
6. Buscar música
7. Sair
Escolha uma opção:
```

### Navegação passo a passo

1. **Adicionar (1):** informe título, artista e álbum (até 99 bytes, sem vazio) e ano (1900–2100). A primeira música vira a atual automaticamente.
2. **Próxima (2) / Anterior (3):** movem a música atual uma posição. Nos limites, o programa avisa ("já está na última/primeira") sem mudar nada.
3. **Exibir atual (4):** mostra título, artista, álbum, ano e posição (`1 de N`). Em playlist vazia, informa que não há músicas.
4. **Listar (5):** mostra todas numeradas a partir de 1; `>` marca a atual.
5. **Buscar (6):** digite um trecho de título ou artista (`queen` acha `Queen`); resultados saem como `[1] Título | Artista`. Um resultado vira atual sozinho; com vários, digite o número (0 cancela).
6. **Sair (7):** libera a memória e encerra.

### Dicas de usabilidade

- Entradas inválidas (texto vazio, letras onde vai número, ano fora da faixa) apenas mostram mensagem e pedem de novo — o programa não trava nem perde a playlist.
- Textos longos demais são recusados com o limite; acentos contam como mais de um byte. O programa usa locale UTF-8 (`setlocale(LC_ALL, "pt_BR.UTF-8")`), então terminais em UTF-8 renderem `Música`, `índice`, `Álbum` corretamente.
- Na busca, refine o termo se aparecer aviso de 64 resultados; `Joao` não encontra `João`.
- Casos de uso em [`docs/REQUISITOS.md`](docs/REQUISITOS.md); contratos das funções em [`docs/GLOSSARIO.md`](docs/GLOSSARIO.md); roteiro da apresentação em [`docs/APRESENTACAO.md`](docs/APRESENTACAO.md).

## Documentação

| Documento | Conteúdo |
|---|---|
| [`docs/REQUISITOS.md`](docs/REQUISITOS.md) | Requisitos, casos de uso e matriz requisito–teste |
| [`docs/GLOSSARIO.md`](docs/GLOSSARIO.md) | Conceitos, contratos das funções e paradigma imperativo |
| [`docs/ARQUITETURA.md`](docs/ARQUITETURA.md) | Camadas, estado e estratégia de memória |
| [`docs/TESTES.md`](docs/TESTES.md) | Bateria T1–T10 e casos de qualidade |
| [`docs/GUIA-DA-EQUIPE.md`](docs/GUIA-DA-EQUIPE.md) | Passo a passo de cada integrante e fluxo de contribuição |
| [`docs/IMPLEMENTAR_FEATURE.md`](docs/IMPLEMENTAR_FEATURE.md) | Guia genérico para adicionar features ao player |
| [`docs/APRESENTACAO.md`](docs/APRESENTACAO.md) | Roteiro da apresentação (5 min, script falado) |
| [`docs/Paradigmas de Programação - AP1 - Testes para o projeto 3.md`](docs/Paradigmas%20de%20Programação%20-%20AP1%20-%20Testes%20para%20o%20projeto%203.md) | Enunciado original com exemplos de comportamento |

## Divisão do trabalho

| Pessoa | Responsabilidade principal | Entregas |
|---|---|---|
| **Pessoa 1** | Implementação técnica e integração | modelagem em C, vetor dinâmico, `malloc`/`free`, navegação, busca/ordenação, integração final e correção de problemas de memória |
| **Pessoa 2** | Requisitos e documentação | regras do sistema, casos de uso, decisões de projeto, organização do README e roteiro da apresentação |
| **Pessoa 3** | Menu e experiência de uso | fluxo do menu, mensagens exibidas, leitura segura de strings/números e validação das entradas |
| **Pessoa 4** | Testes e qualidade | execução dos testes, casos de fronteira, registro de resultados, revisão dos critérios e apoio à apresentação |

## Estrutura do repositório

```text
.
├── README.md
├── docs/
│   ├── PLANO-DE-TRABALHO.md
│   ├── REQUISITOS.md
│   ├── GLOSSARIO.md
│   ├── ARQUITETURA.md
│   ├── IMPLEMENTAR_FEATURE.md
│   ├── APRESENTACAO.md
│   ├── TESTES.md
│   ├── Paradigmas de Programação - AP1 - Testes para o projeto 3.md
│   └── apostila-linguagens-imperativas.pdf
├── src/
│   ├── main.c
│   ├── playlist.c
│   ├── playlist.h
│   ├── io.c
│   └── io.h
└── tests/
    ├── test_playlist_add.c
    ├── test_playlist_display.c
    ├── test_playlist_navigation.c
    ├── test_playlist_search_sort.c
    └── test_fronteira_texto.c
```

## Fluxo de contribuição

É proibido fazer push direto na `main`. Atualize a `main` local, crie uma branch, publique e abra uma PR com objetivo, arquivos alterados, como testar e riscos. Detalhes em [`docs/GUIA-DA-EQUIPE.md`](docs/GUIA-DA-EQUIPE.md).

## Critérios de pronto

- O comportamento dos dez testes de [`docs/TESTES.md`](docs/TESTES.md) foi verificado.
- Entradas inválidas não encerram o programa de forma inesperada.
- Não há acesso fora dos limites do vetor.
- Falhas de alocação são tratadas.
- Não há `double free`, `use-after-free` ou vazamento conhecido.
- As funções têm responsabilidades claras e nomes compreensíveis.
- A apresentação consegue explicar as decisões de estado, memória e navegação.

## Compilação e execução

Pré-requisito: um compilador C com suporte a C11. No Windows, uma opção leve é o WinLibs (GCC/MinGW-w64), instalável sem Visual Studio:

```powershell
winget install --id BrechtSanders.WinLibs.POSIX.UCRT --exact --source winget --scope user
```

Reabra o terminal após instalar para atualizar o `PATH`.

Execute os comandos a partir da raiz do projeto, onde ficam `README.md`, `docs/` e `src/`.

No Windows PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -g src/main.c src/playlist.c src/io.c -o player.exe
.\player.exe
```

Em Linux/macOS ou GitHub Codespaces:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g src/main.c src/playlist.c src/io.c -o player
./player
```

### Testes

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -g -I src tests/test_playlist_add.c src/playlist.c -o test_add.exe
gcc -std=c11 -Wall -Wextra -Wpedantic -g -I src tests/test_playlist_navigation.c src/playlist.c -o test_nav.exe
gcc -std=c11 -Wall -Wextra -Wpedantic -g -I src tests/test_playlist_display.c src/playlist.c -o test_display.exe
gcc -std=c11 -Wall -Wextra -Wpedantic -g -I src tests/test_playlist_search_sort.c src/playlist.c -o test_search.exe
gcc -std=c11 -Wall -Wextra -Wpedantic -g -I src tests/test_fronteira_texto.c -o test_fronteira.exe
.\test_add.exe; .\test_nav.exe; .\test_display.exe; .\test_search.exe; .\test_fronteira.exe
```

Todos devem terminar sem erro (o teste de fronteira imprime `fronteira: PASSOU`). Em Linux/macOS, troque a extensão e use `./` no lugar de `.\`.
