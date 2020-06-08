#include "Partie.h"
#include <fstream>
using namespace std;

Partie::Partie(Pacman* pac, vector<Ghost*> listeF) : score(0) {
	ifstream lecture("labyrinthe.txt");
	try {
		char* tmp;
		int nbligne = -1 , nbcolonne, nombre_ghost = listeF.size();
		//lis le nombre de colonne
		lecture >> nbcolonne;
		//Permet d'ingnorer la ligne lue juste au dessus
		tmp = new char[5];
		lecture.getline(tmp, '\n');
		delete[] tmp;
		Space* pts;
		Wall* ptw;
		while (!lecture.eof()) {
			nbligne++;
			tmp = new char[nbcolonne + 1];
			lecture.getline(tmp, (__int64)nbcolonne + 1, '\n');
			vector<StaticElement*> row;
			for (int i = 0; i < nbcolonne + 1; i++) {
				switch (tmp[i])
				{
				case 'w':
					ptw = new Wall(StaticType::WALL);
					row.push_back(ptw);
					break;
				case 's':
					pts = new Space(StaticType::GUM);
					pts->setType(StaticType::GUM);
					row.push_back(pts);
					break;
				case 'S':
					pts = new Space(StaticType::START);
					pts->setPac(pac, score);
					pac->setX(i);
					pac->setY(nbligne);
					row.push_back(pts);
					break;
				case 'G':
					pts = new Space(StaticType::SUPERGUM);
					row.push_back(pts);
					break;
				case 'c':
					pts = new Space(StaticType::CIMETIERE);
					if (nombre_ghost > 0) {
						pts->setGhost(listeF[nombre_ghost-1]);
						listeF[nombre_ghost-1]->setX(i);
						listeF[nombre_ghost-1]->setY(nbligne);
						listeF[nombre_ghost - 1]->setStartX(i);
						listeF[nombre_ghost - 1]->setStartY(nbligne);
						nombre_ghost--;
					}
					row.push_back(pts);
					break;
				case 'e':
					pts = new Space(StaticType::EMPTY);
					row.push_back(pts);
					break;
				default:
					break;
				}
			}
			labyrinthe.push_back(row);
			delete[] tmp;
		}
		lecture.close();
		//crée une fenêtre au dimension du labyrinthe
		window = new Fenetre(30 * (nbcolonne), 30 * labyrinthe.size());
	}
	catch (const std::exception&) {
		cerr << "Erreur de lecture";
	}
	
}

Partie::~Partie() {
	//Vide le labyrinthe
	for (unsigned int i = 0; i < labyrinthe.size(); i++) {
		for (unsigned int j = 0; j < labyrinthe[i].size(); j++) {
			if (labyrinthe[i][j] != nullptr) {
				delete labyrinthe[i][j];
			}
		}
	}
	if (window != nullptr) {
		delete window;
	}
}

//Permet d'afficher le labyrinthe au début de la partie
void Partie::affiche(Pacman* pac, vector<Ghost*> listeF) {
	for (unsigned int i = 0; i < labyrinthe.size(); i++) {
		for (unsigned int j = 0; j < labyrinthe[i].size(); j++)
		{
			if (labyrinthe[i][j]->getType() == StaticType::WALL) {
				window->DrawWall(j * 30, i * 30, 30);
			}
			else if (labyrinthe[i][j]->getType() == StaticType::GUM) {
				window->DrawGum(j * 30, i * 30);
			}
			else if (labyrinthe[i][j]->getType() == StaticType::SUPERGUM) {
				window->DrawSuperGum(j * 30, i * 30);
			}
		}
	}
	window->DrawPacman(pac->getX() * 30, pac->getY() * 30, pac->getStatus());
	int taille = listeF.size();
	for (int i = 0; i < taille; i++) {
		window->DrawGhost(listeF[i]->getX() * 30, listeF[i]->getY() * 30, listeF[i]->getStatus(), listeF[i]->getCouleur());
	}
	afficheScore();
}

//On vérifie si la case suivante est un mur ou non est déplace pacman en fonction
void Partie::avancePac(Pacman* pac, Direction dir) {
	int X = pac->getX(), Y = pac->getY();
	switch (dir)
	{
	case Direction::NORTH:
		if (labyrinthe[Y - 1][X]->getType() != StaticType::WALL) {
			pac->setDir(dir);
			labyrinthe[Y - 1][X]->setPac(labyrinthe[Y][X]->getPac(), score);
			labyrinthe[Y][X]->setPac(nullptr, score);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			pac->setY(Y - 1);
		}
		break;
	case Direction::SOUTH:
		if (labyrinthe[Y + 1][X]->getType() != StaticType::WALL) {
			pac->setDir(dir);
			labyrinthe[Y + 1][X]->setPac(labyrinthe[Y][X]->getPac(), score);
			labyrinthe[Y][X]->setPac(nullptr, score);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			pac->setY(Y + 1);
		}
		break;
	case Direction::EAST:
		if (labyrinthe[Y][X + 1]->getType() != StaticType::WALL) {
			pac->setDir(dir);
			labyrinthe[Y][X + 1]->setPac(labyrinthe[Y][X]->getPac(), score);
			labyrinthe[Y][X]->setPac(nullptr, score);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			pac->setX(X + 1);
		}
		break;
	case Direction::WEST:
		if (labyrinthe[Y][X - 1]->getType() != StaticType::WALL) {
			pac->setDir(dir);
			labyrinthe[Y][X - 1]->setPac(labyrinthe[Y][X]->getPac(), score);
			labyrinthe[Y][X]->setPac(nullptr, score);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			pac->setX(X - 1);
		}
		break;
	default:
		break;
	}
	window->DrawPacman(pac->getX() * 30, pac->getY() * 30, pac->getStatus());
}

void Partie::avanceGhost(Ghost* g, Pacman* pac) {
	if (!g->getCheckTime()) {
		int tab[4];
		int X = g->getX(), Y = g->getY(), dirX, dirY;
		//selection de la cible selon l'état du fantôme
		if (g->getStatus() != GhostStatus::EYE) {
			//Ici c'est le positionnement de pac-man qui nous intèresse
			dirX = pac->getX();
			dirY = pac->getY();
		}
		else {
			//Ici le but est de retourner au cimetière
			dirX = g->getStartX();
			dirY = g->getStartY();
		}
		//On cherche la case qui est la plus proche de pacman en évitant de retourner sur d'ou on vient
		if (labyrinthe[Y + 1][X]->getType() != StaticType::WALL && (Y + 1 != g->getLastY() || X != g->getLastX())) {
			tab[0] = distanceManhattan(Y + 1, X, dirY, dirX);
		}
		else
		{
			tab[0] = -1;
		}
		if (labyrinthe[Y - 1][X]->getType() != StaticType::WALL && (Y - 1 != g->getLastY() || X != g->getLastX())) {
			tab[1] = distanceManhattan(Y - 1, X, dirY, dirX);
		}
		else
		{
			tab[1] = -1;
		}
		if (labyrinthe[Y][X + 1]->getType() != StaticType::WALL && (Y != g->getLastY() || X + 1 != g->getLastX())) {
			tab[2] = distanceManhattan(Y, X + 1, dirY, dirX);
		}
		else
		{
			tab[2] = -1;
		}
		if (labyrinthe[Y][X - 1]->getType() != StaticType::WALL && (Y != g->getLastY() || X - 1 != g->getLastX())) {
			tab[3] = distanceManhattan(Y, X - 1, dirY, dirX);
		}
		else
		{
			tab[3] = -1;
		}
		//On cherche une case qui n'est pas un mur
		int i = 0;
		while (tab[i] < 0 && i < 4) {
			i++;
		}
		//on cherche celle qui est la plus proche
		int min = tab[i];
		for (int j = i + 1; j < 4; j++) {
			switch (g->getStatus()) {
				//On cherche à se rapprocher de pacman
			case GhostStatus::TRACK:
				if (tab[j] < min && tab[j] > -1) {
					i = j;
					min = tab[j];
				}
				break;
				//On cherche à se rapprocher du cimetère
			case GhostStatus::EYE:
				if (tab[j] < min && tab[j] > -1) {
					i = j;
					min = tab[j];
				}
				break;
				//On fuit pacman
			case GhostStatus::FLEE:
				if (tab[j] > min&& tab[j] > -1) {
					i = j;
					min = tab[j];
				}
				break;
			}
		}
		//On avance en fonction de la case choisie
		switch (i)
		{
		case 0:
			labyrinthe[Y + 1][X]->setGhost(g);
			labyrinthe[Y][X]->setGhost(nullptr);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			g->setLastY(Y);
			g->setLastX(X);
			g->setY(Y + 1);
			break;
		case 1:
			labyrinthe[Y - 1][X]->setGhost(g);
			labyrinthe[Y][X]->setGhost(nullptr);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			g->setLastY(Y);
			g->setLastX(X);
			g->setY(Y - 1);
			break;
		case 2:
			labyrinthe[Y][X + 1]->setGhost(g);
			labyrinthe[Y][X]->setGhost(nullptr);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			g->setLastY(Y);
			g->setLastX(X);
			g->setX(X + 1);
			break;
		case 3:
			labyrinthe[Y][X - 1]->setGhost(g);
			labyrinthe[Y][X]->setGhost(nullptr);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			g->setLastY(Y);
			g->setLastX(X);
			g->setX(X - 1);
			break;
			// Si le fantôme est bloqué face à tois murs, on autorise la marche arrière (exemple : le retour au cimetière quand ils sont mangés)
		case 4:
			labyrinthe[g->getLastY()][g->getLastX()]->setGhost(g);
			labyrinthe[Y][X]->setGhost(nullptr);
			window->Delete(X * 30, Y * 30, labyrinthe[Y][X]->getType());
			int tmpX = X, tmpY = Y;
			g->setX(g->getLastX());
			g->setY(g->getLastY());
			g->setLastY(tmpY);
			g->setLastX(tmpX);
			break;
		}
		//On redessine le fantôme sur sa nouvelle case
		window->DrawGhost(g->getX() * 30, g->getY() * 30, g->getStatus(), g->getCouleur());
		if (g->getStartX() == g->getX() && g->getStartY() == g->getY()) {
			g->setTimeNow();
		}
	}
}

//On vérifie si pacman a manger toutes les gums
bool Partie::finpartie() {
	for (unsigned int i = 0; i < labyrinthe.size(); i++) {
		for (unsigned int j = 0; j < labyrinthe[i].size(); j++) {
			if (labyrinthe[i][j]->getType() == StaticType::GUM) {
				return false;
			}
		}
	}
	return true;
}

//Affiche Pacman
void Partie::affichePacman(Pacman* pac) {
	window->DrawPacman(pac->getX() * 30, pac->getY() * 30, pac->getStatus());
}

//Affiche un fantôme 
void Partie::afficheGhost(Ghost* boo) {
	window->DrawGhost(boo->getX() * 30, boo->getY() * 30, boo->getStatus(), boo->getCouleur());
}

//Permet de calculer les distances pour l'algo A* (pathfiding) utiliser dans la fonction de déplacement des fantômes
int Partie::distanceManhattan(int i1, int j1, int i2, int j2) {
	return abs(i2 - i1) + abs(j2 - j1);
}

//affiche le score
void Partie::afficheScore() {
	window->showScore(score);
}

//affiche l'écarn de game over
void Partie::GameOver(Bouton& btn) {
	window->afficheGameOver(btn);
}