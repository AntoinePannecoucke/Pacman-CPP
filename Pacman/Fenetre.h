#pragma once
#include<SDL.h>
#include<SDL_ttf.h>
#include<iostream>
#include<SDL_image.h>
#include"Pacman.h"
#include"Ghost.h"
#include"StaticElement.h"
#include"config_sdl.h"
#include"Bouton.h"

class Fenetre
{
private:
	SDL_Window* win;
	SDL_Renderer* rend;
	TTF_Font* police;
	TTF_Font* titre;
	int largeur, hauteur;
	//Couleurs
	SDL_Color blanc = { 255,255,255 };
	SDL_Color jaune = { 255,255,0 };
	//méthodes privées
	void btnRestart(Bouton& btn);
	void DrawBlackRect(int const x, int const y);
public:
	//constructeur et destructeur
	Fenetre(int const l = 50, int const h = 50);
	~Fenetre();
	//getteur
	SDL_Renderer* getRend() { return rend; }
	//méthodes
	void DrawWall(int const, int const, int const);
	void DrawGum(int const, int const);
	void DrawSuperGum(int const, int const );
	void DrawPacman(int const, int const, PacmanStatus);
	void DrawGhost(int const, int const, GhostStatus, Couleur c);
	void Delete(int const, int const, StaticType);
	void showScore(int const);
	void afficheGameOver(Bouton& btn);
	
};

