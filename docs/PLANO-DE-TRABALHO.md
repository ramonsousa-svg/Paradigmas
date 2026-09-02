# Plano de trabalho da equipe

## Resultado esperado

Entregar um player de músicas em C, com menu de terminal, playlist representada por vetor dinâmico de `Musica`, navegação com índice atual, validação, testes demonstráveis e liberação completa da memória.

## Quadro de atividades

### Você — núcleo técnico e integração

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

Bloqueios técnicos devem ser encaminhados a você; bloqueios de requisito devem ser registrados por Pessoa 2; falhas reproduzíveis devem ir para Pessoa 4.

## Definition of Done

Uma tarefa só é considerada concluída quando possui código ou documento entregue, instrução de teste, resultado registrado e revisão de outra pessoa.
