#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )
#include<time.h>

struct objeto {
	void spam(int* x,int* y) {
		srand(time(NULL));
		(*x) = 1 + rand() % 10;
		(*x) = (*x) * 60;
		(*y) = 50;
	}
	void iniciar(int, int, int, int);
	void obtenerpos(int*, int*, int*, int*);
	void mover(int,int);
	int x, y,w, h;
	int vel = 5;
	bool vivo = false;
};

struct juego {
	objeto pers, disp[50], fond, fond2, enemigo;
	void iniciar();
	void act();
	void dib();
	
	bool end = false;
	int a = 0;
	enum estado { KEY_DOWN, KEY_UP, KEY_IDLE, KEY_REPEAT };
	int teclas[250];
private:
	SDL_Event pulsa;
	SDL_Window *ventana;
	SDL_Renderer *render;
	SDL_Texture* fondo, * pato, *bala, *ene;
	Mix_Chunk * pat,*grit;
	Mix_Music* mus;
	
};

int main(int argc, char* args[]) {
	
	juego juego;
	juego.iniciar();
	
	while (juego.end == false) {
		
		juego.act();
		juego.dib();
	}


	return 0;
}

void juego::iniciar() {
	int x, y;
	Mix_Init(1);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);
	SDL_Init(SDL_INIT_EVERYTHING);
	ventana = SDL_CreateWindow("DUCKATTACK", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 625, 625, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(ventana, -1, 0);
	fondo = SDL_CreateTextureFromSurface(render, IMG_Load("fondo.jpg"));
	pato = SDL_CreateTextureFromSurface(render, IMG_Load("pato.png"));
	bala = SDL_CreateTextureFromSurface(render, IMG_Load("bala.png"));
	ene = SDL_CreateTextureFromSurface(render, IMG_Load("enemy.png"));
	fond.iniciar(0, 0, 625, 625);
	fond2.iniciar(0, -625, 625, 625);
	pers.iniciar(265, 500, 100, 100);
	enemigo.spam(&x,&y);
	enemigo.iniciar(x, y, 100, 100);
	for (int a = 0; a < 250; a++) {
		teclas[a] = KEY_IDLE;
	}
	mus = Mix_LoadMUS("music.ogg");
	Mix_PlayMusic(mus,5);
}

void juego::act() {
	int fy=0,fx=0;
	if (a == 50) {
		a = 0;
	}
	SDL_Event pulsacion;
	const Uint8* teclado = SDL_GetKeyboardState(NULL);
	SDL_PollEvent(&pulsacion);
	for (int i = 0; i < 250;i++) {
		if (teclado[i] == 1) {
			if (teclas[i] == KEY_IDLE) {
				teclas[i] = KEY_DOWN;
			}
			else {
				teclas[i] = KEY_REPEAT;
			}
		}
		else {
			if (teclas[i] == KEY_REPEAT) {
				teclas[i] = KEY_UP;
			}
			else {
				teclas[i] = KEY_IDLE;
			}
		}
	}
	if (teclas[SDL_SCANCODE_UP]==KEY_REPEAT) {
		fy = -1;
	}
	if(teclas[SDL_SCANCODE_DOWN] == KEY_REPEAT){
		fy = 1;
	}
	if (teclas[SDL_SCANCODE_RIGHT] == KEY_REPEAT) {
		fx = 1;
	}
	if (teclas[SDL_SCANCODE_LEFT] == KEY_REPEAT) {
		fx = -1;
	}
	if (teclas[SDL_SCANCODE_W] == KEY_REPEAT) {
		fy = -1;
	}
	if (teclas[SDL_SCANCODE_S] == KEY_REPEAT) {
		fy = 1;
	}
	if (teclas[SDL_SCANCODE_D] == KEY_REPEAT) {
		fx = 1;
	}
	if (teclas[SDL_SCANCODE_A] == KEY_REPEAT) {
		fx = -1;
	}
	if (teclas[SDL_SCANCODE_SPACE] == KEY_DOWN) {
		int x, y, w, h;
		
		pat=Mix_LoadWAV("quack.wav");
		Mix_PlayChannel(-1, pat, 0);
		pers.obtenerpos(&x, &y, &w, &h);
		disp[a].iniciar(x+22, y-18, 50, 60);
		a++;

	}if (teclas[SDL_SCANCODE_ESCAPE] == KEY_UP) {
		end = true;
	}
	int x, y, w, h;
	pers.mover(fy,fx);
	for (int a = 0; a < 50; a++) {
		if (disp[a].vivo == true) {
			disp[a].mover( -1,0 );
		}		
	}
	fond.mover(1, 0);
	fond2.mover(1, 0);

	if (fond.y == 625) {
		fond.y = -625;
	}
	if (fond2.y == 625) {
		fond2.y = -625;
	}
	if (pers.y>=625)
	{
		pers.y = -29;
	}
	if (pers.y <= -30)
	{
		pers.y = 624;
	}
	if (pers.x >= 625)
	{
		pers.x = -29;
	}
	if (pers.x <= -30)
	{
		pers.x = 624;
	}
	enemigo.mover(1, 0);
	if (enemigo.y >= 625)
	{
		enemigo.y = -29;
		enemigo.x = rand() % 570 + 25;
	}
	/*if (enemigo.y <= -30)
	{
		enemigo.y = 624;
		enemigo.x = rand() % 570 + 25;
	}*/
	for (int a = 0; a < 50; a++) {
		if (disp[a].y > 0&& disp[a].y < 625) {
			disp[a].vivo = true;
		}
		else {
			disp[a].vivo = false;
		}
	}
	for (int a = 0; a < 50; a++) {
		if (((((disp[a].y < enemigo.y + 100) && (disp[a].x < enemigo.x + 100)) && (disp[a].x > enemigo.x)))&&disp[a].vivo==true) {
			enemigo.y = -29;
			enemigo.x = rand() % 570 + 25;
			disp[a].vivo = false;
			grit = Mix_LoadWAV("grito.wav");
			Mix_PlayChannel(-1, grit, 0);
		}
		
	}
		
	
}
void juego::dib() {
	int a=0;
	SDL_Rect obj;
	SDL_RenderClear(render);
	fond.obtenerpos(&obj.x, &obj.y, &obj.w, &obj.h);
	SDL_RenderCopy(render, fondo, NULL, &obj);
	fond2.obtenerpos(&obj.x, &obj.y, &obj.w, &obj.h);
	SDL_RenderCopy(render, fondo, NULL, &obj);
	enemigo.obtenerpos(&obj.x, &obj.y, &obj.w, &obj.h);
	SDL_RenderCopy(render, ene, NULL, &obj);
	pers.obtenerpos(&obj.x, &obj.y, &obj.w, &obj.h);
	SDL_RenderCopy(render, pato, NULL, &obj);
	for (a = 0; a < 50; a++) {
		if (disp[a].vivo == true) {
			disp[a].obtenerpos(&obj.x, &obj.y, &obj.w, &obj.h);
			SDL_RenderCopy(render, bala, NULL, &obj);
		}		
	}
	SDL_RenderPresent(render);
	
	SDL_Delay(14);
}
void objeto::iniciar(int posx,int posy, int larx,int lary){
	x = posx;
	y = posy;
	w = larx;
	h = lary;
	vivo = true;
}
void objeto::obtenerpos(int* posx, int*posy, int*larx, int*lary) {
	*posx = x;
	*posy = y;
	*larx = w;
	*lary = h;
}
void objeto::mover(int fy,int fx) {
	y =y+( vel * fy);
	x = x + (vel * fx);
}