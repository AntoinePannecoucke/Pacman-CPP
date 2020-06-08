#include "Fenetre.h"
#include <sstream>
using namespace std;

Fenetre::Fenetre(int const l, int const h) : largeur(l), hauteur(h), police(nullptr), win(nullptr), rend(nullptr) {
	//Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
	}
	TTF_Init();
	//Création de la fenetre
	win = SDL_CreateWindow("Libre de droit",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		largeur,
		hauteur,
		SDL_WINDOW_SHOWN);
	if (win == NULL)
		cout << "erreur ouverture fenetre";
	//Création du renderer de la fenetre
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	police = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	titre = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 60);
}

Fenetre::~Fenetre() {
	//Destruction SDL
	TTF_CloseFont(police);
	TTF_Quit();
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

//Dessin de mur sur l'écran
void Fenetre::DrawWall(int const x, int const y, int const w) {
	SDL_SetRenderDrawColor(rend, 0, 0, 150, 255);
	SDL_Rect square;
	square.x = x;
	square.y = y;
	square.h = square.w = w;
	SDL_RenderFillRect(rend, &square);
	SDL_RenderPresent(rend);
}

//Dessin de gum sur l'écran
void Fenetre::DrawGum(int const x, int const y) {
	SDL_SetRenderDrawColor(rend, 239, 155, 15, 255);
	SDL_Rect square;
	square.x = x + 12;
	square.y = y + 12;
	square.h = square.w = 6;
	SDL_RenderFillRect(rend, &square);
	SDL_RenderPresent(rend);
}

//Dessin de Super-gum sur l'écran
void Fenetre::DrawSuperGum(int const x, int const y) {
	SDL_SetRenderDrawColor(rend, 239, 155, 15, 255);
	SDL_Rect square;
	square.x = x + 10;
	square.y = y + 10;
	square.h = square.w = 10;
	SDL_RenderFillRect(rend, &square);
	SDL_RenderPresent(rend);
}

//Dessin de pacman sur l'écran
void Fenetre::DrawPacman(int const x, int const y, PacmanStatus ps) {
	SDL_Texture* pacman = loadImage(rend, "pacman.png", 255, 255, 255, 255);
	SDL_Texture* superpac = loadImage(rend, "superpacman.png", 255, 255, 255, 255);
	SDL_Rect square;
	square.x = x + 2;
	square.y = y + 2;
	square.h = square.w = 26;
	switch (ps)
	{
	case PacmanStatus::NORMAL:
		SDL_QueryTexture(pacman, NULL, NULL, &square.w, &square.h);
		SDL_RenderCopy(rend, pacman, NULL, &square);
		break;
	case PacmanStatus::SUPER:
		SDL_QueryTexture(superpac, NULL, NULL, &square.w, &square.h);
		SDL_RenderCopy(rend, superpac, NULL, &square);
	default:
		break;
	}
	SDL_RenderPresent(rend);
	SDL_DestroyTexture(pacman);
	SDL_DestroyTexture(superpac);
}

//Dessin de Fantôme sur l'écran
void Fenetre::DrawGhost(int const x, int const y, GhostStatus gs, Couleur c) {
	SDL_Texture* eyes = loadImage(rend, "Eye.png", 255, 255 / 2, 255 / 2, 255 / 2);
	SDL_Rect square;
	square.x = x + 2;
	square.y = y + 2;
	square.h = square.w = 26;
	switch (gs)
	{
	case GhostStatus::TRACK:
		switch (c)
		{
		case Couleur::BLANC:
			SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
			break;
		case Couleur::BLEU:
			SDL_SetRenderDrawColor(rend, 44, 117, 255, 255);
			break;
		case Couleur::ROUGE:
			SDL_SetRenderDrawColor(rend, 247, 35, 12, 255);
			break;
		case Couleur::ROSE:
			SDL_SetRenderDrawColor(rend, 253, 108, 158, 255);
			break;
		case Couleur::ORANGE:
			SDL_SetRenderDrawColor(rend, 255, 127, 0, 255);
			break;
		}
		break;
	case GhostStatus::EYE:
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);;
		break;
	case GhostStatus::FLEE:
		SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
		break;
	default:
		break;
	}
	SDL_RenderFillRect(rend, &square);
	SDL_QueryTexture(eyes, NULL, NULL, &square.w, &square.h);
	SDL_RenderCopy(rend, eyes, NULL, &square);
	SDL_RenderPresent(rend);
	SDL_DestroyTexture(eyes);
}

//Juste un carré noir
void Fenetre::DrawBlackRect(int const x, int const y) {
	SDL_Rect carre;
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	carre.x = x + 2;
	carre.y = y + 2;
	carre.h = carre.w = 26;
	SDL_RenderFillRect(rend, &carre);
	SDL_RenderPresent(rend);
 }

//Effacement de l'entité mobile puis réaffichage de la case statique
void Fenetre::Delete(int const x, int const y, StaticType t) {
	DrawBlackRect(x, y);
	switch (t)
	{
	case StaticType::GUM:
		DrawGum(x, y);
		break;
	case StaticType::SUPERGUM:
		DrawSuperGum(x, y);
		break;
	default:
		break;
	}
}

//Permet d'afficher le score
void Fenetre::showScore(int const score) {
	SDL_Rect positionTexte;
	positionTexte.x = 10;
	positionTexte.y = hauteur / 2 - 50;
	positionTexte.h = 30;
	positionTexte.w = 170;
	SDL_SetRenderDrawColor(rend, 0, 0, 150, 255);
	SDL_RenderFillRect(rend, &positionTexte);
	ostringstream strs;
	strs << "Score : "<< score;
	SDL_Texture* texture = loadText(rend, strs.str().c_str(), blanc, police);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
	SDL_RenderCopy(rend, texture, NULL, &positionTexte);
	SDL_RenderPresent(rend);
	SDL_DestroyTexture(texture);
}

//Affiche le game over et le bouton restart
void Fenetre::afficheGameOver(Bouton& btn) {
	SDL_Rect positionTexte;
	positionTexte.x = largeur / 3;
	positionTexte.y = hauteur / 2 - 50;
	positionTexte.h = 30;
	positionTexte.w = 170;
	SDL_Texture* texture = loadText(rend, "Game Over", jaune, titre);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
	SDL_RenderCopy(rend, texture, NULL, &positionTexte);
	SDL_RenderPresent(rend);
	SDL_DestroyTexture(texture);
	btnRestart(btn);
}

//Affiche le bouton restart et donne les coordonnées à un Bouton
void Fenetre::btnRestart(Bouton& btn) {
	SDL_Rect positionTexte;
	positionTexte.x = largeur / 3 + 90;
	btn.setX(positionTexte.x);
	positionTexte.y = hauteur / 2 + 40;
	btn.setY(positionTexte.y);
	positionTexte.h = 30;
	btn.setH(positionTexte.h);
	positionTexte.w = 100;
	btn.setW(positionTexte.w);
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderFillRect(rend, &positionTexte);
	positionTexte.x += 10;
	positionTexte.y += 2;
	SDL_Texture* texture = loadText(rend, "Rejouer", blanc, police);
	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
	SDL_RenderCopy(rend, texture, NULL, &positionTexte);
	SDL_RenderPresent(rend);
	SDL_DestroyTexture(texture);
}