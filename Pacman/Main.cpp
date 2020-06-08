#include<iostream>
#include"Pacman.h"
#include"Ghost.h"
#include"Wall.h"
#include"Space.h"
#include<vector>
#include"Partie.h"
#include"Fenetre.h"
#include"Bouton.h"
using namespace std;

int main(int argn, char* argv[]) {
	bool jouer = true;
	while (jouer) {
		//Bouton rejouer
		Bouton restart;
		//Cr�ation des fantomes
		vector<Ghost*> fantomes;
		Ghost Blinky(Couleur::ROUGE), Pinky(Couleur::ROSE), Inky(Couleur::BLEU), Clyde(Couleur::ORANGE);
		fantomes.push_back(&Blinky);
		fantomes.push_back(&Pinky);
		fantomes.push_back(&Inky);
		fantomes.push_back(&Clyde);
		//Cr�ation de pacman
		Pacman pacman;
		//Cr�ation du labyrinthe et placement de pacman et des fant�mes
		Partie test(&pacman, fantomes);
		//affichage de la partie
		test.affiche(&pacman, fantomes);
		bool continuer = true;
		int elapsed_seconds, nbF = fantomes.size();
		SDL_Event event;
		//Boucle pour la gestion du d�placement de pacman
		while (!test.finpartie() && continuer && pacman.getStatus() != PacmanStatus::DEAD)
		{
			//V�rifie si pacman est encore en super ou non
			if (pacman.getCheckTime()) {
				elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(chrono::system_clock::now() - pacman.getStart()).count();
				//Si cela fait trop longtemps
				if (elapsed_seconds > 5) {
					//R�affiche pacman
					pacman.setStatus(PacmanStatus::NORMAL);
					test.affichePacman(&pacman);
					pacman.setCheckTime(false);
					//R�affiche les fant�mes
					for (int i = 0; i < nbF; i++)
					{
						if (fantomes[i]->getStatus() != GhostStatus::EYE) {
							fantomes[i]->setStatus(GhostStatus::TRACK);
							test.afficheGhost(fantomes[i]);
						}
					}
				}
			}
			//V�rifie si les fant�mes qui sont EYE et qui sont de retour au cimeti�re doivent respawn
			for (int i = 0; i < nbF; i++) {
				if (fantomes[i]->getCheckTime()) {
					elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(chrono::system_clock::now() - fantomes[i]->getStart()).count();
					if (elapsed_seconds > 20) {
						//R�affiche le fant�me
						fantomes[i]->setStatus(GhostStatus::TRACK);
						test.afficheGhost(fantomes[i]);
						fantomes[i]->setCheckTime(false);
					}
				}
			}
			//Gestion des touches, jouable en ZQSD ou avec les fl�ches directionnelles
			SDL_WaitEvent(&event);
			switch (event.type)
			{
			//Gestion des touches
			case SDL_KEYDOWN:
				//Touche Z ou fl�che du haut
				if (event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_UP) {
					if (pacman.getDir() != Direction::SOUTH) {
						SDL_Delay(100);
						test.avancePac(&pacman, Direction::NORTH);
						for (int i = 0; i < nbF; i++) {
							test.avanceGhost(fantomes[i], &pacman);
						}
						test.afficheScore();
					}
				}
				//Touche S ou fl�che du bas
				else if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) {
					if (pacman.getDir() != Direction::NORTH) {
						SDL_Delay(100);
						test.avancePac(&pacman, Direction::SOUTH);
						for (int i = 0; i < nbF; i++) {
							test.avanceGhost(fantomes[i], &pacman);
						}
						test.afficheScore();
					}
				}
				//Touche Q ou fl�che gauche
				else if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_LEFT) {
					if (pacman.getDir() != Direction::EAST) {
						SDL_Delay(100);
						test.avancePac(&pacman, Direction::WEST);
						for (int i = 0; i < nbF; i++) {
							test.avanceGhost(fantomes[i], &pacman);
						}
						test.afficheScore();
					}
				}
				//Touche D ou fl�che droite
				else if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) {
					if (pacman.getDir() != Direction::WEST) {
						SDL_Delay(100);
						test.avancePac(&pacman, Direction::EAST);
						for (int i = 0; i < nbF; i++) {
							test.avanceGhost(fantomes[i], &pacman);
						}
						test.afficheScore();
					}
				}
				break;
			//Fermeture de la fenetre avec la croix
			case SDL_QUIT:
				continuer = false;
				jouer = false;
				break;
			}
			//Met tout les fant�mes en fuite quand pacman passe en super
			for (int i = 0; i < nbF; i++) {
				if (pacman.getStatus() == PacmanStatus::SUPER && fantomes[i]->getStatus() == GhostStatus::TRACK) {
					fantomes[i]->setStatus(GhostStatus::FLEE);
					test.afficheGhost(fantomes[i]);
				}
			}
		}
		//Evite l'affiche du game over si on ferme la fenetre avant la fin de la partie
		if (continuer) {
			test.GameOver(restart);
		}
		//Boucle pour la gestion du bouton restart
		while (continuer) {
			SDL_WaitEvent(&event);
			switch (event.type)
			{
			//Gestion du clic sur le bouton restart
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x > restart.getX() && event.button.x< restart.getX() + restart.getW() && event.button.y>restart.getY() && event.button.y < restart.getY() + restart.getH()) {
						continuer = false;
					}
				}
				break;
			//Fermeture de la fenetre avec la croix
			case SDL_QUIT:
				jouer = false;
				continuer = false;
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}