# **Bateria de Testes** **Player de Músicas**

## **Observação sobre os conceitos informados**

Este projeto utiliza um **vetor de structs** para representar a playlist, com um **índice de música atual** que permite navegar para frente e para trás. Como não há limite de tamanho, a implementação assumida segue um padrão de crescimento manual: a cada nova música adicionada, aloca-se um **novo vetor maior**, copiam-se as músicas existentes, adiciona-se a nova música e libera-se o vetor anterior.

---

## **Premissas assumidas sobre a implementação**

typedef struct { char titulo\[100\]; char artista\[100\]; char album\[100\]; int ano; } Musica;

* O vetor da playlist (`Musica *playlist`) inicia como `NULL`, com `total_musicas = 0`.  
* A variável `indice_atual` inicia em `-1` (nenhuma música tocando, pois a playlist está vazia).  
* Ao adicionar a **primeira música**, `indice_atual` passa automaticamente a `0`.  
* Navegar **para frente** incrementa `indice_atual`; navegar **para trás** decrementa `indice_atual`, respeitando os limites `0` e `total_musicas - 1`.  
* Cada inserção realoca manualmente o vetor (novo `malloc`, cópia, `free` do vetor anterior).  
* Menu principal:

```
===== Player de Musicas =====
1. Adicionar musica
2. Proxima musica
3. Musica anterior
4. Exibir musica atual
5. Listar playlist
6. Sair
Escolha uma opcao:
```

---

**Teste 1 — Estado inicial vazio**

**Objetivo:** verificar se a playlist inicia vazia (`NULL`, `total = 0`, `indice_atual = -1`).

```
Playlist inicializada (ponteiro NULL, total = 0, indice atual = -1).

===== Player de Musicas =====
1. Adicionar musica
2. Proxima musica
3. Musica anterior
4. Exibir musica atual
5. Listar playlist
6. Sair
Escolha uma opcao: 4

Nenhuma musica na playlist.

Escolha uma opcao: 5

--- Playlist ---
Playlist vazia.
```

---

## 

## **Teste 2 — Adição da primeira música (alocação inicial)**

**Objetivo:** verificar se a primeira música é adicionada corretamente e definida como música atual.

```
Escolha uma opcao: 1

--- Adicionar Musica ---
Titulo: Bohemian Rhapsody
Artista: Queen
Album: A Night at the Opera
Ano: 1975

Alocando novo vetor (capacidade: 1)...
Musica adicionada com sucesso!
Total de musicas: 1
Musica atual definida automaticamente (indice 0).
```

---

## 

## **Teste 3 — Adição da segunda música**

**Objetivo:** verificar se a segunda inserção aloca um novo vetor maior, copia a música existente e libera o vetor anterior.

```
Escolha uma opcao: 1
Titulo: Hotel California
Artista: Eagles
Album: Hotel California
Ano: 1976

Alocando novo vetor (capacidade: 2)...
Copiando 1 musica(s) existente(s)...
Liberando vetor anterior...
Musica adicionada com sucesso!
Total de musicas: 2
```

---

## 

## **Teste 4 — Navegação para a próxima música**

**Objetivo:** verificar se o índice atual avança corretamente e exibe a nova música.

```
Escolha uma opcao: 2

--- Proxima Musica ---
Titulo: Hotel California
Artista: Eagles
Album: Hotel California
Ano: 1976
```

---

## 

## **Teste 5 — Tentativa de avançar além da última música**

**Objetivo:** verificar se o programa impede a navegação além do limite superior do vetor.

```
Escolha uma opcao: 2

Voce ja esta na ultima musica da playlist.
Musica atual permanece:
Titulo: Hotel California
Artista: Eagles
Album: Hotel California
Ano: 1976
```

---

## 

## **Teste 6 — Navegação para a música anterior**

**Objetivo:** verificar se o índice atual retrocede corretamente e exibe a música anterior.

```
Escolha uma opcao: 3

--- Musica Anterior ---
Titulo: Bohemian Rhapsody
Artista: Queen
Album: A Night at the Opera
Ano: 1975
```

---

## 

## **Teste 7 — Tentativa de retroceder além da primeira música**

**Objetivo:** verificar se o programa impede a navegação abaixo do limite inferior do vetor.

```
Escolha uma opcao: 3

Voce ja esta na primeira musica da playlist.
Musica atual permanece:
Titulo: Bohemian Rhapsody
Artista: Queen
Album: A Night at the Opera
Ano: 1975
```

---

## 

## **Teste 8 — Adição de uma terceira música e listagem completa com indicador da música atual**

**Objetivo:** verificar se a listagem exibe todas as músicas corretamente, destacando qual está tocando no momento.

```
Escolha uma opcao: 1
Titulo: Stairway to Heaven
Artista: Led Zeppelin
Album: Led Zeppelin IV
Ano: 1971

Alocando novo vetor (capacidade: 3)...
Copiando 2 musica(s) existente(s)...
Liberando vetor anterior...
Musica adicionada com sucesso!
Total de musicas: 3

Escolha uma opcao: 5

--- Playlist ---
> 1. Bohemian Rhapsody | Queen  | A Night at the Opera     | 1975
  2. Hotel California   | Eagles | Hotel California        | 1976
  3. Stairway to Heaven | Led Zeppelin | Led Zeppelin IV    | 1971

(> indica a musica atual)
```

---

## 

## **Teste 9 — Exibir informações completas da música atual**

**Objetivo:** verificar se a opção de exibir a música atual mostra corretamente todos os campos da struct.

```
Escolha uma opcao: 4

--- Musica Atual ---
Titulo: Bohemian Rhapsody
Artista: Queen
Album: A Night at the Opera
Ano: 1975
Posicao: 1 de 3
```

---

## 

## **Teste 10 — Fluxo completo: navegação até o final, adição de nova música e encerramento com liberação de memória**

**Objetivo:** validar a consistência da navegação após múltiplas operações e confirmar a liberação de toda a memória alocada ao encerrar o programa.

```
Escolha uma opcao: 2

--- Proxima Musica ---
Titulo: Hotel California
Artista: Eagles
Album: Hotel California
Ano: 1976

Escolha uma opcao: 2

--- Proxima Musica ---
Titulo: Stairway to Heaven
Artista: Led Zeppelin
Album: Led Zeppelin IV
Ano: 1971

Escolha uma opcao: 1
Titulo: Imagine
Artista: John Lennon
Album: Imagine
Ano: 1971

Alocando novo vetor (capacidade: 4)...
Copiando 3 musica(s) existente(s)...
Liberando vetor anterior...
Musica adicionada com sucesso!
Total de musicas: 4

Escolha uma opcao: 5

--- Playlist ---
  1. Bohemian Rhapsody   | Queen         | A Night at the Opera  | 1975
  2. Hotel California    | Eagles        | Hotel California      | 1976
> 3. Stairway to Heaven  | Led Zeppelin  | Led Zeppelin IV        | 1971
  4. Imagine             | John Lennon   | Imagine                | 1971

(> indica a musica atual)

Escolha uma opcao: 6

Liberando vetor da playlist...
Memoria liberada com sucesso.
Encerrando o programa.
```

---

