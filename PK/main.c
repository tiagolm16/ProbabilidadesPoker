#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "baralho.h"
#include "interface.h"

static int definir_simulacoes(int cartasMesa) {
    if (cartasMesa == 0) return 2000;      // pré-flop
    if (cartasMesa == 3) return 5000;      // flop
    if (cartasMesa == 4) return 10000;     // turn
    if (cartasMesa == 5) return 15000;     // river
    return 4000;                           // etapas intermediárias
}

int main(void) {
    srand((unsigned)time(NULL));

    Carta baralho[52];
    inicializarBaralho(baralho);

    Mao jogador = {0};
    Mesa mesa = {0};

    interface_inicializar();
    interface_desenhar_layout();
    interface_mostrar_mensagem("Vamos cadastrar sua mão inicial!");

    for (int i = 0; i < 2; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Carta %d da sua mão:", i + 1);
        jogador.cartas[i] = interface_solicitar_carta(prompt);
    }

    interface_atualizar_mao(jogador);
    interface_atualizar_mesa(mesa);

    interface_mostrar_classificacao(nomeDaMao(avaliarMao(jogador, mesa)));
    int simulacoes = definir_simulacoes(mesa.numCartas);
    interface_mostrar_simulacoes(simulacoes);
    interface_iniciar_progresso(simulacoes);
    Probabilidades probabilidades = simularProbabilidade(
        jogador, mesa, simulacoes, baralho, interface_atualizar_progresso);
    interface_finalizar_progresso();
    interface_mostrar_probabilidades(probabilidades);

    while (mesa.numCartas < 5) {
        if (!interface_perguntar_continuar(
                "Revelar a próxima carta da mesa? (s/n)")) {
            break;
        }

        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Carta %d da mesa:", mesa.numCartas + 1);
        mesa.cartas[mesa.numCartas] = interface_solicitar_carta(prompt);
        mesa.numCartas++;

        interface_atualizar_mesa(mesa);
        interface_mostrar_classificacao(nomeDaMao(avaliarMao(jogador, mesa)));

        simulacoes = definir_simulacoes(mesa.numCartas);
        interface_mostrar_simulacoes(simulacoes);
        interface_iniciar_progresso(simulacoes);
        probabilidades = simularProbabilidade(
            jogador, mesa, simulacoes, baralho, interface_atualizar_progresso);
        interface_finalizar_progresso();
        interface_mostrar_probabilidades(probabilidades);

        if (mesa.numCartas == 5) {
            interface_mostrar_mensagem("Todas as cartas foram reveladas!");
            break;
        }
    }

    interface_esperar_tecla();
    interface_finalizar();
    return 0;
}
