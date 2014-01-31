#ifndef POSMOV_H
#define POSMOV_H

/*
 *  posMov.h
 *  WeisserKoenig
 *
 *
 */

// AUTOR: Pedro Bittencourt Arruda.

#include "move.h"

extern int randBTS[13][64];

// FUNÇÃO isAttacked: Verifica se uma casa está sendo atacada por um jogador em uma posição
//		válida. Serve para checar xeque-mate e movimentos do rei.
bool isAttacked(enum piece board[], Player turn, position i);

// FUNÇÃO posMov: dada uma posição de um tabuleiro e um jogador a jogar, indica todos os
//		lançes para aquela posição em um vetor de move. No entanto, jogadas especiais
//		como roque e en passant não são contabilizadas, por necessitar de informação
//		externa(consenso em como arrumar isto? Como passar informação extra?). Ainda, jogadas
//		que colocam o rei em xeque(que são inválidas) tabém são contabilizadas. Quando
//		necessário, precisa-se usar isAttacked para se decidir se a posição é válida.
move* posMov(SearchBoard board, Player turn);

// FUNÇÃO mkUsrMove: faz um movimento checando todas as condições necessárias e coordena jogadas
//		especiais. Retorna um rei branco caso a jogada seja inválida. É feita para checar as
//		condições do usuário. Pode ser transformado em um método de um classe se alguem quser...
enum piece mkUsrMove(SearchBoard *board, Player turn, move m);

// FUNÇÃO mkCpmMove: mesmo que o anterior, mas não checa tantas condições e é feito para os
//		movimentos do computador. Parte do pressuposto de que a jogada é válida.
enum piece mkCmpMove(SearchBoard *board, Player turn, move m);

// FUNÇÃO fastMov: faz um movimento no tabuleiro sem checar quaisquer condições e retorna a
//		peça capturada. Usa três registers, então, cuidado.
inline enum piece fastMov(enum piece board[], move m);

// FUNÇÃO undoMov: desfaz um movimento do tabuleiro, dado que ele é válido e foi feito.
//		Não muda o lado a jogar. Use turn = !turn depois.
//		NOTA: seu asma! essa função é inútil, já que não se pode sempre voltar uma jogada só
//			com o conhecimento dos parâmetros desta função(i.e: se foi movimento do rei, como vou
//			saber se ele podia rocar antes?). Ah! não implementei os roques. Enfim, se possível,
//			NÃO USE ESTA FUNÇÃO!!!!
inline void undoMov(SearchBoard *board, move m, enum piece captured);


// FUNÇÕES de estado de jogo:
#define isCheck(board, turn) (isAttacked((board)->board, (Player)!(turn), (board)->kings[turn])) // Não excluindo xeque-mate.
bool isCheckmate(SearchBoard *board, Player turn);
bool isStalemate(SearchBoard *board, Player turn);

// FUNÇÃO printBoard: imprime uma posição na tela.
void printBoard(enum piece board[]);	// NOVO!!!

#endif
