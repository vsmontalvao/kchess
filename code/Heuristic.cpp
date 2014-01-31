#include "heuristics.h"
#include "Heuristic.h"

short getHeuristic(SearchBoard* board, Player player, int *Level)
{
    short h_max;
    short l2;
    if(false/*&&getHashValue(board, player, &h_max, &l2)*/)
    {
        if(Level!=NULL)
            *Level = (int)l2;
        return (player==whites?h_max:h_max);
    }

    h_max = /*get_heuristics(board,player);*/
             ((player)==whites?
               (board)->mat[whites]-(board)->mat[blacks]:
               (board)->mat[whites]-(board)->mat[blacks]);
               //(1+(board)->mat[whites]+(board)->mat[blacks]))
    //addToHash(board, player, h_max, 0);
    if(Level!=NULL)
        *Level = 0;
    return h_max;
}
