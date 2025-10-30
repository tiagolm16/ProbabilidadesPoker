#include <stdio.h>
#include "baralho.h"

int main() {
    Mao jogador;
    Mesa mesa = {.numCartas = 0};
    Carta baralho[52];

    printf("=== Calculadora de Probabilidades de Poker ===\n\n");

    // Inicializa o baralho uma única vez
    inicializarBaralho(baralho);

    // Leitura da mão
    lerMao(&jogador);
    imprimirMao(jogador);

    printf("\nAgora, vá revelando as cartas da mesa (Flop, Turn, River):\n");

    // Loop progressivo da mesa
    for (int i = 0; i < 5; i++) {
        printf("\nCarta %d da mesa:\n", i + 1);
        mesa.cartas[mesa.numCartas] = lerCarta();
        mesa.numCartas++;

        imprimirMesa(mesa);

        int valor = avaliarMao(jogador, mesa);
        printf("Classificação atual: %s\n", nomeDaMao(valor));

        // ✅ Define automaticamente o número de simulações com base na etapa
        int simulacoes;
        if (mesa.numCartas <= 0)
            simulacoes = 2000;     // pré-flop
        else if (mesa.numCartas == 3)
            simulacoes = 5000;     // flop
        else if (mesa.numCartas == 4)
            simulacoes = 10000;    // turn
        else
            simulacoes = 15000;    // river

        printf("(Simulando %d rodadas para análise...)\n", simulacoes);
        simularProbabilidade(jogador, mesa, simulacoes, baralho);

        if (mesa.numCartas == 5) {
            printf("\n--- Mão completa! ---\n");
            break;
        }

        char opcao;
        printf("\nDeseja revelar mais uma carta? (s/n): ");
        scanf(" %c", &opcao);
        if (opcao != 's' && opcao != 'S') break;
    }

    printf("\n=== Fim da simulação ===\n");
    return 0;
}
