#pragma once
#include "Element.h"
#include"Pacman.h"
#include"Ghost.h"

enum class StaticType
{
	EMPTY, GUM, SUPERGUM, CIMETIERE, START, WALL
};

class StaticElement :
	public Element
{
protected:
	StaticType type;
public:
	//constructeur
	StaticElement() : type(StaticType::EMPTY) { ; }
	//méthodes virtuelles
	virtual Pacman* getPac() { return EXIT_SUCCESS; }
	virtual void setPac(Pacman*, int&) { ; }
	virtual void setGhost(Ghost*) { ; }
	//getteur
	StaticType getType() { return type; }
	//setteur
	void setType(StaticType st) { type = st; }
};

