#ifndef MOVE_H
#define MOVE_H
/*
 *  move.h
 *  WeisserKoenig
 *
 */

// AUTOR : Pedro Bittencourt Arruda.

#define SHORT_MAX 0x7FFF // Alguém discorda?

// Define tipos para serem usados no programa:

typedef unsigned char position; /* Posições são guardadas em unsigned char e consistem em um
	número de 0 a 63, cujos três primeiros bits correpondem à linha e os três seguintes
	correspondem à coluna. */

typedef unsigned short move; /* Um movimento é um conjunto de 12 bits em que os seis primeiros
	correspondem à posição inicial e os seis últimos à posição final, como definido
	anteriormente. */

// Define os macros pra serem usados em conjunto com as definições:

#define _mov(a, b) ((a) + ((b) << 6)) /* Representa um movimento com posição inical a e final
	b. Recomendo usar notação octal para a e b (e.g.: 012 = 10). */
	//Não entendi nada --Felipe

#define final(m) ( ((unsigned char)((m)>>6)) & 0x3F ) /* Retorna posição final de um movimento */
#define initial(m) ( ((unsigned char)(m)) & 0x3F ) /* Retorna posição inicial de um movimento */


// Funções para representação em string de movimentos:

// FUNÇÃO movToStr: escreve o movimento m na string str.
void movToStr(move m, char str[]);

// FUNÇÃO strToMov: gera o movimento m correspondente à string str. Acho que está bugado...
// Eu tenho certeza :-), já corrigi --Felipe
void strToMov(move *m, const char str []);


// Definição do tipo enum piece: não mexa! É crucial esta ordem exata...

enum piece {
	empty = 0, // Eu esqueci se começa no 0 ou no 1. Só por precaução...
	wpawn, bpawn, wknight, bknight, wbishop, bbishop,
	wrook, brook, wqueen, bqueen, wking, bking, invalid
};

typedef enum piece PIECE; // para quem quiser usar...

// Macros associados a enum piece:

#define isSameColor(x, y) ((x)%2 == (y)%2 && (x) && (y)) /* Verfica se duas peças são da
	mesma cor. O valor empty não corresponde a nenhuma cor. */

#define isSameColorNE(x, y) ( (x)%2 == (y)%2 ) /* Mesmo que o anterior, mas se você sabe
	que nenhuma das peças são 'nulas'. */

// Define os valores associadas a cada jogador:
typedef enum Player {blacks, whites} Player;

// A variável que guardará o estado inical do jogo:
const enum piece start_pos[64] = {
	wrook,		wpawn,	empty,	empty,	empty,	empty, bpawn, brook,

	wknight,	wpawn,	empty,	empty,	empty,	empty, bpawn, bknight,

	wbishop,	wpawn,	empty,	empty,	empty,	empty, bpawn, bbishop,

	wqueen,		wpawn,	empty,	empty,	empty,	empty, bpawn, bqueen,

	wking,		wpawn,	empty,	empty,	empty,	empty, bpawn, bking,

	wbishop,	wpawn,	empty,	empty,	empty,	empty, bpawn, bbishop,

	wknight,	wpawn,	empty,	empty,	empty,	empty, bpawn, bknight,

	wrook,		wpawn,	empty,	empty,	empty,	empty, bpawn, brook,
};

// A variável que guarda os pesos das peças:
const int weights[] = {0, 100, 100, 300, 300, 300, 300, 500, 500, 900, 900, 20000, 20000};

// Define estrutura de jogadas especiais:
typedef struct specMov
{
	bool rkKing, rkQueen;
	bool enPass[8];
} specMov;

// Define uma estrutura que o Fábio estava precisando:
typedef struct SearchBoard
{
	enum piece board[64];
	int mat[2];		// Material das peças em jogo: começa em 39 para cada jogador.
	specMov spc[2];		// Jogadas especiais para as brancas e pretas.
	position kings[2];  // Indica as posições dos reis.
	int heu;			// Valor atribuido para a heurística de posição.
	int hashAddr;
} SearchBoard;

#endif
