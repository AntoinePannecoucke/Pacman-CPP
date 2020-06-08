#pragma once
#include<iostream>

class Element
{
protected:
	//coordonnées de l'élément
	int x, y;
public:
	//constructeur et destructeur
	Element(int const X = 0, int const Y = 0) : x(X), y(Y) { /*printf("Element +\n");*/ }
	~Element() { /*printf("Element -\n");*/ }
	//getteur
	int getX() { return x; }
	int getY() { return y; }
	//setteur
	void setX(int const X) { x = X; }
	void setY(int const Y) { y = Y; }
};

