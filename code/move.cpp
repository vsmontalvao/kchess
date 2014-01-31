#include "move.h"
// Funções para representação em string de movimentos:

// FUNÇÃO movToStr: escreve o movimento m na string str.
void movToStr(move m, char str[])
{
	// <move> ::= <SourceCol> <SourceRow> <DestCol> <DestRow>;
	// <SourceCol> ::= 'a'...'h'; <SourceRow> ::= '1'...'8';
	// <DestCol> ::= 'a'...'h'; <DestRow> ::= '1'...'8';
	// e.g.: e2e4 : from 041 to 043.
	str[1] = '1' + ((char)m & 0x07);
	str[0] = 'a' + ((m = (m>>3)) & 0x07);
	str[3] = '1' + ((m = (m>>3)) & 0x07);
	str[2] = 'a' + ((m = (m>>3)) & 0x07);
	str[4] = '\0';
}

// FUNÇÃO strToMov: gera o movimento m correspondente à string str. Acho que está bugado...
// Eu tenho certeza :-), já corrigi --Felipe
void strToMov(move *m, const char str [])
{
	*m = 0;
	*m |= (str[2] - 'a'); *m = *m << 3;
	*m |= (str[3] - '1'); *m = *m << 3;
	*m |= (str[0] - 'a'); *m = *m << 3;
	*m |= (str[1] - '1');
}
