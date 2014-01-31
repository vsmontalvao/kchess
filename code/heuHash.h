/*
 *  hash.h
 *  WeisserKoenig
 *
 *  Created by Pedro Bittencourt Arruda on 09/10/11.
 *  Copyright 2011 ITA. All rights reserved.
 *
 */

#include "posMov.h"

extern int randBTS[13][64]; // randBTS : piece x position --> (Z_2)^20. É tipo uma função...

#define hashLen 0x10000 // Pode ser guardado em unsigned short,
					  // mas implementar em int... E ah! deve ser potência de 2... (Z_2)^16

// Define elemento da hash:
typedef struct hashElm
{
	//enum piece board[64];
	specMov spc;

	int secondAddr;

	short heu, prf;
} hashNode;

// As hashs:
extern hashElm hash[2][hashLen]; // Uma para cada cor.

void initRandBTS();
int hashF(enum piece board[]);
void addToHash(SearchBoard g, Player p);
short findHeu(SearchBoard g, Player p);
