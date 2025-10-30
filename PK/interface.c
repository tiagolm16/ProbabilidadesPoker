#include <ctype.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

#define COLOR_HEADER 1
#define COLOR_HAND 2
#define COLOR_BOARD 3
#define COLOR_PROBS 4
#define COLOR_PROGRESS 5
#define COLOR_HEARTS 6
#define COLOR_DIAMONDS 7
#define COLOR_CLUBS 8
#define COLOR_SPADES 9

static const int MAO_Y = 6;
static const int MESA_Y = 9;
static const int INFO_Y = 12;
static const int PROGRESS_Y = 16;
static const int INPUT_Y = 20;
static const int INPUT_X = 2;

static void valor_para_string(int valor, char *dest) {
    switch (valor) {
        case 11: strcpy(dest, "J"); break;
        case 12: strcpy(dest, "Q"); break;
        case 13: strcpy(dest, "K"); break;
        case 14: strcpy(dest, "A"); break;
        default: snprintf(dest, 3, "%d", valor);
    }
}

static const char* simbolo_naipe(char naipe) {
    switch (naipe) {
        case 'C': return "♥";
        case 'O': return "♦";
        case 'P': return "♣";
        case 'E': return "♠";
        default: return "?";
    }
}

static short cor_naipe(char naipe) {
    switch (naipe) {
        case 'C': return COLOR_HEARTS;
        case 'O': return COLOR_DIAMONDS;
        case 'P': return COLOR_CLUBS;
        case 'E': return COLOR_SPADES;
        default: return 0;
    }
}

static void desenhar_carta(Carta carta, int y, int x) {
    char valor[3];
    valor_para_string(carta.valor, valor);
    const char *naipe = simbolo_naipe(carta.naipe);

    short cor = cor_naipe(carta.naipe);
    if (cor > 0) attron(COLOR_PAIR(cor) | A_BOLD);
    mvprintw(y, x, "[%2s%s]", valor, naipe);
    if (cor > 0) attroff(COLOR_PAIR(cor) | A_BOLD);
}

void interface_inicializar() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(COLOR_HEADER, COLOR_CYAN, -1);
        init_pair(COLOR_HAND, COLOR_GREEN, -1);
        init_pair(COLOR_BOARD, COLOR_YELLOW, -1);
        init_pair(COLOR_PROBS, COLOR_MAGENTA, -1);
        init_pair(COLOR_PROGRESS, COLOR_BLUE, -1);
        init_pair(COLOR_HEARTS, COLOR_RED, -1);
        init_pair(COLOR_DIAMONDS, COLOR_YELLOW, -1);
        init_pair(COLOR_CLUBS, COLOR_GREEN, -1);
        init_pair(COLOR_SPADES, COLOR_CYAN, -1);
    }
}

void interface_finalizar() {
    endwin();
}

void interface_desenhar_layout() {
    clear();
    attron(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    mvprintw(0, (COLS - 26) / 2, "ProbabilidadesPoker");
    mvprintw(1, (COLS - 46) / 2, "Meu primeiro projeto de pôquer com probabilidade real!");
    attroff(COLOR_PAIR(COLOR_HEADER) | A_BOLD);

    attron(A_DIM);
    mvprintw(3, 2, "Digite cartas no formato valor naipe (ex: A C, 10 O).");
    mvprintw(4, 2, "Naipes: C = ♥, O = ♦, P = ♣, E = ♠.");
    attroff(A_DIM);

    attron(COLOR_PAIR(COLOR_HAND) | A_BOLD);
    mvprintw(MAO_Y, 2, "Sua mão:");
    attroff(COLOR_PAIR(COLOR_HAND) | A_BOLD);

    attron(COLOR_PAIR(COLOR_BOARD) | A_BOLD);
    mvprintw(MESA_Y, 2, "Mesa:");
    attroff(COLOR_PAIR(COLOR_BOARD) | A_BOLD);

    attron(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    mvprintw(INFO_Y, 2, "Classificação atual: --");
    mvprintw(INFO_Y + 1, 2, "Rodando simulações: --");
    mvprintw(INFO_Y + 2, 2, "Vitória: --   Empate: --   Derrota: --");
    attroff(COLOR_PAIR(COLOR_PROBS) | A_BOLD);

    refresh();
}

static void limpar_linha(int y) {
    move(y, 0);
    clrtoeol();
}

void interface_atualizar_mao(Mao mao) {
    limpar_linha(MAO_Y + 1);
    int x = 2;
    for (int i = 0; i < 2; i++) {
        desenhar_carta(mao.cartas[i], MAO_Y + 1, x);
        x += 8;
    }
    refresh();
}

void interface_atualizar_mesa(Mesa mesa) {
    limpar_linha(MESA_Y + 1);
    int x = 2;
    for (int i = 0; i < mesa.numCartas; i++) {
        desenhar_carta(mesa.cartas[i], MESA_Y + 1, x);
        x += 8;
    }
    refresh();
}

void interface_mostrar_classificacao(const char *nome) {
    attron(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    mvprintw(INFO_Y, 2, "Classificação atual: %-20s", nome);
    attroff(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    refresh();
}

void interface_mostrar_simulacoes(int simulacoes) {
    attron(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    mvprintw(INFO_Y + 1, 2, "Rodando %6d simulações...", simulacoes);
    attroff(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    refresh();
}

void interface_mostrar_probabilidades(Probabilidades probs) {
    attron(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    mvprintw(INFO_Y + 2, 2,
             "Vitória: %6.2f%%   Empate: %6.2f%%   Derrota: %6.2f%%",
             probs.vitoria, probs.empate, probs.derrota);
    attroff(COLOR_PAIR(COLOR_PROBS) | A_BOLD);
    refresh();
}

void interface_iniciar_progresso(int total) {
    attron(COLOR_PAIR(COLOR_PROGRESS) | A_BOLD);
    mvprintw(PROGRESS_Y, 2, "Simulando %d cenários...", total);
    attroff(COLOR_PAIR(COLOR_PROGRESS) | A_BOLD);
    limpar_linha(PROGRESS_Y + 1);
    mvprintw(PROGRESS_Y + 1, 2, "[%-40s]   0%%", "");
    refresh();
}

void interface_atualizar_progresso(int atual, int total) {
    double porcentagem = (total == 0) ? 0.0 : (double)atual / total;
    int preenchidos = (int)(porcentagem * 40);
    if (preenchidos > 40) preenchidos = 40;

    char barra[41];
    for (int i = 0; i < 40; i++)
        barra[i] = (i < preenchidos) ? '#' : ' ';
    barra[40] = '\0';

    mvprintw(PROGRESS_Y + 1, 2, "[%s] %3d%%", barra, (int)(porcentagem * 100));
    refresh();
}

void interface_finalizar_progresso() {
    attron(COLOR_PAIR(COLOR_PROGRESS) | A_BOLD);
    mvprintw(PROGRESS_Y, 2, "Simulações concluídas!%*s", 20, "");
    attroff(COLOR_PAIR(COLOR_PROGRESS) | A_BOLD);
    interface_atualizar_progresso(1, 1);
}

void interface_mostrar_mensagem(const char *mensagem) {
    limpar_linha(INPUT_Y);
    limpar_linha(INPUT_Y + 1);
    mvprintw(INPUT_Y, INPUT_X, "%s", mensagem);
    refresh();
}

static int converter_valor(const char *valorStr) {
    if (strlen(valorStr) == 0) return -1;
    if (strcmp(valorStr, "A") == 0) return 14;
    if (strcmp(valorStr, "K") == 0) return 13;
    if (strcmp(valorStr, "Q") == 0) return 12;
    if (strcmp(valorStr, "J") == 0) return 11;
    int valor = atoi(valorStr);
    if (valor >= 2 && valor <= 10) return valor;
    return -1;
}

static char converter_naipe(char naipeChar) {
    switch (naipeChar) {
        case 'C': return 'C';
        case 'O': return 'O';
        case 'P': return 'P';
        case 'E': return 'E';
        default: return '\0';
    }
}

Carta interface_solicitar_carta(const char *prompt) {
    Carta carta;
    while (1) {
        interface_mostrar_mensagem(prompt);
        echo();
        curs_set(1);
        char entrada[16];
        move(INPUT_Y + 1, INPUT_X);
        clrtoeol();
        getnstr(entrada, sizeof(entrada) - 1);
        noecho();
        curs_set(0);

        char valorStr[4] = "";
        char naipeChar = '\0';
        int lidos = sscanf(entrada, "%3s %c", valorStr, &naipeChar);
        if (lidos < 2 && strlen(entrada) >= 2) {
            valorStr[0] = entrada[0];
            valorStr[1] = '\0';
            naipeChar = entrada[1];
        }

        for (size_t i = 0; i < strlen(valorStr); i++)
            valorStr[i] = (char)toupper((unsigned char)valorStr[i]);
        naipeChar = (char)toupper((unsigned char)naipeChar);

        int valor = converter_valor(valorStr);
        char naipe = converter_naipe(naipeChar);

        if (valor != -1 && naipe != '\0') {
            carta.valor = valor;
            carta.naipe = naipe;
            return carta;
        }

        interface_mostrar_mensagem("Entrada inválida! Exemplos: A C, 10 O, 7 P.");
        napms(900);
    }
}

int interface_perguntar_continuar(const char *pergunta) {
    interface_mostrar_mensagem(pergunta);
    while (1) {
        int ch = getch();
        if (ch == 's' || ch == 'S') return 1;
        if (ch == 'n' || ch == 'N') return 0;
    }
}

void interface_esperar_tecla() {
    interface_mostrar_mensagem("Pressione qualquer tecla para sair...");
    getch();
}
