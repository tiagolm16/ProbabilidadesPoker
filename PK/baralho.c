#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "baralho.h"

// ==========================
// Funções de leitura e impressão
// ==========================
void imprimirCarta(Carta c) {
    char *nomeValor;
    switch (c.valor) {
        case 11: nomeValor = "J"; break;
        case 12: nomeValor = "Q"; break;
        case 13: nomeValor = "K"; break;
        case 14: nomeValor = "A"; break;
        default:
            printf("%d%c", c.valor, c.naipe);
            return;
    }
    printf("%s%c", nomeValor, c.naipe);
}

void imprimirMao(Mao m) {
    printf("Sua mão: ");
    for (int i = 0; i < 2; i++) {
        imprimirCarta(m.cartas[i]);
        printf(" ");
    }
    printf("\n");
}

Carta lerCarta() {
    Carta c;
    char valorEntrada[3];
    printf("Digite a carta (ex: A C, 10 O, J P): ");
    scanf("%s %c", valorEntrada, &c.naipe);

    if (valorEntrada[0] == 'A')
        c.valor = 14;
    else if (valorEntrada[0] == 'K')
        c.valor = 13;
    else if (valorEntrada[0] == 'Q')
        c.valor = 12;
    else if (valorEntrada[0] == 'J')
        c.valor = 11;
    else
        c.valor = atoi(valorEntrada);

    return c;
}

void lerMao(Mao *m) {
    printf("Digite suas duas cartas:\n");
    for (int i = 0; i < 2; i++) {
        m->cartas[i] = lerCarta();
    }
}

void lerMesa(Mesa *m) {
    printf("Digite as cartas da mesa (até 5 no total):\n");
    for (int i = 0; i < 5; i++) {
        printf("Carta %d: ", i + 1);
        m->cartas[i] = lerCarta();
        m->numCartas++;

        printf("Mesa atual: ");
        for (int j = 0; j <= i; j++) {
            imprimirCarta(m->cartas[j]);
            printf(" ");
        }
        printf("\n");

        if (i < 4) {
            char opcao;
            printf("Deseja adicionar mais uma carta à mesa? (s/n): ");
            scanf(" %c", &opcao);
            if (opcao != 's' && opcao != 'S')
                break;
        }
    }
}

void imprimirMesa(Mesa m) {
    printf("Mesa: ");
    for (int i = 0; i < m.numCartas; i++) {
        imprimirCarta(m.cartas[i]);
        printf(" ");
    }
    printf("\n");
}

// ==========================
// Avaliação de mão
// ==========================
int avaliarMao(Mao jogador, Mesa mesa) {
    int contagemValor[15] = {0};
    int contagemNaipe[4] = {0};
    char naipes[] = {'C', 'O', 'P', 'E'};

    for (int i = 0; i < 2; i++) {
        contagemValor[jogador.cartas[i].valor]++;
        for (int n = 0; n < 4; n++)
            if (jogador.cartas[i].naipe == naipes[n])
                contagemNaipe[n]++;
    }

    for (int i = 0; i < mesa.numCartas; i++) {
        contagemValor[mesa.cartas[i].valor]++;
        for (int n = 0; n < 4; n++)
            if (mesa.cartas[i].naipe == naipes[n])
                contagemNaipe[n]++;
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

// ==========================
// Simulação Monte Carlo otimizada
// ==========================
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

int simularProbabilidade(Mao jogador, Mesa mesa, int simulacoes, Carta baralho[52]) {
    srand(time(NULL));
    int vitorias = 0, derrotas = 0, empates = 0;

    for (int s = 0; s < simulacoes; s++) {
        // Cópia do baralho base
        Carta tempBaralho[52];
        for (int i = 0; i < 52; i++) tempBaralho[i] = baralho[i];
        int totalCartas = 52;

        // Remove cartas conhecidas
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

        // Sorteia 2 cartas para o oponente
        Mao oponente;
        for (int i = 0; i < 2; i++) {
            int r = rand() % totalCartas;
            oponente.cartas[i] = tempBaralho[r];
            tempBaralho[r] = tempBaralho[--totalCartas];
        }

        // Completa a mesa (se necessário)
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
    }

    printf("\n=== RESULTADO DAS SIMULAÇÕES ===\n");
    printf("Simulações: %d\n", simulacoes);
    printf("Vitórias: %.2f%%\n", 100.0 * vitorias / simulacoes);
    printf("Empates: %.2f%%\n", 100.0 * empates / simulacoes);
    printf("Derrotas: %.2f%%\n", 100.0 * derrotas / simulacoes);

    return 0;
}
