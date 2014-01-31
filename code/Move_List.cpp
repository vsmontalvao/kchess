#include "Heuristic.h"
#include <stdio.h>
#include <stdlib.h>
#include "Move_List.h"

#define TRUE 1
#define FALSE 0

SearchList NewEmptyListMax()
{
    SearchList resp;
    resp.MaxLevel = 0;
    resp.listtype = MAX;
    return resp;
}

SearchList NewEmptyListMin()
{
    SearchList resp;
    resp.MaxLevel = 0;
    resp.listtype = MIN;
    return resp;
}

void ConcatenateList(SearchList *L1,SearchList *L2)
{
    int i=0;
    while(i<L2->MaxLevel)
    {
        L1->list[L1->MaxLevel++] = L2->list[i++];
    }
}

#define getPieceValue(piece) (1)

void PossibleMovesListMax(SearchBoard *board, Player player, SearchList *searchlist)
{
    move* PossibleMoves = posMov(*board, player);
    enum piece backup;
    SearchBoard NewBoard;

    searchlist->listtype = MAX;

    searchlist->MaxLevel = 0;
    int i = 0;
    while(PossibleMoves[i]!=0)
    {
        NewBoard = *board;
        searchlist->list[searchlist->MaxLevel].Move = PossibleMoves[i];
        if(mkCmpMove(&NewBoard,player,searchlist->list[searchlist->MaxLevel].Move)!=invalid)
        {
            searchlist->list[searchlist->MaxLevel].h_max = getHeuristic(&NewBoard,(Player)ChangePlayer(player));
            searchlist->MaxLevel++;
        }
        i++;
    }

    free(PossibleMoves);

    OrdenateList(searchlist);
}

void PossibleMovesListMin(SearchBoard *board, Player player, SearchList *searchlist)
{
    move* PossibleMoves = posMov(*board, player);
    enum piece backup;
    SearchBoard NewBoard;

    searchlist->listtype = MIN;

    searchlist->MaxLevel = 0;
    int i = 0;
    while(PossibleMoves[i]!=0)
    {
        NewBoard = *board;
        searchlist->list[searchlist->MaxLevel].Move = PossibleMoves[i];
        if(mkCmpMove(&NewBoard,player,searchlist->list[searchlist->MaxLevel].Move)!=invalid)
        {
            searchlist->list[searchlist->MaxLevel].h_max = getHeuristic(&NewBoard,(Player)ChangePlayer(player));
            searchlist->MaxLevel++;
        }
        i++;
    }

    free(PossibleMoves);

    OrdenateList(searchlist);
}

void EnqueueList(SearchList *L, SearchMove M)
{
    L->list[L->MaxLevel] = M;
    L->MaxLevel++;
}

SearchMove DequeueList(SearchList *L)
{
    if(L->MaxLevel>0) L->MaxLevel--;
    return L->list[L->MaxLevel];
}

void AtributeList(SearchList *L1,SearchList *L2)
{
    *L1 = *L2;
}

void EmptyList(SearchList *L)
{
    L->MaxLevel = 0;
}

char IsEmpty(SearchList *L)
{
    return (L->MaxLevel<=0?TRUE:FALSE);
}

void OrdenateListRecursive(SearchList *list, int NMax, int MinLevel)
{
    SearchMove MinMove,MedMove,MaxMove;
    int MinCounter, MaxCounter;
    short MedHMax;// heuristica da mediana
    short MinHMax;// menor heuristica encontrada
    short MaxHMax;// maior heuristica encontrada

    if(NMax<=1+MinLevel) return;
    if(list->listtype==MAX)
    {
        if(NMax==2+MinLevel)
        {
            if(list->list[MinLevel].h_max<=list->list[MinLevel+1].h_max)
                return;
            else
            {
                MedMove = list->list[MinLevel];
                list->list[MinLevel] = list->list[MinLevel+1];
                list->list[MinLevel+1] = MedMove;
                return;
            }
        }
        else
        {
            MinMove = list->list[MinLevel];
            MedMove = list->list[MinLevel+((NMax-MinLevel)/2)];
            MaxMove = list->list[NMax-1];
            MinHMax = MinMove.h_max;
            MedHMax = MedMove.h_max;
            MaxHMax = MaxMove.h_max;

            if(MinHMax>MaxHMax)
            {
                MaxMove = list->list[MinLevel];
                MinMove = list->list[NMax-1];
                MinHMax = MinMove.h_max;
                MaxHMax = MaxMove.h_max;
            }
            if(MedHMax<MinHMax)
            {
                MedMove = MinMove;
                MinMove = list->list[MinLevel+((NMax-MinLevel)/2)];
                MedHMax = MedMove.h_max;
                MinHMax = MinMove.h_max;
            }
            else if(MedHMax>MaxHMax)
            {
                MedMove = MaxMove;
                MaxMove = list->list[MinLevel+((NMax-MinLevel)/2)];
                MedHMax = MedMove.h_max;
                MaxHMax = MaxMove.h_max;
            }

            if(NMax==MinLevel+3)
            {
                list->list[MinLevel]=MinMove;
                list->list[MinLevel+1]=MedMove;
                list->list[MinLevel+2]=MaxMove;
                return;
            }

            // realizando o quicksort propriamente dito

            list->list[MinLevel]=MedMove;
            list->list[MinLevel+((NMax-MinLevel)/2)]=MinMove;
            list->list[NMax-1]=MaxMove;

            MinCounter = MinLevel+1;
            MaxCounter = NMax-2;
            while(MinCounter<MaxCounter)
            {
                while(MinCounter<MaxCounter&&list->list[MinCounter].h_max<=MedHMax)
                {
                    if(list->list[MinCounter].h_max<MinHMax)
                        MinHMax = list->list[MinCounter].h_max;
                    MinCounter++;
                }
                while(MinCounter<MaxCounter&&list->list[MaxCounter].h_max>=MedHMax)
                {
                    if(list->list[MaxCounter].h_max>MaxHMax)
                        MaxHMax = list->list[MaxCounter].h_max;
                    MaxCounter--;
                }
                if(MinCounter<MaxCounter)
                {
                    MinMove = list->list[MaxCounter];
                    list->list[MaxCounter] = list->list[MinCounter];
                    list->list[MinCounter] = MinMove;
                }
            }
            if(list->list[MinCounter].h_max>MedHMax)
                MinCounter--;
            list->list[MinLevel] = list->list[MinCounter];
            list->list[MinCounter] = MedMove;
            if(MinHMax<MedHMax)
                OrdenateListRecursive(list, MinCounter, MinLevel);
            if(MaxHMax>MedHMax)
                OrdenateListRecursive(list, NMax, MinCounter+1);
        }
    }
    else
    {
        if(NMax==2+MinLevel)
        {
            if(list->list[MinLevel].h_max>=list->list[MinLevel+1].h_max)
                return;
            else
            {
                MedMove = list->list[MinLevel];
                list->list[MinLevel] = list->list[MinLevel+1];
                list->list[MinLevel+1] = MedMove;
                return;
            }
        }
        else
        {
            MaxMove = list->list[MinLevel];
            MedMove = list->list[MinLevel+((NMax-MinLevel)/2)];
            MinMove = list->list[NMax-1];
            MaxHMax = MaxMove.h_max;
            MedHMax = MedMove.h_max;
            MinHMax = MinMove.h_max;

            if(MinHMax>MaxHMax)
            {
                MaxMove = list->list[NMax-1];
                MinMove = list->list[MinLevel];
                MinHMax = MinMove.h_max;
                MaxHMax = MaxMove.h_max;
            }
            if(MedHMax<MinHMax)
            {
                MedMove = MinMove;
                MinMove = list->list[MinLevel+((NMax-MinLevel)/2)];
                MedHMax = MedMove.h_max;
                MinHMax = MinMove.h_max;
            }
            else if(MedHMax>MaxHMax)
            {
                MedMove = MaxMove;
                MaxMove = list->list[MinLevel+((NMax-MinLevel)/2)];
                MedHMax = MedMove.h_max;
                MaxHMax = MaxMove.h_max;
            }

            if(NMax==3+MinLevel)
            {
                list->list[MinLevel]=MaxMove;
                list->list[1+MinLevel]=MedMove;
                list->list[2+MinLevel]=MinMove;
                return;
            }

            // realizando o quicksort propriamente dito

            list->list[MinLevel]=MedMove;
            list->list[MinLevel+((NMax-MinLevel)/2)]=MaxMove;
            list->list[NMax-1]=MinMove;

            MaxCounter = 1+MinLevel;
            MinCounter = NMax-2;
            while(MinCounter>MaxCounter)
            {
                while(MinCounter>MaxCounter&&list->list[MinCounter].h_max<=MedHMax)
                {
                    if(list->list[MinCounter].h_max<MinHMax)
                        MinHMax = list->list[MinCounter].h_max;
                    MinCounter--;
                }
                while(MinCounter>MaxCounter&&list->list[MaxCounter].h_max>=MedHMax)
                {
                    if(list->list[MaxCounter].h_max>MaxHMax)
                        MaxHMax = list->list[MaxCounter].h_max;
                    MaxCounter++;
                }
                if(MinCounter>MaxCounter)
                {
                    MinMove = list->list[MaxCounter];
                    list->list[MaxCounter] = list->list[MinCounter];
                    list->list[MinCounter] = MinMove;
                }
            }
            if(list->list[MaxCounter].h_max<MedHMax)
                MaxCounter--;
            list->list[MinLevel] = list->list[MaxCounter];
            list->list[MaxCounter] = MedMove;
            if(MaxHMax>MedHMax)
                OrdenateListRecursive(list, MaxCounter, MinLevel);
            if(MinHMax<MedHMax)
                OrdenateListRecursive(list, NMax, MaxCounter+1);
        }
    }
}

void OrdenateList(SearchList *list)
{
    SearchMove MinMove,MedMove,MaxMove;
    int NMax = list->MaxLevel;
    int MinCounter, MaxCounter;
    short MedHMax;// heuristica da mediana
    short MinHMax;// menor heuristica encontrada
    short MaxHMax;// maior heuristica encontrada

    if(NMax<=1) return;
    if(list->listtype==MAX)
    {
        if(NMax==2)
        {
            if(list->list[0].h_max<=list->list[1].h_max)
                return;
            else
            {
                MedMove = list->list[0];
                list->list[0] = list->list[1];
                list->list[1] = MedMove;
                return;
            }
        }
        else
        {
            MinMove = list->list[0];
            MedMove = list->list[NMax/2];
            MaxMove = list->list[NMax-1];
            MinHMax = MinMove.h_max;
            MedHMax = MedMove.h_max;
            MaxHMax = MaxMove.h_max;

            if(MinHMax>MaxHMax)
            {
                MaxMove = list->list[0];
                MinMove = list->list[NMax-1];
                MinHMax = MinMove.h_max;
                MaxHMax = MaxMove.h_max;
            }
            if(MedHMax<MinHMax)
            {
                MedMove = MinMove;
                MinMove = list->list[NMax/2];
                MedHMax = MedMove.h_max;
                MinHMax = MinMove.h_max;
            }
            else if(MedHMax>MaxHMax)
            {
                MedMove = MaxMove;
                MaxMove = list->list[NMax/2];
                MedHMax = MedMove.h_max;
                MaxHMax = MaxMove.h_max;
            }

            if(NMax==3)
            {
                list->list[0]=MinMove;
                list->list[1]=MedMove;
                list->list[2]=MaxMove;
                return;
            }

            // realizando o quicksort propriamente dito

            list->list[0]=MedMove;
            list->list[NMax/2]=MinMove;
            list->list[NMax-1]=MaxMove;

            MinCounter = 1;
            MaxCounter = NMax-2;
            while(MinCounter<MaxCounter)
            {
                while(MinCounter<MaxCounter&&list->list[MinCounter].h_max<=MedHMax)
                {
                    if(list->list[MinCounter].h_max<MinHMax)
                        MinHMax = list->list[MinCounter].h_max;
                    MinCounter++;
                }
                while(MinCounter<MaxCounter&&list->list[MaxCounter].h_max>=MedHMax)
                {
                    if(list->list[MaxCounter].h_max>MaxHMax)
                        MaxHMax = list->list[MaxCounter].h_max;
                    MaxCounter--;
                }
                if(MinCounter<MaxCounter)
                {
                    MinMove = list->list[MaxCounter];
                    list->list[MaxCounter] = list->list[MinCounter];
                    list->list[MinCounter] = MinMove;
                }
            }
            if(list->list[MinCounter].h_max>MedHMax)
                MinCounter--;
            list->list[0] = list->list[MinCounter];
            list->list[MinCounter] = MedMove;
            if(MinHMax<MedHMax)
                OrdenateListRecursive(list, MinCounter, 0);
            if(MaxHMax>MedHMax)
                OrdenateListRecursive(list, NMax, MinCounter+1);
        }
    }
    else
    {
        if(NMax==2)
        {
            if(list->list[0].h_max>=list->list[1].h_max)
                return;
            else
            {
                MedMove = list->list[0];
                list->list[0] = list->list[1];
                list->list[1] = MedMove;
                return;
            }
        }
        else
        {
            MaxMove = list->list[0];
            MedMove = list->list[NMax/2];
            MinMove = list->list[NMax-1];
            MaxHMax = MaxMove.h_max;
            MedHMax = MedMove.h_max;
            MinHMax = MinMove.h_max;

            if(MinHMax>MaxHMax)
            {
                MaxMove = list->list[NMax-1];
                MinMove = list->list[0];
                MinHMax = MinMove.h_max;
                MaxHMax = MaxMove.h_max;
            }
            if(MedHMax<MinHMax)
            {
                MedMove = MinMove;
                MinMove = list->list[NMax/2];
                MedHMax = MedMove.h_max;
                MinHMax = MinMove.h_max;
            }
            else if(MedHMax>MaxHMax)
            {
                MedMove = MaxMove;
                MaxMove = list->list[NMax/2];
                MedHMax = MedMove.h_max;
                MaxHMax = MaxMove.h_max;
            }

            if(NMax==3)
            {
                list->list[0]=MaxMove;
                list->list[1]=MedMove;
                list->list[2]=MinMove;
                return;
            }

            // realizando o quicksort propriamente dito

            list->list[0]=MedMove;
            list->list[NMax/2]=MaxMove;
            list->list[NMax-1]=MinMove;

            MaxCounter = 1;
            MinCounter = NMax-2;
            while(MinCounter>MaxCounter)
            {
                while(MinCounter>MaxCounter&&list->list[MinCounter].h_max<=MedHMax)
                {
                    if(list->list[MinCounter].h_max<MinHMax)
                        MinHMax = list->list[MinCounter].h_max;
                    MinCounter--;
                }
                while(MinCounter>MaxCounter&&list->list[MaxCounter].h_max>=MedHMax)
                {
                    if(list->list[MaxCounter].h_max>MaxHMax)
                        MaxHMax = list->list[MaxCounter].h_max;
                    MaxCounter++;
                }
                if(MinCounter>MaxCounter)
                {
                    MinMove = list->list[MaxCounter];
                    list->list[MaxCounter] = list->list[MinCounter];
                    list->list[MinCounter] = MinMove;
                }
            }
            if(list->list[MaxCounter].h_max<MedHMax)
                MaxCounter--;
            list->list[0] = list->list[MaxCounter];
            list->list[MaxCounter] = MedMove;
            if(MaxHMax>MedHMax)
                OrdenateListRecursive(list, MaxCounter, 0);
            if(MinHMax<MedHMax)
                OrdenateListRecursive(list, NMax, MaxCounter+1);
        }
    }
}
