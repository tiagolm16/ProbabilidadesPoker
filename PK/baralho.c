#include <stdlib.h>
#include "baralho.h"

static int indiceNaipe(char naipe) {
    switch (naipe) {
        case 'C': return 0;
        case 'O': return 1;
        case 'P': return 2;
        case 'E': return 3;
        default: return -1;
    }
}

int avaliarMao(Mao jogador, Mesa mesa) {
    int contagemValor[15] = {0};
    int contagemNaipe[4] = {0};

    for (int i = 0; i < 2; i++) {
        contagemValor[jogador.cartas[i].valor]++;
        int idx = indiceNaipe(jogador.cartas[i].naipe);
        if (idx >= 0) contagemNaipe[idx]++;
    }

    for (int i = 0; i < mesa.numCartas; i++) {
        contagemValor[mesa.cartas[i].valor]++;
        int idx = indiceNaipe(mesa.cartas[i].naipe);
        if (idx >= 0) contagemNaipe[idx]++;
    }

    int pares = 0, trincas = 0, quadra = 0;
    for (int v = 2; v <= 14; v++) {
        if (contagemValor[v] == 2) pares++;
        if (contagemValor[v] == 3) trincas++;
        if (contagemValor[v] == 4) quadra++;
    }

    int temFlush = 0;
    for (int n = 0; n < 4; n++)
        if (contagemNaipe[n] >= 5)
            temFlush = 1;

    int sequencia = 0, consecutivas = 0;
    for (int v = 2; v <= 14; v++) {
        if (contagemValor[v] > 0) consecutivas++;
        else consecutivas = 0;
        if (consecutivas >= 5) sequencia = 1;
    }

    if (!sequencia && contagemValor[14] && contagemValor[2] &&
        contagemValor[3] && contagemValor[4] && contagemValor[5])
        sequencia = 1;

    int fullHouse = (trincas >= 1 && pares >= 1);

    if (sequencia && temFlush) return 9;
    else if (quadra) return 8;
    else if (fullHouse) return 7;
    else if (temFlush) return 6;
    else if (sequencia) return 5;
    else if (trincas) return 4;
    else if (pares == 2) return 3;
    else if (pares == 1) return 2;
    else return 1;
}

const char* nomeDaMao(int valor) {
    switch (valor) {
        case 1: return "Carta Alta";
        case 2: return "Par";
        case 3: return "Dois Pares";
        case 4: return "Trinca";
        case 5: return "Sequência";
        case 6: return "Flush";
        case 7: return "Full House";
        case 8: return "Quadra";
        case 9: return "Straight Flush";
        default: return "Desconhecido";
    }
}

void inicializarBaralho(Carta baralho[52]) {
    char naipes[] = {'C', 'O', 'P', 'E'};
    int index = 0;
    for (int n = 0; n < 4; n++) {
        for (int v = 2; v <= 14; v++) {
            baralho[index].valor = v;
            baralho[index].naipe = naipes[n];
            index++;
        }
    }
}

int compararMaos(Mao jogador1, Mao jogador2, Mesa mesa) {
    int forca1 = avaliarMao(jogador1, mesa);
    int forca2 = avaliarMao(jogador2, mesa);
    if (forca1 > forca2) return 1;
    else if (forca2 > forca1) return -1;
    else return 0;
}

Probabilidades simularProbabilidade(Mao jogador, Mesa mesa, int simulacoes,
                                    Carta baralho[52],
                                    ProgressoCallback callback) {
    int vitorias = 0, derrotas = 0, empates = 0;

    int passo = simulacoes / 100;
    if (passo < 1) passo = 1;

    if (callback) callback(0, simulacoes);

    for (int s = 0; s < simulacoes; s++) {
        Carta tempBaralho[52];
        int totalCartas = 52;
        for (int i = 0; i < 52; i++) tempBaralho[i] = baralho[i];

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < totalCartas; j++) {
                if (tempBaralho[j].valor == jogador.cartas[i].valor &&
                    tempBaralho[j].naipe == jogador.cartas[i].naipe) {
                    tempBaralho[j] = tempBaralho[--totalCartas];
                    break;
                }
            }
        }

        for (int i = 0; i < mesa.numCartas; i++) {
            for (int j = 0; j < totalCartas; j++) {
                if (tempBaralho[j].valor == mesa.cartas[i].valor &&
                    tempBaralho[j].naipe == mesa.cartas[i].naipe) {
                    tempBaralho[j] = tempBaralho[--totalCartas];
                    break;
                }
            }
        }

        Mao oponente;
        for (int i = 0; i < 2; i++) {
            int r = rand() % totalCartas;
            oponente.cartas[i] = tempBaralho[r];
            tempBaralho[r] = tempBaralho[--totalCartas];
        }

        Mesa mesaCompleta = mesa;
        for (int i = mesa.numCartas; i < 5; i++) {
            int r = rand() % totalCartas;
            mesaCompleta.cartas[i] = tempBaralho[r];
            tempBaralho[r] = tempBaralho[--totalCartas];
            mesaCompleta.numCartas++;
        }

        int resultado = compararMaos(jogador, oponente, mesaCompleta);
        if (resultado == 1) vitorias++;
        else if (resultado == -1) derrotas++;
        else empates++;

        if (callback && ((s + 1) % passo == 0 || s + 1 == simulacoes))
            callback(s + 1, simulacoes);
    }

    Probabilidades p;
    p.vitoria = 100.0 * vitorias / simulacoes;
    p.empate = 100.0 * empates / simulacoes;
    p.derrota = 100.0 * derrotas / simulacoes;
    return p;
}
