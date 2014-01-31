#include "Decision.h"


Node* getRoot(Tree T)
{
    return T;
}

Node** getSons(Node *N)
{
    if(N==NULL) return NULL;
    return N->filhos;
}

void AddSon(Node *N, Node *Son)
{
    int i = 0;
    while(N->filhos[i]!=NULL) i++;
    N->filhos[i] = Son;
    N->filhos[i+1] = NULL;
}

bool ChooseSon(Node** Sons,Player P,char* Name)
{
    if(Sons==NULL||Sons[0]==NULL) return false;
    int Max = 0, i = 0;
    while(Sons[i]!=0)
    {
        if(Sons[Max]->games<Sons[i]->games)
            Max = i;
        i++;
    }
    strcpy(Name,Sons[Max]->name);
    return true;
}

void MakeTreeMove(Node **N,move M)
{
    int i=0;
    if((*N)->filhos[0]==NULL)
    {
        *N = NULL;
        return;
    }
    while((*N)->filhos[i]!=NULL)
    {
        char name[6];
        movToStr(M,name);
        if(strcmp((*N)->filhos[i]->name, name)==0)
        {
            *N = (*N)->filhos[i];
            //printf("bizu");
            return;
        }
        //printf("\n%s : %s\n",(*N)->filhos[i]->name, name);
        i++;
    }
    *N = NULL;
    //printf("\nblablabla\n");
}

bool ReadDatabase(Tree T, char* filename)
{
    FILE *ff = fopen(filename,"r");
    char c;
    if(ff==NULL) return false;
    fscanf(ff," %c ",&c);
    fscanf(ff," %c ",&c);
    int i = 0;
    while(c=='(')
    {
        T->filhos[i] = (Node*)malloc(sizeof(Node));
        ReadDatabaseRecursion(ff,T->filhos[i]);
        i++;
        fscanf(ff," %c ",&c);
    }
    T->filhos[i]=NULL;
    return true;
}

void ReadDatabaseRecursion(FILE *ff, Node* N)
{
    char c;
    int i = 0;
    fscanf(ff," %s ",N->name);
    N->name[4]='\0';
    //printf("\n%s : ",N->name);
    fscanf(ff," %d%c %f%c %f%c %f%c %c ",&(N->games),&c,&(N->WhiteWins),&c,&(N->Draw),&c,&(N->BlackWins),&c,&c);
    //printf("funcionou ate aqui : %c\n",c);
    fscanf(ff," %c ",&c);
    while(c=='(')
    {
        N->filhos[i] = (Node*)malloc(sizeof(Node));
        ReadDatabaseRecursion(ff,N->filhos[i]);
        i++;
        fscanf(ff," %c ",&c);
    }
    //fscanf(ff," %c ",&c);
    //if(i>0) fscanf(ff," %c ",&c);
    //printf("\n %c\n",c);
    N->filhos[i] = NULL;
}
