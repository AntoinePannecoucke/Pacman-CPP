#include "Pacman.h"
using namespace std;

void Pacman::setStatus(PacmanStatus st) {
	if (st == PacmanStatus::SUPER) {
		//Met active la verification de temps
		this->setTimeNow();
	}
	status = st;
}