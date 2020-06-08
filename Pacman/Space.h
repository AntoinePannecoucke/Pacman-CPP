#pragma once
#include "StaticElement.h"
#include "Pacman.h"
#include "Ghost.h"


class Space :
	public StaticElement
{
private:
	Pacman* pac;
	Ghost* boo;
public:
	//constructeur
	Space(StaticType t) : pac(nullptr), boo(nullptr) { this->type = t; }
	//m�thodes virtuelles
	virtual void setPac(Pacman*, int&);
	virtual Pacman* getPac() { return pac; }
	//setteur
	void setGhost(Ghost*);
	//m�thodes void
	void testKill(int&);
};

