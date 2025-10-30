#ifndef INTERFACE_H
#define INTERFACE_H

#include "baralho.h"

void interface_inicializar();
void interface_finalizar();
void interface_desenhar_layout();
Carta interface_solicitar_carta(const char *prompt);
int interface_perguntar_continuar(const char *pergunta);
void interface_atualizar_mao(Mao mao);
void interface_atualizar_mesa(Mesa mesa);
void interface_mostrar_classificacao(const char *nome);
void interface_mostrar_simulacoes(int simulacoes);
void interface_mostrar_probabilidades(Probabilidades probs);
void interface_iniciar_progresso(int total);
void interface_atualizar_progresso(int atual, int total);
void interface_finalizar_progresso();
void interface_mostrar_mensagem(const char *mensagem);
void interface_esperar_tecla();

#endif
