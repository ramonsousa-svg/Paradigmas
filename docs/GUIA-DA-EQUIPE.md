# Guia passo a passo da equipe

Este documento transforma o plano geral em tarefas executáveis. Cada pessoa deve concluir seus itens, registrar dúvidas e entregar uma PR. Uma tarefa só termina quando o critério de conclusão foi atendido.

## Regras para todos

Antes de começar:

1. leia o README e este guia;
2. confira no quadro qual é a sua frente;
3. atualize a `main` local;
4. crie uma branch com o nome sugerido;
5. faça commits pequenos;
6. abra uma PR com descrição e evidências de teste.

Comandos iniciais:

```bash
git switch main
git pull
git switch -c nome-da-branch
```

Ao terminar:

```bash
git add arquivos-alterados
git commit -m "tipo: descreve a mudança"
git push -u origin nome-da-branch
```

Na PR, informe: objetivo, arquivos alterados, como testar, resultado obtido e qualquer pendência.

## Ordem de execução

O trabalho deve seguir esta ordem para reduzir bloqueios:

1. Pessoa 2 confirma as regras e os exemplos.
2. Pessoa 1 cria a estrutura técnica mínima.
3. Pessoa 3 implementa o menu e as entradas usando essa estrutura.
4. Pessoa 4 executa os testes e registra falhas.
5. Pessoa 1 integra as correções e prepara a versão final.
6. Todo o time revisa a apresentação e o histórico de PRs.

---

## Pessoa 1 — núcleo técnico e integração

### Branch sugerida

`feat/nucleo-playlist`

### Passos

1. Criar `src/playlist.h` e `src/playlist.c`.
2. Declarar `Musica` com `titulo`, `artista`, `album` e `ano`.
3. Criar o estado da playlist com vetor, quantidade de músicas e índice atual.
4. Inicializar o estado como vetor `NULL`, total `0` e índice `-1`.
5. Implementar a inserção: alocar vetor maior, copiar registros, adicionar a música, liberar o vetor anterior e atualizar o total.
6. Ao inserir a primeira música, definir o índice atual como `0`.
7. Implementar próxima e anterior, impedindo índice menor que `0` ou maior que `total - 1`.
8. Implementar listagem e exibição da música atual.
9. Implementar busca e ordenação quando o contrato da Pessoa 2 estiver definido.
10. Implementar a função de encerramento que libera o vetor e deixa o ponteiro como `NULL`.
11. Revisar todos os retornos de `malloc`, limites e caminhos de erro.

### Critério de conclusão

Os testes T1 a T10 passam com dados fixos, as funções não dependem do menu e não há vazamento ou acesso fora dos limites conhecido.

### PR

Título sugerido: `feat: implementa nucleo da playlist`.

---

## Pessoa 2 — requisitos e apresentação

### Branch sugerida

`docs/requisitos-apresentacao`

### Passos

1. Ler a apostila e o arquivo de testes.
2. Listar as funções obrigatórias e separar as opcionais.
3. Confirmar com o time o significado de playlist vazia, primeira música, última música e entrada inválida.
4. Criar uma tabela com campos da música, tipo, limite e exemplo.
5. Definir o comportamento de busca e ordenação antes da implementação.
6. Registrar as decisões no `PLANO-DE-TRABALHO.md`.
7. Montar um roteiro curto: paradigma imperativo, estado, struct, ponteiros, memória e testes.
8. Revisar se o README ensina uma pessoa nova a iniciar o projeto.

### Critério de conclusão

Não existe requisito ambíguo para as funções principais; cada requisito possui pelo menos um teste correspondente; o roteiro da apresentação cabe em uma demonstração do programa e da documentação.

### PR

Título sugerido: `docs: consolida requisitos e roteiro da apresentacao`.

---

## Pessoa 3 — menu e validação de entradas

### Branch sugerida

`feat/menu-validacao`

### Passos

1. Criar `src/main.c` e exibir o menu de seis opções.
2. Implementar um laço que continue até a opção de saída.
3. Conectar a opção de adicionar à função da playlist.
4. Criar leitura segura para título, artista e álbum, respeitando os limites dos campos.
5. Criar leitura segura para ano e opção do menu.
6. Tratar entrada vazia, texto no lugar de número, opção inexistente e fim inesperado da entrada.
7. Conectar próxima, anterior, exibição atual e listagem.
8. Exibir mensagens claras para playlist vazia e tentativa de ultrapassar os limites.
9. Testar com entradas válidas e inválidas antes de abrir a PR.

### Critério de conclusão

O programa não trava nem corrompe strings diante dos casos adicionais de `TESTES.md`; o menu corresponde às seis opções do enunciado mais Buscar (Sair fica por último); o `main` apenas coordena o fluxo.

### PR

Título sugerido: `feat: adiciona menu e validacao de entradas`.

---

## Pessoa 4 — testes e qualidade

### Branch sugerida

`test/bateria-player`

### Passos

1. Conferir se o programa compila com `-Wall -Wextra -Wpedantic`.
2. Executar T1 e T2 para validar o estado inicial e a primeira inserção.
3. Executar T3, T4 e T5 para validar cópia, navegação para frente e limite superior.
4. Executar T6 e T7 para validar navegação para trás e limite inferior.
5. Executar T8 e T9 para validar listagem e detalhes da música atual.
6. Executar T10 para validar o fluxo completo e a liberação final.
7. Executar os casos adicionais de entradas inválidas e playlist vazia.
8. Registrar tudo em `docs/TESTES.md`, incluindo commit, data, resultado e observações.
9. Para cada falha, abrir uma issue com passos para reproduzir e saída esperada.
10. Reexecutar os testes depois das correções e atualizar o registro.

### Critério de conclusão

Todos os testes obrigatórios possuem resultado registrado; falhas têm issue ou foram corrigidas; a versão final não apresenta erro conhecido de limite, entrada ou memória.

### PR

Título sugerido: `test: registra bateria do player`.

---

## Checklist final do time

- [ ] Requisitos confirmados pela Pessoa 2.
- [ ] Núcleo da playlist integrado.
- [ ] Menu e validações funcionando.
- [ ] Busca e ordenação definidas e implementadas, se exigidas pelo professor.
- [ ] Testes T1 a T10 registrados.
- [ ] Casos inválidos testados.
- [ ] Memória liberada no encerramento.
- [ ] README e documentação revisados.
- [ ] PRs mergeadas na `main`.
- [ ] Demonstração e apresentação ensaiadas.
