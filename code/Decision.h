#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Move_List.h"

/**********************************************************************/
/**********************    ABERTURAS   ********************************/
/**********************************************************************/



class Node
{
    public:
    char name[6];
    int games;
    double WhiteWins;
    double Draw;
    double BlackWins;
    Node *filhos[50];
    inline Node()
    {
        for(int i = 0; i<50; i++)
            filhos[i] = NULL;
    }
};

typedef Node *Tree;

inline Tree NewTree()
{
    Tree resp;
    resp = (Node*)malloc(sizeof(Node));
    resp->games = -1;
    resp->filhos[0] = NULL;
    return resp;
}

Node* getRoot(Tree T);
Node** getSons(Node *N);
void AddSon(Node *N, Node *Son);
bool ChooseSon(Node** Sons,Player P,char* Name);
void MakeTreeMove(Node **N,move M);
bool ReadDatabase(Tree T, char* filename);
void ReadDatabaseRecursion(FILE *ff, Node* N);

/**********************************************************************/
/**********************    DECISAO     ********************************/
/**********************************************************************/

class Decision
{
    private:
        bool m_SeeTree;
        Player m_player;
        Tree m_T;
        Node *m_Node;
    public:
        Decision(Player P, char *DatabaseAddr = "Aberturas.txt");
        ~Decision();
        void MakeOpponentMove(move M);
        move MakeComputerMove(SearchBoard *board);
};

extern int numchamadas;
extern int numjogadas;
extern int nivelmax;
//move Cutoff_Search(Board *board, Player player, time_t time_max);
move IterativeMinMax(SearchBoard *board, Player player, int time_max);
int HeuristicLimitedSearchMax(SearchBoard *board,Player player,
                                 int HMaxLimit,const int HMinLimit,
                                 int Alpha, int Beta, int n, clock_t AcumulatedTime);
int HeuristicLimitedSearchMin(SearchBoard *board,Player player,
                                 int HMaxLimit,const int HMinLimit,
                                 int Alpha, int Beta, int n, clock_t AcumulatedTime);


