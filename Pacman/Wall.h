#pragma once
#include "StaticElement.h"
#include<iostream>


class Wall :
	public StaticElement
{
public:
	//constructeur
	Wall(StaticType const t) { this->type = t; }
	//m�thodes virtuelles
	virtual void setGhost(Ghost*) { ; }
};

