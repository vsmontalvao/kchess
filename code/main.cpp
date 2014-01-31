#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <assert.h>
#include "posMov.h"
#include "Decision.h"

const int WIDTH = 538;
const int HEIGHT = 538;

//int randBTS[13][64]={0};
SDL_Surface *whitePromo, *blackPromo;
SDL_Surface *lightSelected, *darkSelected;
SDL_Surface *selectScreen;
SDL_Surface *whiteWin,*blackWin,*draw;
int selX,selY;
bool acceptingInput;


Player user, opponent;
Decision* computer;

class Board{
	public:
	piece m[8][8];
};

void init(SearchBoard* b){
	for (int i=0; i<64; i++)b->board[i]=start_pos[i];
	b->spc[0].rkKing=true;
	b->spc[0].rkQueen=true;
	b->spc[1].rkKing=true;
	b->spc[1].rkQueen=true;
	for (int i=0; i<8; i++){
		b->spc[0].enPass[i]=false;
		b->spc[1].enPass[i]=false;
	}
	b->kings[whites] = 040;
	b->kings[blacks] = 047;
}

/*int main(){
	SearchBoard* b;
	b=(SearchBoard*)malloc(sizeof(SearchBoard));
	init(b);
	for (int i=7; i>=0; i--){
		for (int j=0; j<8; j++) printf("%d ",b->board[8*j+i]);
		printf("\n");
	}
	move m;
	char str[10];
	int k=1;
	while (true){
		scanf(" %s",str);
		strToMov(&m,str);
		if (k&1) mkUsrMove(b,whites,m);
		else mkUsrMove(b,blacks,m);
		printf("\n");
		for (int i=7; i>=0; i--){
			for (int j=0; j<8; j++) printf("%d ",b->board[8*j+i]);
			printf("\n");
		}
		k++;
	}
	return 0;
}
*/

SDL_Surface* loadImage(const char* filename){
	SDL_Surface* rawImage = IMG_Load(filename);
	SDL_Surface* img = SDL_DisplayFormatAlpha(rawImage);
	SDL_FreeSurface(rawImage);
	return img;
}

void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( source, NULL, destination, &offset );
}

void loadPieceImages(SDL_Surface** v){
	v[wking]=loadImage("wking.png");
	v[wqueen]=loadImage("wqueen.png");
	v[wrook]=loadImage("wrook.png");
	v[wbishop]=loadImage("wbishop.png");
	v[wknight]=loadImage("wknight.png");
	v[wpawn]=loadImage("wpawn.png");
	v[bking]=loadImage("bking.png");
	v[bqueen]=loadImage("bqueen.png");
	v[brook]=loadImage("brook.png");
	v[bbishop]=loadImage("bbishop.png");
	v[bknight]=loadImage("bknight.png");
	v[bpawn]=loadImage("bpawn.png");
}

void loadPromoScreens(){
	whitePromo=loadImage("whitePromo.png");
	blackPromo=loadImage("blackPromo.png");
}

void loadSelectedImages(){
	darkSelected=loadImage("darkSelected.jpg");
	lightSelected=loadImage("lightSelected.jpg");
}

void loadSelectImage(){
	selectScreen = loadImage("select.png");
}

void loadEndImages(){
	whiteWin = loadImage("whiteWin.png");
	blackWin = loadImage("blackWin.png");
	draw	 = loadImage("draw.png");
}

void drawBoard(SearchBoard* b,SDL_Surface* screen, SDL_Surface* boardImage, SDL_Surface** pieceImage){
	applySurface(0,0,boardImage,screen);
	if (selX!=-1 && selY!=-1){
		if (user==whites){
			if ((selX+selY)%2) applySurface(13+64*selX,13+64*(7-selY),lightSelected,screen);
			else applySurface(13+64*selX,13+64*(7-selY),darkSelected,screen);
		}
		else {
			if ((selX+selY)%2) applySurface(13+64*(7-selX),13+64*(selY),lightSelected,screen);
			else applySurface(13+64*(7-selX),13+64*(selY),darkSelected,screen);
		}
	}
	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			if (b->board[8*j+i] && user == whites) applySurface(13+64*j,13+64*(7-i),pieceImage[b->board[8*j+i]],screen);
			else if (b->board[8*j+i] && user == blacks) applySurface(13+64*(7-j),13+64*i,pieceImage[b->board[8*j+i]],screen);
		}
	}
	SDL_Flip(screen);
}

Player getUserSide(SDL_Surface* screen){
	applySurface(181,221,selectScreen,screen);
	SDL_Flip(screen);
	bool selected=false;
	SDL_Event event;
	int choice;
	while (!selected){
		while(SDL_PollEvent(&event)){
			if (event.type==SDL_QUIT) exit(0);
			if(event.type==SDL_MOUSEBUTTONDOWN) { 					 
				int x = event.button.x;
				int y = event.button.y;
				choice=-1;
				if (y>=221 && y<=317){
					if (x>=197 && x<=261) choice=1;
					else if (x>=277 && x<=341) choice=0;
				}
				if (choice!=-1) selected=true; 					
			}
		}
		SDL_Delay(0);
	}
	return choice?whites:blacks;
}

void changeActivePlayer(Player* p){
	if (*p == whites) *p = blacks;
	else *p = whites;
}

void makeMove(SearchBoard* b, Player* player, int rowSource, int rankSource, int rowDest, int rankDest, int piece, SDL_Surface* screen,
			  SDL_Surface* boardImage, SDL_Surface** pieceImage){
	move m = _mov((rowSource<<3)+rankSource,(rowDest<<3)+rankDest);
	m+=piece<<12;
	if (*player && b->board[initial(m)]==wpawn && initial(m)%8==6 && m>>12==0){
		applySurface(101,221,whitePromo,screen);
		SDL_Flip(screen);
		bool selected=false;
		SDL_Event event;
		while (!selected){
			while(SDL_PollEvent(&event)){
				if (event.type==SDL_QUIT) exit(0);
				if(event.type==SDL_MOUSEBUTTONDOWN) { 					 
					int x = event.button.x;
					int y = event.button.y;
					int piece=-1;
					if (y>=221 && y<=317){
						if (x>=117 && x<=181) piece=wqueen;
						else if (x>=197 && x<=261) piece=wrook;
						else if (x>=277 && x<=341) piece=wbishop;
						else if (x>=357 && x<=421) piece=wknight;
					}
					if (piece!=-1) selected=true;
					m+=piece<<12; 					
				}
			}
			SDL_Delay(0);
		}
	}
	else if (!(*player) && b->board[initial(m)]==bpawn && initial(m)%8==1 && m>>12==0){
		applySurface(101,221,blackPromo,screen);
		SDL_Flip(screen);
		bool selected=false;
		SDL_Event event;
		while (!selected){
			while(SDL_PollEvent(&event)){
				if (event.type==SDL_QUIT) exit(0);
				if(event.type==SDL_MOUSEBUTTONDOWN) { 					 
					int x = event.button.x;
					int y = event.button.y;
					int piece=-1;
					if (y>=221 && y<=317){
						if (x>=117 && x<=181) piece=bqueen;
						else if (x>=197 && x<=261) piece=brook;
						else if (x>=277 && x<=341) piece=bbishop;
						else if (x>=357 && x<=421) piece=bknight;
					}
					if (piece!=-1) selected=true;
					m+=piece<<12; 					
				}
			}
			SDL_Delay(0);
		}
	}
	if (mkUsrMove(b,*player,m)!=invalid){		
		if (*player == user) computer->MakeOpponentMove(m);
		changeActivePlayer(player);
	}
	drawBoard(b,screen,boardImage,pieceImage);
}

move makeComputerMove(SearchBoard* b, Player* player, SDL_Surface* screen, SDL_Surface* boardImage, SDL_Surface** pieceImage){
	return computer->MakeComputerMove(b);	
}

void checkGameEnd(SearchBoard* b, Player player, SDL_Surface* screen){
	if (isCheckmate(b,blacks)){
		applySurface(101,221,whiteWin,screen);
		acceptingInput=false;
		SDL_Flip(screen);
	}
	else if (isCheckmate(b,whites)){
		applySurface(101,221,blackWin,screen);
		acceptingInput=false;
		SDL_Flip(screen);
	}
	else if (isStalemate(b,player)){
		applySurface(101,221,draw,screen);
		acceptingInput=false;
		SDL_Flip(screen);
	}
}

int main(){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_SWSURFACE );
	SDL_Surface* boardImage = loadImage("chessboard.jpg");
	SDL_Surface* pieceImage[20];
	SearchBoard* b;
	b=(SearchBoard*)malloc(sizeof(SearchBoard));
	init(b);
	loadPieceImages(pieceImage);
	loadPromoScreens();
	loadSelectedImages();
	loadSelectImage();
	loadEndImages();
	selX=selY=-1;
	user = whites;
	drawBoard(b,screen,boardImage,pieceImage);
	bool quit=false;
	bool selectingSource=true;
	acceptingInput=true;
	SDL_Event event;
	int rankSource,rowSource,rankDest,rowDest;
	Player player = whites;
	
	user = getUserSide(screen);
	
	if (user == whites) opponent = blacks;
	else opponent = whites;
	
	computer = new Decision(opponent);
	
	drawBoard(b,screen,boardImage,pieceImage);
	
	while (!quit){
		checkGameEnd(b,player,screen);
		if (player==opponent){
			move m = makeComputerMove(b,&player,screen,boardImage,pieceImage);
			makeMove(b,&player,(m>>3)&7,(m)&7,(m>>9)&7,(m>>6)&7,m>>12,screen,boardImage,pieceImage);
		}
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT) quit = true;
			if(event.type==SDL_MOUSEBUTTONDOWN && acceptingInput) {
				if(event.button.button==SDL_BUTTON_LEFT){ 
					int x = event.button.x;
					int y = event.button.y;
					if (selectingSource){
						if (user==whites){
							selY = rankSource = 7-(y-13)/64;
							selX = rowSource = (x-13)/64;
						}
						else {
							selY = rankSource = (y-13)/64;
							selX = rowSource = 7-(x-13)/64;
						}
						drawBoard(b,screen,boardImage,pieceImage);
						selectingSource=false;
					}
					else {
						if (user==whites){
							selY = rankDest = 7-(y-13)/64;
							selX = rowDest = (x-13)/64;					
						}
						else {
							selY = rankDest = (y-13)/64;
							selX = rowDest = 7-(x-13)/64;
						}
						selectingSource=true;
						selX=selY=-1;
						makeMove(b,&player,rowSource,rankSource,rowDest,rankDest,empty,screen,boardImage,pieceImage);
					}
				}
			}
		}
		SDL_Delay(0);
	}
	return 0;
}
