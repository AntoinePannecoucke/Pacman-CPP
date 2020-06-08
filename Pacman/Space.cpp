#include "Space.h"
using namespace std;

void Space::setPac(Pacman* p, int& score) {
	pac = p;
	//incr�mente le score
	if (type == StaticType::GUM && p != nullptr) {
		score += 100;
	}
	//Met pacman en super
	if (type == StaticType::SUPERGUM) {
		pac->setStatus(PacmanStatus::SUPER);
	}
	//rend la case vide si pacman passe sur la case
	if (p != nullptr && this->type != StaticType::EMPTY) {
		type = StaticType::EMPTY;
	}
	//v�rifie si un fant�me et pacman sont sur la m�me case
	if (pac != nullptr && boo != nullptr) {
		testKill(score);
	}
}

void Space::setGhost(Ghost* g) {
	boo = g;
	//v�rifie si un fant�me et pacman sont sur la m�me case
	if (pac != nullptr && boo != nullptr) {
		int buffer;
		testKill(buffer);
	}
}

void Space::testKill(int& score) {
	//Tue pacman
	if (pac->getStatus() != PacmanStatus::SUPER && boo->getStatus() != GhostStatus::EYE) {
		pac->setStatus(PacmanStatus::DEAD);
	}
	//Mange un fant�me et incr�mente le score
	else if (pac->getStatus() == PacmanStatus::SUPER) {
		boo->setStatus(GhostStatus::EYE);
		score += 1000;
	}
}
