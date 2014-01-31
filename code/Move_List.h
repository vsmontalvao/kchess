#define ChangePlayer(player) (!(player)) // [Pdr] Do you mean 'ChangePlayer'?
#include "posMov.h"

typedef struct SearchMove
{
    move Move;
    int h_max;
} SearchMove;

typedef enum ListType {MAX,MIN} ListType;

typedef struct SearchList
{
    /*
        Cada piao   pode fazer no maximo 28 jogadas diferentes(supondo que tenha virado uma rainha)
        Cada torre  pode fazer no maximo 14 jogadas diferentes
        Cada cavalo pode fazer no maximo  8 jogadas diferentes
        Cada bispo  pode fazer no maximo 14 jogadas diferentes
        Cada rainha pode fazer no maximo 28 jogadas diferentes
        Cada rei    pode fazer no maximo  9 jogadas diferentes(se puder fazer roque ele nap pode ir para traz)

        Logo o numero maximo de jogadas possiveis eh 333
    */
    SearchMove list[350]; // um pouco maior que 333 (caso eu tenha errado alguma conta)
    int MaxLevel;
    ListType listtype;
} SearchList;

SearchList NewEmptyListMax();
SearchList NewEmptyListMin();
void PossibleMovesListMax(SearchBoard *board, Player player, SearchList *searchlist);
void PossibleMovesListMin(SearchBoard *board, Player player, SearchList *searchlist);
void ConcatenateList(SearchList *L1,SearchList *L2);

char       IsEmpty(SearchList *L);
SearchMove DequeueList(SearchList *L);
void       EnqueueList(SearchList *L, SearchMove M);
void       OrdenateList(SearchList *list);
void       AtributeList(SearchList *L1,SearchList *L2);
void       EmptyList(SearchList *L);
