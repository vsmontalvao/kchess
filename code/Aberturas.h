#include "Decision.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
bool ChooseSon(Node** Sons,char* Name);
void MakeTreeMove(Node **N,move M);
bool ReadDatabase(Tree T, char* filename);
void ReadDatabaseRecursion(FILE *ff, Node* N);
