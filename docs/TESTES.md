# Plano de testes

Os testes abaixo são a referência de comportamento para o player. Pessoa 4 deve marcar cada item como `PASSOU` ou `FALHOU` e anexar a saída observada no pull request.

| ID | Cenário | Resultado esperado |
|---|---|---|
| T1 | Início vazio | ponteiro `NULL`, total `0`, índice `-1`; exibição e listagem informam playlist vazia |
| T2 | Adicionar primeira música | vetor com capacidade 1; total `1`; índice atual passa a `0` |
| T3 | Adicionar segunda música | novo vetor é alocado; registro anterior é copiado; vetor antigo é liberado |
| T4 | Próxima música | índice avança e a segunda música é exibida |
| T5 | Avançar além da última | índice não muda e o programa informa que já está na última |
| T6 | Música anterior | índice retrocede e a música anterior é exibida |
| T7 | Retroceder antes da primeira | índice não muda e o programa informa que já está na primeira |
| T8 | Terceira música e listagem | as três músicas aparecem; `>` indica a música atual |
| T9 | Exibir música atual | título, artista, álbum, ano e posição são exibidos |
| T10 | Fluxo completo e saída | navegação, quarta inserção e listagem funcionam; toda a memória é liberada ao sair |

## Casos adicionais de qualidade

- selecionar uma opção inexistente no menu;
- informar texto vazio;
- informar texto maior que o campo;
- informar letras quando o programa espera um número;
- usar ano fora do intervalo definido pela equipe;
- tentar navegar em playlist vazia;
- simular falha de alocação, se houver mecanismo de teste;
- executar a versão final com sanitizador ou ferramenta equivalente disponível no ambiente.

## Registro de execução

Copie este modelo para cada rodada:

```text
Data:
Versão/commit:
Responsável:

T1: PASSOU / FALHOU — observação:
T2: PASSOU / FALHOU — observação:
...
T10: PASSOU / FALHOU — observação:

Casos adicionais:
Falhas abertas:
```
