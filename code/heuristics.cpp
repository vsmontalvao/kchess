/*
 *  heuristics.cpp
 *  WeisserKoenig
 *
 *  Created by Pedro Bittencourt Arruda on 12/10/11.
 *  Copyright 2011 ITA. All rights reserved.
 *
 */

#include "heuristics.h"

short posWg[][64] =
{
	{0}, //vazio

	{//peão
		  0,   5,   5,  -1,   5,  20,  75,   0,

		  0,  10, -10,   0,   5,  20,  75,   0,

		  0,  10, -10,   0,   8,  20,  75,   0,

		  0, -15,   0,  20,  25,  40,  75,   0,

		  0, -15,   0,  20,  25,  40,  75,   0,

		  0,  20, -20,   0,   8,  20,  75,   0,

		  0,  10, -10,   0,   5,  20,  75,   0,

		  0,   5,   5,  -1,   5,  20,  75,   0
	},

	{//cavalo
		-60, -45, -20, -20, -20, -20, -45, -60,

		-45,  -10,   0,   0,   0,  0, -10, -45,

		-20,   0,  10,  10,  10,  10,   0, -20,

		-20,   5,  10,  30,  30,  10,   0, -20,

		-20,   5,  10,  30,  30,  10,   0, -20,

		-20,   0,  10,  10,  10,  10,   0, -20,

		-45, -10,   0,   0,   0,  0,  -10, -45,

		-60, -45, -20, -20, -20, -20, -45, -60,
	},

	{//bispo
		 -5, -10, -10, -10, -10, -10, -10,  -5,

		-10,   2,   0,   5,   5,   0,   2, -10,

		-10,   0,   5,  10,  10,   5,   0, -10,

		-10,   5,  10,  15,  15,  10,   5, -10,

		-10,   5,  10,  15,  15,  10,   5, -10,

		-10,   0,   5,  10,  10,   5,   0, -10,

		-10,   2,   0,   5,   5,   0,   2, -10,

		 -5, -10, -10, -10, -10, -10, -10,  -5,
	},

	{//torre
		  0,  -5,  -5,  -5,  -5,  -5,  -5,   0,

		  0,   0,   0,   0,   0,   0,  10,   2,

		  5,   0,   0,   0,   0,   0,  10,   2,

		  7,   0,   0,   0,   0,   0,  10,   2,

		  7,   0,   0,   0,   0,   0,  10,   2,

		  5,   0,   0,   0,   0,   0,  10,   2,

		  0,   0,   0,   0,   0,   0,  10,   2,

		  0,  -5,  -5,  -5,  -5,  -5,   8,   0,
	},

	{//rainha
		-20, -15, -10,  -5,  -5, -10, -15, -20,

		-15,   0,   0,   0,   0,   0,   0, -15,

		-10,   0,   5,   5,   5,   5,   0, -10,

		  0,   0,   5,   7,   7,   5,   0,  -5,

		 -2,   0,   5,   7,   7,   5,   0,  -5,

		-10,   0,   5,   5,   5,   5,   0, -10,

		-15,   0,   0,   0,   0,   0,   0, -15,

		-20, -15, -10,  -5,  -5, -10, -15, -20,
	}
};

int posKgWg[][64] =
{
	{//início e meio
		 5,   0,  -5, -30, -50, -50, -70, -70,

		20,  -2,  -7, -30, -50, -50, -70, -70,

		30,  -2, -10, -30, -50, -50, -70, -70,

		 0,  -5, -10, -30, -50, -50, -70, -70,

		 0,  -5, -10, -30, -50, -50, -70, -70,

		20,  -5, -10, -30, -50, -50, -70, -70,

		30,  -2,  -7, -30, -50, -50, -70, -70,

		10,   0,  -5, -25, -50, -50, -70, -70,
	},

	{//fim
		-45, -25, -15, -15, -15, -15, -25, -45,

		-25,  -5,   0,   0,   0,   0,  -5, -25,

		-15,   0,  10,  15,  15,  10,   0, -15,

		-15,   0,  15,  30,  30,  15,   0, -15,

		-15,   0,  15,  30,  30,  15,   0, -15,

		-15,   0,  10,  15,  15,  10,   0, -15,

		-25,  -5,   0,   0,   0,   0,  -5, -25,

		-45, -25, -15, -15, -15, -15, -25, -45,
	}
};

int get_heuristics(SearchBoard *board, Player p)
{
	const int pieceToInd[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0, 0};
	//const short pieceToDist[13] = {0, 0, 0, 3, -3, 4, -4, 5, -5, 9, -9, 2, -2};

	register int heu = 0;
	register enum piece cPos;
	register int totMat = 0;

	if((heu = findHeu(*board, p)) != 0x7FFF)
		return heu;

	for(int i = 0; i < 64; i++)
	{
		cPos = board->board[i];

		heu += (cPos%2 ? weights[cPos] : weights[cPos]); // Soma material
		heu += (cPos%2 ? posWg[pieceToInd[cPos]][i] : -posWg[pieceToInd[cPos]][i ^ 7]);
		// Soma posição(magia: i ^ 7)

		totMat += weights[cPos];
	}

	//Posição do rei branco ponderada pela fase do jogo:
	heu += (posKgWg[0][board->kings[whites]]*(totMat - 40000) +
			posKgWg[1][board->kings[whites]]*(44220 - totMat))/4200;

	//Posição do rei preto ponderada pela fase do jogo:
	heu -= (posKgWg[0][board->kings[blacks]]*(totMat - 40000) +
			posKgWg[1][board->kings[blacks]]*(44220 - totMat))/4200;

	// Já que não temos mais nada a acrescentar...

	// Coloca na hash o novo valor:
	board->heu = heu;
	addToHash(*board, p);

	return heu;
}
