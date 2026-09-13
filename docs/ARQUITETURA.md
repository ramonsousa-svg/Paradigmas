
# Arquitetura

## Camadas

- **Núcleo** (`playlist.c/.h`): vetor dinâmico, navegação, busca e ordenação. Sem impressão: nada de `printf` aqui.
- **Entrada/saída** (`io.c/.h`): leitura segura de texto e números, compartilhada.
- **Interface** (`main.c`): menu, mensagens, exibição e listagem formatada.
- **Testes** (`tests/`): verificação de comportamento do núcleo + fronteira via `player` real.
- **Build/CI**: `Makefile` (Linux/macOS), `scripts/build.ps1` (Windows), `.github/workflows/ci.yml` (matriz de SO + sanitizadores).

## Estado

- `Playlist` contém vetor `Musica*`, `total_musicas` e `indice_atual`.
- Playlist vazia: vetor `NULL`, total `0`, índice `-1`.
- Primeira música adicionada vira a atual (índice `0`).

## Memória

- Inserção: aloca vetor maior, copia, adiciona, libera anterior.
- Encerramento: libera vetor e reseta estado.

## Extensões recentes

- Busca parcial em título e artista (case-insensitive), com limite de 64 resultados e aviso quando truncada.
- `playlist_definir_atual`: altera música atual por posição.
- Menu com 7 opções: 6 do enunciado + Buscar (6); Sair por último (7).
- Entrada de texto aceita até 99 bytes ASCII (capacidade de `char[100]`); caracteres multibyte podem reduzir o limite.
