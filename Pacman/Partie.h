#pragma once
#include"Pacman.h"
#include"Ghost.h"
#include"Space.h"
#include"Wall.h"
#include<vector>
#include"Fenetre.h"
#include"Bouton.h"

class Partie
{
private:
	std::vector< std::vector<StaticElement*>> labyrinthe;
	Fenetre* window;
	int score;
public:
	//constructeur et destructeur
	Partie(Pacman*, std::vector<Ghost*>);
	~Partie();
	//méthodes void
	void affiche(Pacman*, std::vector<Ghost*> listeF);
	void avancePac(Pacman*, Direction);
	void avanceGhost(Ghost*, Pacman* pac);
	void afficheScore();
	void GameOver(Bouton&);
	void affichePacman(Pacman*);
	void afficheGhost(Ghost*);
	//méthodes bool
	bool finpartie();
	//méthodes int
	int distanceManhattan(int, int, int, int);
};

