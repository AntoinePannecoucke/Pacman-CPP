#pragma once
#include "Element.h"
#include<chrono>

enum class Direction
{
	NONE, NORTH, SOUTH, EAST, WEST
};

class MobileElement :
	public Element
{
protected:
	Direction direction;
	std::chrono::system_clock::time_point statusTime;
	bool checkTime;
public:
	//constructeur
	MobileElement() : direction(Direction::NONE), checkTime(false) { ; }
	//getteur
	std::chrono::system_clock::time_point getStart() { return statusTime; }
	bool getCheckTime() { return checkTime; }
	Direction getDir() { return direction; }
	//setteur
	void setTimeNow() { statusTime = std::chrono::system_clock::now(); checkTime = true; }
	void setCheckTime(bool b) { checkTime = b; }
	void setDir(Direction dir) { direction = dir; }
};

