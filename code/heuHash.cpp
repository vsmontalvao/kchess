/*
 *  hash.cpp
 *  WeisserKoenig
 *
 *
 */

#include "heuHash.h" // Não tem esse arquivo!

#include "move.h"
#include <stdlib.h>

int randBTS[13][64];
hashElm hash[2][hashLen];

// Incia as strings de bites aleatórias:(conjunto deve conter uma base de Z_2^16)
// NOTA: geramos int, mas apenas os primeiros 16 bits são sgnificativos para o primeiro endereço.
void initRandBTS()
{
	srand(2008); // Funciona como boa semente, i.e., gera Z_2^16.
	for(int i = 0; i < 13; i++)
		for(int j = 0; j < 64; j++)
			randBTS[i][j] = rand();
}

// Zobrist hashing function: calcula a posição devida de elementos na hash e o segundo endereço.
int hashF(enum piece board[])
{
	register int hash = 0;

	for(int i = 0; i < 64; i++)
		hash ^= randBTS[board[i]][i];

	return hash;
}

// Coloca na hash. Somente os primeiros 16 bits são significativos.
void addToHash(SearchBoard g, Player p)
{
	/*for(int j = 0; j < 64; j++)
		hash[p][g.hashAddr].board[j] = g.board[j];*/

	hash[p][g.hashAddr & 0xFFFF].spc = g.spc[p];
	hash[p][g.hashAddr & 0xFFFF].heu = g.heu;
	hash[p][g.hashAddr & 0xFFFF].secondAddr = g.hashAddr;
}

// Otem o valor tabelado, caso exista. Retorna 0x7fff, caso contrário
short findHeu(SearchBoard g, Player p)
{
	bool nEq;
	/*// Tabuleiros iguasi?
	for(int i = 0; i < 64; i++)
		if(hash[p][g.hashAddr].board[i] != g.board[i])
		{
			nEq = true;
			break;
		}//*/ // Resolvi que não vou checa isso

	// Segundo endereço igual? Pode haver colisão, mas estou disposto a correr o risco.
	if(g.hashAddr != hash[p][g.hashAddr & 0xFFFF].secondAddr)
		return 0x7FFF; // hash miss

	// Caso sim, demais flags iguais?
	if(hash[p][g.hashAddr & 0xFFFF].spc.rkKing != g.spc[p].rkKing)
		return 0x7FFF; // hash miss
	else if(hash[p][g.hashAddr & 0xFFFF].spc.rkQueen != g.spc[p].rkQueen)
		return 0x7FFF; // hash miss
	else for(int i = 0; i < 8; i++)
		if(hash[p][g.hashAddr & 0xFFFF].spc.enPass[i] != g.spc[p].enPass[i])
			return 0x7FFF; // hash miss

	// Assim, retornamos:
	return hash[p][g.hashAddr & 0xFFFF].heu; // hash hit
}
