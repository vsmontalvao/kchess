/*
 *  heuristics.h
 *  WeisserKoenig
 *
 *  Created by Pedro Bittencourt Arruda on 12/10/11.
 *  Copyright 2011 ITA. All rights reserved.
 *
 */

#include "move.h"
#include "heuHash.h"

extern short posWg[][64];
extern int posKgWg[][64];
// baseado nos pesos de Tomasz Michniewski
//		em http://chessprogramming.wikispaces.com/Simplified+evaluation+function

int get_heuristics(SearchBoard *board, Player p);
