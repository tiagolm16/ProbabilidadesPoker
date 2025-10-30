#ifndef BARALHO_H
#define BARALHO_H

typedef struct {
    int valor;      // 2–14 (11=J, 12=Q, 13=K, 14=A)
    char naipe;     // 'C' = Copas, 'O' = Ouros, 'P' = Paus, 'E' = Espadas
} Carta;

typedef struct {
    Carta cartas[2];
} Mao;

typedef struct {
    Carta cartas[5];
    int numCartas; // 0 a 5 (flop, turn, river)
} Mesa;

// Funções gerais
void imprimirCarta(Carta c);
void imprimirMao(Mao m);
Carta lerCarta();
void lerMao(Mao *m);
void lerMesa(Mesa *m);
void imprimirMesa(Mesa m);

// Avaliação
int avaliarMao(Mao jogador, Mesa mesa);
const char* nomeDaMao(int valor);

// Simulação
void inicializarBaralho(Carta baralho[52]);
int compararMaos(Mao jogador1, Mao jogador2, Mesa mesa);
int simularProbabilidade(Mao jogador, Mesa mesa, int simulacoes, Carta baralho[52]);

#endif
