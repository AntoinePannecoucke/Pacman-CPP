#pragma once
#include "MobileElement.h"

enum class GhostStatus
{
	TRACK, EYE, FLEE
};

enum class Couleur
{
	ROSE, ORANGE, ROUGE, BLEU, BLANC
};

class Ghost :
	public MobileElement
{
private:
	GhostStatus status;
	Couleur color;
	//dernières coordonnées
	int lastX, lastY;
	//coordonnées du spawn
	int startX, startY;
public:
	//Constructeur
	Ghost( Couleur const c = Couleur::BLANC) : color(c), lastX(-1), lastY(-1), startX(-1), startY(-1) { 
		status = GhostStatus::TRACK;
	}
	//getteur
	int getLastX() { return lastX; }
	int getLastY() { return lastY; }
	int getStartX() { return startX; }
	int getStartY() { return startY; }
	GhostStatus getStatus() { return status; }
	Couleur getCouleur() { return color; }
	//setteur
	void setStatus(GhostStatus gs) { status = gs; }
	void setLastX(int X) { lastX = X; }
	void setLastY(int Y) { lastY = Y; }
	void setStartX(int X) { startX = X; }
	void setStartY(int Y) { startY = Y; }
};

