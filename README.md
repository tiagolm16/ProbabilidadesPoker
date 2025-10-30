# ProbabilidadesPoker 🎲

Oi! Eu sou um estudante de Engenharia de Software de 18 anos e esse é meu primeiro
projeto "sério" de pôquer com probabilidade real. A ideia foi juntar duas coisas
que eu curto muito: cartas e programação. Com ele, dá pra ver como as chances de
vitória mudam conforme as cartas aparecem na mesa.

## O que o simulador faz?
- Você digita suas duas cartas iniciais (a famosa mão inicial de Texas Hold'em).
- Depois vai revelando as cartas da mesa no seu ritmo.
- O programa avalia a força da sua mão (par, sequência, flush...) a cada nova
  carta.
- Enquanto isso, ele roda milhares de simulações aleatórias para descobrir a
  chance real de ganhar, empatar ou perder naquele momento.

Tudo acontece direto no terminal usando `ncurses`, com cores diferentes para a
mão, para a mesa e para as probabilidades. Também rola uma barra de progresso
pra mostrar que as simulações Monte Carlo estão trabalhando por você.

## Mas o que é simulação Monte Carlo?
É um nome chique para "testar várias possibilidades aleatórias e ver o que
acontece". No nosso caso, o programa embaralha as cartas que ainda não foram
reveladas e joga várias partidas simuladas. Contando quantas vezes você ganha,
empata ou perde, dá pra estimar as probabilidades reais da vida.

## Como compilar e rodar
```bash
cd PK
gcc main.c baralho.c interface.c -lncurses -o poker
./poker
```

> Dica: se o `gcc` reclamar do `ncurses`, instale a biblioteca (em sistemas
> baseados em Debian/Ubuntu é `sudo apt-get install libncurses5-dev`).

## Dicas de uso
- Informe as cartas no formato `valor naipe`, por exemplo: `A C`, `10 O`, `7 P`.
- Os naipes seguem a notação em português: `C` (copas ♥), `O` (ouros ♦), `P`
  (paus ♣) e `E` (espadas ♠).
- Depois de cada rodada de simulação dá pra decidir se você quer revelar a
  próxima carta da mesa ou encerrar por ali.

## Por quê fazer isso?
Porque é divertido ver matemática e programação funcionando em tempo real! Esse
projeto me ajudou a entender melhor probabilidade, lógica e também a mexer com
interfaces de terminal. Espero que ele também te inspire a experimentar novas
ideias. :)
