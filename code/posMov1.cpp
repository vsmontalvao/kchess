/*
 *  posMov.cpp
 *  WeisserKoenig
 *
 *
 */

// Uma prévia de como está sendo. Está muito bronco?
// AUTOR: Pedro Bittencourt Arruda.

#include "posMov.h"
#include <stdlib.h>
#include <stdio.h>

#define notFrontier(i) ( (((i) + 1) & 7) && ((i) & 7) && ((i) > 7) && ((i) < 56) )
#define notFrontierH8(i) ( (((i) + 1) & 7) && ((i) < 56) )
#define notFrontierA1(i) ( ((i) & 7) && ((i) > 7)
// Os demais casos se implementam na mão mesmo...

#define leak(i) {if((i) > 63) printf("Leak: %d", (i));}

// FUNÇÃO isAttacked:
bool isAttacked(enum piece board[], Player turn, position i)
{
	//register enum piece p;

	// Ataque por peão:
	// NOTA: nos testes, um peão na oitava ou primeira casa é bugado! Como isso não acontece na
	//		prática, evite deixar um lá por qualquer motivo!
	if(turn == whites ?
		(i < 56 && board[i + 007] == wpawn) || (i > 011 && board[i - 011] == wpawn)
		:
		(i > 7 && board[i - 007] == bpawn) || (i < 64 - 011 && board[i + 011] == bpawn))
		return true;//*/

	// Caso de en passant:
	//for(int i = 0; i < 8; i++)
		// que se lasque! quando algém for usar, que peça para eu fazer isso!!!

	// Ataque por cavalo:
	if((turn == whites && ((i%8 < 7 && (i >> 3) < 6 && board[i + 021] == wknight) ||
						   (i%8 < 6 && (i >> 3) < 7 && board[i + 012] == wknight) ||
						   (i%8 < 6 && (i >> 3) > 0 && board[i - 006] == wknight) ||
						   (i%8 < 7 && (i >> 3) > 1 && board[i - 017] == wknight) ||
						   (i%8 > 0 && (i >> 3) > 1 && board[i - 021] == wknight) ||
						   (i%8 > 1 && (i >> 3) > 0 && board[i - 012] == wknight) ||
						   (i%8 > 1 && (i >> 3) < 7 && board[i + 006] == wknight) ||
						   (i%8 > 0 && (i >> 3) < 6 && board[i + 017] == wknight)))
	   ||
	   (turn == blacks && ((i%8 < 7 && (i >> 3) < 6 && board[i + 021] == bknight) ||
						   (i%8 < 6 && (i >> 3) < 7 && board[i + 012] == bknight) ||
						   (i%8 < 6 && (i >> 3) > 0 && board[i - 006] == bknight) ||
						   (i%8 < 7 && (i >> 3) > 1 && board[i - 017] == bknight) ||
						   (i%8 > 0 && (i >> 3) > 1 && board[i - 021] == bknight) ||
						   (i%8 > 1 && (i >> 3) > 0 && board[i - 012] == bknight) ||
						   (i%8 > 1 && (i >> 3) < 7 && board[i + 006] == bknight) ||
						   (i%8 > 0 && (i >> 3) < 6 && board[i + 017] == bknight))))
		return true;//*/

	// Ataque por bispo ou dama:
	register int j;

	if((i + 1) & 7 && i < 56)
	{
		for(j = 011; board[i + j] == empty && notFrontier(i + j); j += 011);

		if(turn == whites && (board[i + j] == wqueen || board[i + j] == wbishop) ||
			turn == blacks && (board[i + j] == bqueen || board[i + j] == bbishop))
			return true;
	}

	if(i & 7 && i > 7)
	{
		for(j = 011; board[i - j] == empty && notFrontier(i - j); j += 011);

		if(turn == whites && (board[i - j] == wqueen || board[i - j] == wbishop) ||
		   turn == blacks && (board[i - j] == bqueen || board[i - j] == bbishop))
			return true;
	}

	if((i + 1) & 7 && i > 7)
	{
		for(j = 007; board[i - j] == empty && notFrontier(i - j); j += 007);

		if(turn == whites && (board[i - j] == wqueen || board[i - j] == wbishop) ||
		   turn == blacks && (board[i - j] == bqueen || board[i - j] == bbishop))
			return true;
	}

	if(i & 7 && i < 56)
	{
		for(j = 007; board[i + j] == empty && notFrontier(i + j); j += 007);

		if(turn == whites && (board[i + j] == wqueen || board[i + j] == wbishop) ||
		   turn == blacks && (board[i + j] == bqueen || board[i + j] == bbishop))
			return true;
	}
	//*/

	// Ataque por torre ou dama:
	if((i + 1) & 7)
	{
		for(j = 001; board[i + j] == empty && ((i + j + 1) & 7); j += 001);

		if(turn == whites && (board[i + j] == wqueen || board[i + j] == wrook) ||
		   turn == blacks && (board[i + j] == bqueen || board[i + j] == brook))
			return true;
	}

	if(i & 7)
	{
		for(j = 001; board[i - j] == empty && ((i - j) & 7); j += 001);

		if(turn == whites && (board[i - j] == wqueen || board[i - j] == wrook) ||
		   turn == blacks && (board[i - j] == bqueen || board[i - j] == brook))
			return true;
	}

	if(i > 7)
	{
		for(j = 010; board[i - j] == empty && (i - j > 7); j += 010);

		if(turn == whites && (board[i - j] == wqueen || board[i - j] == wrook) ||
		   turn == blacks && (board[i - j] == bqueen || board[i - j] == brook))
			return true;
	}

	if(i < 56)
	{
		for(j = 010; board[i + j] == empty && (i + j < 56); j += 010);

		if(turn == whites && (board[i + j] == wqueen || board[i + j] == wrook) ||
		   turn == blacks && (board[i + j] == bqueen || board[i + j] == brook))
			return true;
	}//*/

	// Ataque por rei:
	if((turn == whites && ((i%8 < 7 &&                 board[i + 001] == wking) ||
						   (i%8 < 7 && (i >> 3) < 7 && board[i + 011] == wking) ||
						   (           (i >> 3) < 7 && board[i + 010] == wking) ||
						   (i%8 > 0 && (i >> 3) < 7 && board[i + 007] == wking) ||
						   (i%8 > 0 &&                 board[i - 001] == wking) ||
						   (i%8 > 0 && (i >> 3) > 0 && board[i - 011] == wking) ||
						   (           (i >> 3) > 0 && board[i - 010] == wking) ||
						   (i%8 < 7 && (i >> 3) > 0 && board[i - 007] == wking)))
	   ||
	   (turn == blacks && ((i%8 < 7 &&                 board[i + 001] == bking) ||
						   (i%8 < 7 && (i >> 3) < 7 && board[i + 011] == bking) ||
						   (           (i >> 3) < 7 && board[i + 010] == bking) ||
						   (i%8 > 0 && (i >> 3) < 7 && board[i + 007] == bking) ||
						   (i%8 > 0 &&                 board[i - 001] == bking) ||
						   (i%8 > 0 && (i >> 3) > 0 && board[i - 011] == bking) ||
						   (           (i >> 3) > 0 && board[i - 010] == bking) ||
						   (i%8 < 7 && (i >> 3) > 0 && board[i - 007] == bking))))
		return true;//*/

	return false;
}

move * posMov(SearchBoard board, Player turn)
{
	register move * moves = (move*) malloc(350*sizeof(moves)); /* Com o limite superior
																sugerido pelo Fábio. */
	register int count = 0;
	//register move m;
	//register reg;

	for(int i = 0; i < 64; i++)
	{
		if(board.board[i]%2 == turn) // Ah não! isso eu NÃO indento!!!
		switch(board.board[i])
		{
				// para cada jogada possível (evite for, se pssível...)
				// verificar se é válida (if)
				// então moves[count++] = m;

			// Movimentos do peão branco:
			case wpawn:
				// Movimento comum(sem promoção):
				if(i%8 < 6 && board.board[i + 001] == empty)
					moves[count++] = _mov(i, i + 001);

				// Capturar à direita(sem promoção):
				if(i%8 < 6 && (i >> 3) < 7 && board.board[i + 011]%2 == 0
				   && board.board[i + 011])
					moves[count++] = _mov(i, i + 011);

				// Capturar à direita(sem promoção):
				if(i%8 < 6 && (i >> 3) > 0 && board.board[i - 007]%2 == 0
				   && board.board[i - 007])
					moves[count++] = _mov(i, i - 007);

				// Movimento inicial de duas caasa:
				if(i%8 == 1 && board.board[i + 002] == empty && board.board[i + 001] == empty)
					moves[count++] = _mov(i, i + 002);

				// Promoção:
				if(i%8 == 6)
				{
					if(i < 63 && board.board[i + 001] == empty)
					{
						moves[count++] = _mov(i, i + 001) + (wknight << 12);
						moves[count++] = _mov(i, i + 001) + (wbishop << 12);
						moves[count++] = _mov(i, i + 001) + (wrook << 12);
						moves[count++] = _mov(i, i + 001) + (wqueen << 12);
					}

					if(i > 7 && board.board[i - 007] != empty)
					{
						moves[count++] = _mov(i, i - 007) + (wknight << 12);
						moves[count++] = _mov(i, i - 007) + (wbishop << 12);
						moves[count++] = _mov(i, i - 007) + (wrook << 12);
						moves[count++] = _mov(i, i - 007) + (wqueen << 12);
					}

					if(i < 56 && board.board[i + 011] != empty)
					{
						moves[count++] = _mov(i, i + 011) + (wknight << 12);
						moves[count++] = _mov(i, i + 011) + (wbishop << 12);
						moves[count++] = _mov(i, i + 011) + (wrook << 12);
						moves[count++] = _mov(i, i + 011) + (wqueen << 12);
					}
				}

				// não implementei en-passant...
				// En passant:
				for(int j = 0; j < 8; j++)
				 if(board.spc[whites].enPass[j])
				 {
					 if(j < 7 && board.board[(j << 3) + 014] == wpawn)
						 moves[count++] = _mov((j << 3) + 014, (j << 3) + 005);
					 if(j > 0 && board.board[(j << 3) + 004 - 010] == wpawn)
						 moves[count++] = _mov((j << 3) + 004 - 010, (j << 3) + 005);
				 }
				break;

			// Movimentos do peão preto:
			case bpawn:
				// Movimento comum(sem promoção):
				if(i%8 > 0 && board.board[i - 001] == empty)
					moves[count++] = _mov(i, i - 001);

				// Capturar à direita(sem promoção):
				if(i%8 > 0 && (i >> 3) > 0 && board.board[i - 011]%2 == 1)
					moves[count++] = _mov(i, i - 011);

				// Capturar à esquerda(sem promoção):
				if(i%8 > 0 && (i >> 3) < 7 && board.board[i + 007]%2 == 1)
					moves[count++] = _mov(i, i + 007);

				// Movimento inical de duas casas:
				if(i%8 == 6 && board.board[i - 002] == empty && board.board[i - 001] == empty)
					moves[count++] = _mov(i, i - 002);

				// Promoção:
				if(i%8 == 1)
				{
					if(board.board[i - 001] == empty)
					{
						moves[count++] = _mov(i, i - 001) + (bknight << 12);
						moves[count++] = _mov(i, i - 001) + (bbishop << 12);
						moves[count++] = _mov(i, i - 001) + (brook << 12);
						moves[count++] = _mov(i, i - 001) + (bqueen << 12);
					}

					if(board.board[i + 007] != empty)
					{
						moves[count++] = _mov(i, i + 007) + (bknight << 12);
						moves[count++] = _mov(i, i + 007) + (bbishop << 12);
						moves[count++] = _mov(i, i + 007) + (brook << 12);
						moves[count++] = _mov(i, i + 007) + (bqueen << 12);
					}

					if(board.board[i - 011] != empty)
					{
						moves[count++] = _mov(i, i - 011) + (bknight << 12);
						moves[count++] = _mov(i, i - 011) + (bbishop << 12);
						moves[count++] = _mov(i, i - 011) + (brook << 12);
						moves[count++] = _mov(i, i - 011) + (bqueen << 12);
					}
				}

				// En passant:
				for(int j = 0; j < 8; j++)
					if(board.spc[blacks].enPass[j])
					{
						if(j < 7 && board.board[(j << 3) + 013] == bpawn)
							moves[count++] = _mov((j << 3) + 013, (j << 3) + 002);
						if(j > 0 && board.board[(j << 3) + 003 - 010] == bpawn)
							moves[count++] = _mov((j << 3) + 003 - 010, (j << 3) + 002);
					}
				break;

			// Movimentos do cavaleiro:
			case wknight: case bknight:
				if(i%8 < 7 && (i >> 3) < 6 && !isSameColor(board.board[i], board.board[i + 021]))
					moves[count++] = _mov(i, i + 021);

				if(i%8 < 6 && (i >> 3) < 7 && !isSameColor(board.board[i], board.board[i + 012]))
					moves[count++] = _mov(i, i + 012);

				if(i%8 < 6 && (i >> 3) > 0 && !isSameColor(board.board[i], board.board[i - 006]))
					moves[count++] = _mov(i, i - 006);

				if(i%8 < 7 && (i >> 3) > 1 && !isSameColor(board.board[i], board.board[i - 017]))
					moves[count++] = _mov(i, i - 017);

				if(i%8 > 0 && (i >> 3) > 1 && !isSameColor(board.board[i], board.board[i - 021]))
					moves[count++] = _mov(i, i - 021);

				if(i%8 > 1 && (i >> 3) > 0 && !isSameColor(board.board[i], board.board[i - 012]))
					moves[count++] = _mov(i, i - 012);

				if(i%8 > 1 && (i >> 3) < 7 && !isSameColor(board.board[i], board.board[i + 006]))
					moves[count++] = _mov(i, i + 006);

				if(i%8 > 0 && (i >> 3) < 6 && !isSameColor(board.board[i], board.board[i + 017]))
					moves[count++] = _mov(i, i + 017);
				break;

			// As quatro direções do bispo e movimentos da rainha:
			// Ele tá checando algumas condições desnecessarias, mas são rápidas...
			case wbishop: case bbishop: case wqueen: case bqueen:
				if((i + 1) & 7 && i < 56)
				for(int j = 011; ; j += 011)
				{
					if(board.board[i + j] == empty && notFrontier(i + j))
						moves[count++] = _mov(i, i + j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i + j]))
							moves[count++] = _mov(i, i + j);

						break;
					}
				}

				if(i & 7 && i > 7)
				for(int j = 011; ; j += 011)
				{
					if(board.board[i - j] == empty && notFrontier(i - j))
						moves[count++] = _mov(i, i - j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i - j]))
							moves[count++] = _mov(i, i - j);

						break;
					}
				}

				if(i & 7 && i < 56)
				for(int j = 007; ; j += 007)
				{
					if(board.board[i + j] == empty && notFrontier(i + j))
						moves[count++] = _mov(i, i + j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i + j]))
							moves[count++] = _mov(i, i + j);

						break;
					}
				}

				if((i + 1) & 7 && i > 7)
				for(int j = 007; ; j += 007)
				{
					if(board.board[i - j] == empty && notFrontier(i - j))
						moves[count++] = _mov(i, i - j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i - j]))
							moves[count++] = _mov(i, i - j);

						break;
					}
				}

				if(board.board[i] == wbishop || board.board[i] == bbishop)
					break; /* O case só termina para o bispo. Para a rainha,
							ainda precisamos contar os movimentos de torre. */

			// As quatro direções da torre:
			case wrook: case brook:
				if((i +1) & 7)
				for(int j = 001; ; j += 001)
				{
					if(board.board[i + j] == empty && ((i + j + 1) & 7) && ((i + j) & 7))
						moves[count++] = _mov(i, i + j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i + j]))
							moves[count++] = _mov(i, i + j);

						break;
					}
				}

				if(i & 7) // Nem vem que não tem! Não vou indentar!
				for(int j = 001; ; j += 001)
				{
					if(board.board[i - j] == empty && ((i - j + 1) & 7) && ((i - j) & 7))
						moves[count++] = _mov(i, i - j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i - j]))
							moves[count++] = _mov(i, i - j);

						break;
					}
				}

				if(i < 56)
				for(int j = 010; ; j += 010)
				{
					if(board.board[i + j] == empty && (i + j) < 56)
						moves[count++] = _mov(i, i + j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i + j]))
							moves[count++] = _mov(i, i + j);

						break;
					}
				}

				if(i > 7)
				for(int j = 010; ; j += 010)
				{
					if(board.board[i - j] == empty && (i - j) > 7)
						moves[count++] = _mov(i, i - j);
					else
					{
						if(!isSameColor(board.board[i], board.board[i - j]))
							moves[count++] = _mov(i, i - j);

						break;
					}
				}
				break;

			// Movimentos do rei:
			case wking: case bking: // esse vai ser o mais difícil!
				if(i%8 < 7 && !isSameColor(board.board[i + 001], board.board[i]))
					moves[count++] = _mov(i, i + 001);
				if(i%8 < 7 && (i >> 3) < 7 && !isSameColor(board.board[i + 011], board.board[i]))
					moves[count++] = _mov(i, i + 011);
				if((i >> 3) < 7 && !isSameColor(board.board[i + 010], board.board[i]))
					moves[count++] = _mov(i, i + 010);
				if(i%8 > 0 && (i >> 3) < 7 && !isSameColor(board.board[i + 007], board.board[i]))
					moves[count++] = _mov(i, i + 007);
				if(i%8 > 0 && !isSameColor( board.board[i - 001], board.board[i]))
					moves[count++] = _mov(i, i - 001);
				if(i%8 > 0 && (i >> 3) > 0 && !isSameColor(board.board[i - 011], board.board[i]))
					moves[count++] = _mov(i, i - 011);
				if((i >> 3) > 0 && !isSameColor(board.board[i - 010], board.board[i]))
					moves[count++] = _mov(i, i - 010);
				if(i%8 < 7 && (i >> 3) > 0 && !isSameColor(board.board[i - 007], board.board[i]))
					moves[count++] = _mov(i, i - 007);
				break;
		}
	}

	// Todos os roques de todas as cores e sabores:
	if(board.spc[whites].rkKing && turn == whites &&
		!isAttacked(board.board, blacks, 040) &&
		board.board[050] == empty && !isAttacked(board.board, blacks, 050) &&
		board.board[060] == empty && !isAttacked(board.board, blacks, 060))
		moves[count++] = _mov(040, 060);

	if(board.spc[blacks].rkKing && turn == blacks &&
	   !isAttacked(board.board, whites, 047) &&
	   board.board[057] == empty && !isAttacked(board.board, whites, 057) &&
	   board.board[067] == empty && !isAttacked(board.board, whites, 067))
		moves[count++] = _mov(047, 067);

	if(board.spc[whites].rkQueen && turn == whites &&
	   !isAttacked(board.board, blacks, 040) &&
	   board.board[030] == empty && !isAttacked(board.board, blacks, 030) &&
	   board.board[020] == empty && !isAttacked(board.board, blacks, 020) &&
	   board.board[010] == empty)
		moves[count++] = _mov(040, 020);

	if(board.spc[blacks].rkQueen && turn == blacks &&
	   !isAttacked(board.board, whites, 047) &&
	   board.board[037] == empty && !isAttacked(board.board, whites, 037) &&
	   board.board[027] == empty && !isAttacked(board.board, whites, 027) &&
	   board.board[017] == empty)
		moves[count++] = _mov(047, 027);

	// Falta en passant:
	// Última posição recebe um movimento inválido para sinalizar o final da lista:
	moves[count++] = 0;
	return moves;
}

// FUNÇÃO fastMove:
inline enum piece fastMove(enum piece board[], move m)
{
	register unsigned short init = initial(m);
	register unsigned short end = final(m);
	register enum piece captured = board[end];

	board[end] = board[init];
	board[init] = empty;

	return captured;
}

// FUNÇÃO undoMov:
/*void undoMov(SearchBoard *board, move m, enum piece captured)
{
	register unsigned short end = final(m);

	// Acrescimo de material por captura:
	board->mat[!turn] += weights[captured];

	// Movimentos do rei são contabilizados na estrutura:
	if(board->board[initial(m)] == wking)
		board->kings[whites] = initial(m);
	else if(board->board[initial(m)] == bking)
		board->kings[blacks] = initial(m);

	// Removendo:
	board->board[initial(m)] = board->board[end];
	board->board[end] = captured;

	// Calculando endereço na hash:
	board->hashAddr ^= randBTS[board->board[final(m)]][final(m)] ^
	randBTS[board->board[initial(m)]][initial(m)] ^
	~ randBTS[board->board[initial(m)]][final(m)] ^ ~ randBTS[empty][initial(m)];
}*/

// FUNÇÃO mkUsrMove:
enum piece mkUsrMove(SearchBoard *board, Player turn, move m)
{
	move * moves = posMov(*board, turn);
	bool find = false;

	for(int i = 0; moves[i] != 0; i++)
		if(m == moves[i])
		{
			find = true;
			break;
		}

	if(!find)
		return invalid;

	free(moves);
	return mkCmpMove(board, turn, m);
}

//FUNÇÃO mkCmpMove: não necessita tantas checagens...
enum piece mkCmpMove(SearchBoard *board, Player turn, move m)
{
	enum piece captured;

	// Pode ser um roque branco do rei:
	if(board->board[040] == wking && m == _mov(040, 060))
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[wking][040] ^ randBTS[empty][050] ^ randBTS[empty][060] ^
		randBTS[wrook][070] ^
		randBTS[empty][040] ^ randBTS[wrook][050] ^ randBTS[wking][060] ^ randBTS[empty][070];

		// Movendo:
		board->board[040] = empty;
		board->board[050] = wrook;
		board->board[060] = wking;
		board->board[070] = empty;

		// Posição final do rei:
		board->kings[whites] = 060;

		// Não pode mais rocar:
		board->spc[whites].rkKing = board->spc[whites].rkQueen = false;

		return empty;
	}

	// Pode ser um roque preto do rei:
	else if(board->board[047] == bking && m == _mov(047, 067))
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[bking][047] ^ randBTS[empty][057] ^ randBTS[empty][067] ^
		randBTS[brook][077] ^
		randBTS[empty][047] ^ randBTS[brook][057] ^ randBTS[bking][067] ^ randBTS[empty][077];

		// Movendo:
		board->board[047] = empty;
		board->board[057] = brook;
		board->board[067] = bking;
		board->board[077] = empty;

		// Posição final do rei:
		board->kings[blacks] = 067;

		// Não pode mais rocar:
		board->spc[blacks].rkKing = board->spc[blacks].rkQueen = false;

		return empty;
	}

	// Pode ser um roque branco da dama:
	else if(board->board[040] == wking && m == _mov(040, 020))
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[wking][040] ^ randBTS[empty][030] ^ randBTS[empty][020] ^
		randBTS[wrook][000] ^
		randBTS[empty][040] ^ randBTS[wrook][030] ^ randBTS[wking][020] ^ randBTS[empty][000];

		// Movendo:
		board->board[040] = empty;
		board->board[030] = wrook;
		board->board[020] = wking;
		board->board[000] = empty;

		// Posição final do rei:
		board->kings[whites] = 020;

		// Não pode mais rocar:
		board->spc[whites].rkKing = board->spc[whites].rkQueen = false;

		return empty;
	}

	// Pode ser um roque preto da dama:
	else if(board->board[047] == bking && m == _mov(047, 027))
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[bking][047] ^ randBTS[empty][037] ^
		randBTS[empty][027] ^ randBTS[brook][007] ^
		randBTS[empty][047] ^ randBTS[brook][037] ^ randBTS[bking][027] ^ randBTS[empty][007];

		// Movendo:
		board->board[047] = empty;
		board->board[037] = brook;
		board->board[027] = bking;
		board->board[007] = empty;

		// Posição final do rei:
		board->kings[blacks] = 027;

		// Não pode mais rocar:
		board->spc[blacks].rkKing = board->spc[blacks].rkQueen = false;

		return empty;
	}

	//Promoção branca:
	else if(turn && board->board[initial(m)]==wpawn && initial(m)%8 == 6)
	{
		int a = m>>12;

		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[wpawn][initial(m)] ^ randBTS[empty][initial(m)] ^
		randBTS[board->board[final(m)]][final(m)] ^
		randBTS[*(reinterpret_cast<piece*>(&a))][final(m)];

		// Movendo:
		board->board[initial(m)] = empty;
		board->board[final(m)] = *(reinterpret_cast<piece*>(&a));
	}

	//Promoção preta:
	else if(!turn && board->board[initial(m)]==bpawn && initial(m)%8 == 1)
	{
		int a = m>>12;

		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[bpawn][initial(m)] ^ randBTS[empty][initial(m)] ^
		randBTS[board->board[final(m)]][final(m)] ^
		randBTS[*(reinterpret_cast<piece*>(&a))][final(m)];

		// Movendo:
		board->board[initial(m)] = empty;
		board->board[final(m)] = *(reinterpret_cast<piece*>(&a));
	}

	// Pode ser en passant das brancas:
	else if(turn == whites && board->board[initial(m)] == wpawn && initial(m)%8 == 4
			&& final(m) - initial(m) != 001 && board->board[final(m)] == empty &&
			board->spc[whites].enPass[final(m) >> 3])
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[bpawn][final(m) - 001] ^ randBTS[empty][final(m) - 001] ^
		randBTS[wpawn][initial(m)] ^ randBTS[empty][initial(m)] ^
		randBTS[empty][final(m)] ^ randBTS[wpawn][final(m)];

		// Movendo:
		board->board[initial(m)] = empty;
		board->board[final(m)] = wpawn;
		board->board[final(m) - 001] = empty;

		// Desconto de material por captura:
		board->mat[blacks] -= weights[bpawn];

		return bpawn;
	}

	// Pode ser en passant das pretas:
	else if(turn == blacks && board->board[initial(m)] == bpawn && initial(m)%8 == 3
			&& initial(m) - final(m) != 001 && board->board[final(m)] == empty &&
			board->spc[blacks].enPass[final(m) >> 3])
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[wpawn][final(m) + 001] ^ randBTS[empty][final(m) + 001] ^
		randBTS[bpawn][initial(m)] ^ randBTS[empty][initial(m)] ^
		randBTS[empty][final(m)] ^ randBTS[bpawn][final(m)];

		// Movendo:
		board->board[initial(m)] = empty;
		board->board[final(m)] = bpawn;
		board->board[final(m) + 001] = empty;

		// Desconto de material por captura:
		board->mat[whites] -= weights[wpawn];

		return wpawn;
	}

	else
	{
		// Calculando endereço na hash:
		board->hashAddr ^= randBTS[board->board[final(m)]][final(m)] ^
		randBTS[board->board[initial(m)]][initial(m)] ^
		randBTS[board->board[initial(m)]][final(m)] ^ randBTS[empty][initial(m)];

		// Movendo:
		if (board->kings[turn] == initial(m)) board->kings[turn]=final(m);
		captured = fastMove(board->board, m);

		// Não pode mover para posição de cheque
		if(isAttacked(board->board, (Player)!turn, board->kings[turn]))
		{
			fastMove(board->board, _mov(final(m), initial(m))); // Desfaz...
			board->board[final(m)] = captured;
			if (board->kings[turn] == final(m)) board->kings[turn]=initial(m);

			return invalid;
		}

		// Não pode mais rocar:
		if(board->spc[turn].rkKing && (turn == whites ? initial(m) == 040 || initial(m) == 070 :
									   initial(m) == 047 || initial(m) == 077))
			board->spc[turn].rkKing = false;

		if(board->spc[turn].rkQueen && (turn == whites ? initial(m) == 040 || initial(m) == 000 :
										initial(m) == 047 || initial(m) == 007))
			board->spc[turn].rkQueen = false;


		if(board->spc[turn].rkKing && (turn == whites ? (captured == wrook && final(m) == 070) :
									   (captured == brook && final(m) == 077)))
		   board->spc[turn].rkKing = false;

		if(board->spc[turn].rkQueen && (turn == whites ? (captured == wrook && final(m) == 000) :
										  (captured == brook && final(m) == 007)))
			board->spc[turn].rkQueen = false;

		// Contabiliza en passant:
		if(board->board[final(m)] == wpawn && final(m) - initial(m) == 002)
			board->spc[blacks].enPass[initial(m) >> 3] =  true;
		else if(board->board[final(m)] == bpawn && initial(m) - final(m) == 002)
			board->spc[whites].enPass[initial(m) >> 3] =  true;

		for(int i = 0; i < 8; i++)
			board->spc[turn].enPass[i] = 0;

		// Desconto de material por captura:
		board->mat[!turn] -= weights[captured];
	}
	return captured;
}

// FUNÇÕES de estado de jogo:
bool isCheck(SearchBoard board, Player turn)
{
	return isAttacked(board.board, (Player)!turn, board.kings[turn]);
}

bool isCheckmate(SearchBoard *board, Player turn)
{
	/*enum piece captured;

	if(!isAttacked(board.board, (Player)!turn, board.kings[turn]))
		return false;

	move * moves = posMov(board, turn);

	for(int i = 0; moves[i] != 0; i++)
	{
		captured = fastMove(board.board, moves[i]); // se for en passant, aqui vai dar pau...

		if(!isAttacked(board.board, (Player)!turn, board.kings[turn]))
			return false;

		fastMove(board.board, _mov(final(moves[i]), initial(moves[i]))); // Desfaz...
		board.board[final(moves[i])] = captured;
	}

	free(moves);
	return true;*/



    SearchBoard B = *board;
    if(!isAttacked(board->board, (Player)!turn, board->kings[turn]))
		return false;
    move * moves = posMov(*board, turn);
    int i=0;
    while(moves[i]!=0)
    {
        B = *board;
        if(mkCmpMove(&B,turn,moves[i])!=invalid)
        {
            free(moves);
            return false;
        }
        i++;
    }
    free(moves);
    return true;
}

bool isStalemate(SearchBoard *board, Player turn)
{
	/*enum piece captured;
	if(isAttacked(board.board, (Player)!turn, board.kings[turn]))
		return false;

	move * moves = posMov(board, turn);

	for(int i = 0; moves[i] != 0; i++)
	{
		captured = fastMove(board.board, moves[i]);

		if(!isAttacked(board.board, (Player)!turn, board.kings[turn]))
			return false;

		fastMove(board.board, _mov(final(moves[i]), initial(moves[i]))); // Desfaz...
		board.board[final(moves[i])] = captured;
	}

	free(moves);
	return true;*/

	SearchBoard B = *board;
    move * moves = posMov(*board, turn);
    int i=0;
    while(moves[i]!=0)
    {
        B = *board;
        if(mkCmpMove(&B,turn,moves[i])!=invalid)
        {
            free(moves);
            return false;
        }
        i++;
    }
    free(moves);
    if(!isAttacked(board->board, (Player)!turn, board->kings[turn]))
		return true;
    return false;
}
