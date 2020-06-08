#include "Space.h"
using namespace std;

void Space::setPac(Pacman* p, int& score) {
	pac = p;
	//incrémente le score
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
	//vérifie si un fantôme et pacman sont sur la même case
	if (pac != nullptr && boo != nullptr) {
		testKill(score);
	}
}

void Space::setGhost(Ghost* g) {
	boo = g;
	//vérifie si un fantôme et pacman sont sur la même case
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
	//Mange un fantôme et incrémente le score
	else if (pac->getStatus() == PacmanStatus::SUPER) {
		boo->setStatus(GhostStatus::EYE);
		score += 1000;
	}
}
