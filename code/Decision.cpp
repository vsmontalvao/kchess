// Notation: [Pdr] me; who else would like to comment on other's code, please write
//		"// [<abreviation>] <comment> ".

#include "heuristics.h"
#include "Heuristic.h"
#include "Decision.h"
#include <stdio.h>

#define REAL_MIN  -0x7FFF
#define REAL_MAX  0x7FFF
//int NIVEL_MAX = 4;
//int NIVEL_MIN = 4;
#define Ntest 1

int numchamadas = 0,numjogadas = 0, nivelmax = 0;

void printBoard(enum piece board[])
{
    printf("\n\n\n");
    printf("\t\t  1   2   3   4   5   6   7   8");
    printf("\n\t\t---------------------------------\n");

    for(int i = 0; i < 8; i++)
    {
        printf("\t%c\t|", i +'a');
        for(int j = 0; j < 8; j++)
            switch(board[8*i + j])
            {
            case empty:
                printf("   |");
                break;
            case wpawn:
                printf(" P |");
                break;
            case bpawn:
                printf(" p |");
                break;
            case wknight:
                printf(" N |");
                break;
            case bknight:
                printf(" n |");
                break;
            case wbishop:
                printf(" B |");
                break;
            case bbishop:
                printf(" b |");
                break;
            case wrook:
                printf(" R |");
                break;
            case brook:
                printf(" r |");
                break;
            case wqueen:
                printf(" Q |");
                break;
            case bqueen:
                printf(" q |");
                break;
            case wking:
                printf(" K |");
                break;
            case bking:
                printf(" k |");
                break;
            default:
                printf(" X |");
                break;
            }

        printf("\n\t\t---------------------------------\n");
    }

    printf("\n\n\n");
}

/*
move Cutoff_Search(SearchBoard *board, Player player, time_t time_max)
{
    SearchHeap Greedy = ordenate_heap(board,possible_moves(board,player),PosicVal);
    SearchHeap NextLevel = NewEmptyHeapMax();
    move aux;
    int i = 0;
    while(!IsEmpty(Greedy))
    {
        aux = ExtractMax(Greedy);
        InsertNode(NextLevel,aux,-Cutoff_Value(board,Change_Player(player),time_max,aux));
    }
    return ExtractMax(NextLevel);
}*/

// [Pdr] quite small names, huh?

bool TimeInterruptionLastSearch = false;
clock_t Tzero,TimeMax;
move IterativeMinMax(SearchBoard *board, Player player, int time_max)
{
    char name[6];
    int TIME_INCREASE = 1;

    SearchList Greedy = NewEmptyListMax();
    SearchList NextLevel;

    if(player==whites)
        PossibleMovesListMax(board, player, &NextLevel);
    else
        PossibleMovesListMin(board, player, &NextLevel);
    SearchMove M;
    SearchBoard NewBoard;
    enum piece backup;

    int HMaxLimit = getHeuristic(board, player);
    int Alpha = REAL_MIN;
    int Beta = REAL_MAX;
    int ContadorDoNumerodeJogadasPossiveis=0;

    Tzero = clock();
    clock_t SpentTime = 1;// para nao ter 0 no denominador
    TimeMax = time_max*CLOCKS_PER_SEC;
    TimeInterruptionLastSearch = false;
    clock_t AcumulatedTime;

    int auxiliar;
    for(int i=2; clock()-Tzero<time_max && !TimeInterruptionLastSearch /*&& i<=NIVEL_MIN*/; i++)
    {
        //NIVEL_MAX = i;
        Alpha = REAL_MIN;
        Beta = REAL_MAX;
        OrdenateList(&NextLevel);
        AtributeList(&Greedy,&NextLevel);
        EmptyList(&NextLevel);
        //M = DequeueList(&Greedy);
        ContadorDoNumerodeJogadasPossiveis = 0;
        while(!TimeInterruptionLastSearch&&clock()-Tzero<time_max&&!IsEmpty(&Greedy))
        {
            M = DequeueList(&Greedy);
            movToStr(M.Move,name);
            //printf("\n%s:",name);
            NewBoard = *board;
            if(mkCmpMove(&NewBoard,player,M.Move)!=invalid)
            {
                ContadorDoNumerodeJogadasPossiveis++;
                movToStr(M.Move,name);
                printf("\n%d : ",i);
                printf("%s: %d",name,M.h_max);
                AcumulatedTime = clock();
                if(player==whites)
                    auxiliar = HeuristicLimitedSearchMin(&NewBoard,(Player)ChangePlayer(player),HMaxLimit,i,Alpha,Beta,1,AcumulatedTime);
                else
                    auxiliar = HeuristicLimitedSearchMax(&NewBoard,(Player)ChangePlayer(player),HMaxLimit,i,Alpha,Beta,1,AcumulatedTime);
                if(!TimeInterruptionLastSearch)
                {
                    M.h_max = auxiliar;
                }
                printf(" : %d\n",M.h_max);
                //printf("%d",M.h_max);
                EnqueueList(&NextLevel,M);
            }
            else
            {
                //printf("\nfail");
            }
                //M = DequeueList(&Greedy);
        }
        if(ContadorDoNumerodeJogadasPossiveis==1)
            return DequeueList(&NextLevel).Move;
        //TIME_INCREASE = (time(NULL)-Tzero)/SpentTime;
        //SpentTime = time(NULL)-Tzero;
    }
    ConcatenateList(&Greedy,&NextLevel);
    OrdenateList(&Greedy);
    SearchMove resp, aux;
    resp = DequeueList(&Greedy);
    NewBoard = *board;
    mkCmpMove(&NewBoard,player,aux.Move);
    int contador;
    srand(time(NULL));
    SearchBoard AnotherNewBoard;
    while(!IsEmpty(&Greedy))
    {
        aux = DequeueList(&Greedy);
        AnotherNewBoard = *board;
        mkCmpMove(&AnotherNewBoard,player,aux.Move);
        if((player==whites&&aux.h_max>resp.h_max)||(player==blacks&&aux.h_max<resp.h_max))
        {
            resp = aux;
            NewBoard = *board;
            mkCmpMove(&NewBoard,player,aux.Move);
        }
        if(aux.h_max==resp.h_max)
        {
            AnotherNewBoard = *board;
            mkCmpMove(&AnotherNewBoard,player,aux.Move);
            if((player==whites&&get_heuristics(&AnotherNewBoard,player)>get_heuristics(&NewBoard,player))
               ||(player==blacks&&get_heuristics(&AnotherNewBoard,player)<get_heuristics(&NewBoard,player))
               ||rand()%50==0)
            {
                resp = aux;
                NewBoard = AnotherNewBoard;
            }
        }
    }
    return resp.Move;
}

int HeuristicLimitedSearchMax(SearchBoard *board,Player player,int HMaxZero,const int Level, int Alpha, int Beta, int n, clock_t AcumulatedTime)
{
    bool FoundOnHash;
    int HashLevel;
    int HashHeuristic;
    clock_t Defasagem = AcumulatedTime - clock();
            //printf("\nfuncionou ate aqui max\n");
            if(n>nivelmax) nivelmax = n;
    int HMaxLimit = HMaxZero - Level;
    int HMinLimit = -HMaxZero - Level;
    numchamadas++;
    SearchList Greedy;
    PossibleMovesListMax(board, player,&Greedy);
    SearchList NextLevel = NewEmptyListMax();
    SearchMove M;// = DequeueList(&Greedy);
    SearchBoard NewBoard;
    enum piece backup;
    int LastHeuristic = getHeuristic(board,player);
    while((!IsEmpty(&Greedy))/*&&(M.hmax-n>HMaxLimit)*/)
    {
            if(clock()-Tzero>TimeMax)
            {
                printf("\nacabou o tempo: %d\n",clock()-Tzero);
                TimeInterruptionLastSearch = true;
                return -1000;
            }
        numjogadas++;
        M = DequeueList(&Greedy);
        NewBoard = *board;
        if((mkCmpMove(&NewBoard,player,M.Move)!=invalid))
        {
            FoundOnHash = false;
            /*if(getHashValue(&NewBoard, (Player)ChangePlayer(player), &HashHeuristic, &HashLevel))
            {
                if(HashLevel>1+Level-n)
                {
                    M.h_max = HashHeuristic;
                    FoundOnHash = true;
                }
            }*/

            //if(n<=NIVEL_MAX&&(M.h_max-n>HMaxLimit||M.h_max!=LastHeuristc))
            if(!FoundOnHash&&/*n<=NIVEL_MAX&&(*/(n<Level||(M.h_max!=LastHeuristic)&&(n<Level+2))/*||M.h_max!=LastHeuristc)*/)
            {
                M.h_max = HeuristicLimitedSearchMin(&NewBoard,
                                                    (Player)ChangePlayer(player),
                                                    HMaxZero,
                                                    Level,
                                                    Alpha,
                                                    Beta,
                                                    n+1,
                                                    Defasagem+clock());
                if(TimeInterruptionLastSearch)
                    return -1000;
                //addToHash(&NewBoard, (Player)ChangePlayer(player), M.h_max, 1+Level-n);
                //if(n==Ntest) printf("max%d:%d",n,M.h_max);
            }
            //else printf("\n%d\n",n);
            //undoMov(board->board,M.Move,backup);
            EnqueueList(&NextLevel,M);

        }

        // AlphaBeta condition
        // Alpha is the value of the best alternative for MAX along the path to board
        // Beta is the value of the best alternative for MIN along the path to board
        if(M.h_max>Beta)
        {
            EmptyList(&Greedy);
            EmptyList(&NextLevel);
            return M.h_max;
        }
        if(M.h_max>Alpha)
            Alpha = M.h_max;

        //M = DequeueList(&Greedy);
    }
    int resp1 = REAL_MIN, resp2 = REAL_MIN;
    OrdenateList(&Greedy);
    OrdenateList(&NextLevel);
    if(IsEmpty(&Greedy   )&&IsEmpty(&NextLevel))
    {
        //printf("MAX%d:BIZU!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",n);
        return REAL_MIN+n;
    }
    if(!IsEmpty(&Greedy   )) resp1 = DequeueList(&Greedy).h_max;
    if(!IsEmpty(&NextLevel)) resp2 = DequeueList(&NextLevel).h_max;
    EmptyList(&Greedy);
    EmptyList(&NextLevel);
    //if(n==Ntest) printf("\nMAX%d:%d",n,(resp1>resp2?resp1:resp2));
    return (resp1>resp2?resp1:resp2);
}

int HeuristicLimitedSearchMin(SearchBoard *board,Player player,int HMaxZero,const int Level, int Alpha, int Beta, int n, clock_t AcumulatedTime)
{
    bool FoundOnHash;
    int HashLevel;
    int HashHeuristic;
    clock_t Defasagem = AcumulatedTime - clock();
            //printf("\nfuncionou ate aqui min\n");
            if(n>nivelmax) nivelmax = n;
    int HMaxLimit = HMaxZero - Level;
    int HMinLimit = -HMaxZero - Level;
    numchamadas++;
    SearchList Greedy;
    PossibleMovesListMin(board, player,&Greedy);
    SearchList NextLevel = NewEmptyListMin();
    SearchMove M;// = DequeueList(&Greedy);
    SearchBoard NewBoard;
    enum piece backup;
    int LastHeuristic = getHeuristic(board,player);
    //if(IsEmpty(&Greedy))
    //{printf("\n<<<<<<<<<<<<<<<<<<<<<<BUGOU MUITO BIZARRO>>>>>>>>>>>>>>>>>>>>>\n");printBoard(board->board);}
    while((!IsEmpty(&Greedy))/*&&(-M.hmax-n>HMinLimit)*/)
    {
            if(clock()-Tzero>TimeMax)
            {
                printf("\nacabou o tempo: %d\n",clock()-Tzero);
                TimeInterruptionLastSearch = true;
                return -1000;
            }
        numjogadas++;
        M = DequeueList(&Greedy);
        NewBoard = *board;
        if(mkCmpMove(&NewBoard,player,M.Move)!=invalid)
        {
            FoundOnHash = false;
            /*if(getHashValue(&NewBoard, (Player)ChangePlayer(player), &HashHeuristic, &HashLevel))
            {
                if(HashLevel>1+Level-n)
                {
                    M.h_max = HashHeuristic;
                    FoundOnHash = true;
                    printf("\n%d", HashLevel);
                }
            }*/

            //if((n<=NIVEL_MAX)&&(-M.h_max-n>HMaxLimit||-M.h_max!=LastHeuristic))
            if(!FoundOnHash&&/*n<=NIVEL_MAX&&*/(n<Level||(M.h_max!=LastHeuristic)&&(n<Level+2)))
            {
                M.h_max = HeuristicLimitedSearchMax(&NewBoard,
                                                    (Player)ChangePlayer(player),
                                                    HMaxZero,
                                                    Level,
                                                    Alpha,
                                                    Beta,
                                                    n+1,
                                                    Defasagem+clock());
                if(TimeInterruptionLastSearch)
                    return -1000;
                //addToHash(&NewBoard, (Player)ChangePlayer(player), M.h_max, 1+Level-n);
                //if(n==Ntest) printf("\nmin%d:%d",n,M.h_max);
            }
            //else printf("\n%d\n",n);
            //undoMov(board->board,M.Move,backup);
            EnqueueList(&NextLevel,M);

            // AlphaBeta condition
            // Alpha is the value of the best alternative for MAX along the path to board
            // Beta is the value of the best alternative for MIN along the path to board
            if(M.h_max<Alpha)
            {
                EmptyList(&Greedy);
                EmptyList(&NextLevel);
                return M.h_max;
            }
            if(M.h_max<Beta)
                Beta = M.h_max;
        }

        //M = DequeueList(&Greedy);
    }
    move Mov1,Mov2;
    int resp1 = REAL_MAX, resp2 = REAL_MAX;
    OrdenateList(&Greedy);
    OrdenateList(&NextLevel);
    if(IsEmpty(&Greedy   )&&IsEmpty(&NextLevel))
    {
        //printf("BIZU!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",n);
        return REAL_MAX-n;
    }
    if(!IsEmpty(&Greedy   )) resp1 = DequeueList(&Greedy).h_max;
    if(!IsEmpty(&NextLevel)) resp2 = DequeueList(&NextLevel).h_max;
    //while(!IsEmpty(&Greedy)) printf("\n%d:%d",n,DequeueList(&Greedy).h_max);
    //while(!IsEmpty(&NextLevel)) printf("\n%d:%d",n,DequeueList(&NextLevel).h_max);
    //printf("\n%d:final: %d",n,(resp1<resp2?resp1:resp2));
    //if(resp1==resp2&&resp1==REAL_MAX&&IsEmpty(&Greedy   )&&IsEmpty(&NextLevel)) return REAL_MAX-10;//printf("MIN%dBUGOU BIZARRO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",n);
    EmptyList(&Greedy);
    EmptyList(&NextLevel);
    //if(n==Ntest) printf("\nMIN%d:%d",n,(resp1<resp2?resp1:resp2));
    return (resp1<resp2?resp1:resp2);
}

/***********************************************************/
/**********************  CLASSE DECISAO ********************/
/***********************************************************/
Decision::Decision(Player P, char *DatabaseAddr)
{
    m_SeeTree = true;
    m_T = NewTree();
    m_Node = m_T;
    m_player = P;
    if(ReadDatabase(m_T,DatabaseAddr)==false)
    {
        printf("\no arquivo não existe\n");
    }
}

Decision::~Decision()
{

}

void Decision::MakeOpponentMove(move M)
{
    if(m_SeeTree&&m_Node!=NULL)
        MakeTreeMove(&m_Node,M);
}

move Decision::MakeComputerMove(SearchBoard *board)
{

            char name[6];
            SearchMove M;
            if(m_SeeTree&&ChooseSon(getSons(m_Node), m_player, name))
            {
                printf("\n%s\n",name);
                strToMov(&(M.Move),name);
                MakeTreeMove(&m_Node,M.Move);
                return M.Move;
            }
            else
                m_SeeTree = false;
            if(!m_SeeTree)
            {
                M.Move = IterativeMinMax(board, m_player, 5000000);
                return M.Move;
            }
}
