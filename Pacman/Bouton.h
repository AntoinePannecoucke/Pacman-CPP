#pragma once
#include"Element.h"

class Bouton :
	public Element
{
private:
	//coordonnées du bouton et tailles
	int h, w;
public:
	//getteur
	int getH() { return h; }
	int getW() { return w; }
	//setteur
	void setH(const int nb) { h = nb; }
	void setW(const int nb) { w = nb; }
};

