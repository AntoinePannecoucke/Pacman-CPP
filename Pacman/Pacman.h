#pragma once
#include "MobileElement.h"

enum class PacmanStatus
{
	NORMAL, SUPER, DEAD
};

class Pacman :
	public MobileElement
{
private:
	PacmanStatus status;
public:
	//constructeur
	Pacman(PacmanStatus const ps = PacmanStatus::NORMAL) : status(ps) { ; }
	//getteur
	PacmanStatus getStatus() { return status; }
	//setteur
	void setStatus(PacmanStatus st);
};

