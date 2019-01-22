#include <conio.h>
#include <Windows.h>		//	pour utiliser GetAsynKeyState
#include <time.h>

#include "UIKit.h"

#include "config.h"
#include "Vaisseau.h"
#include "Laser.h"
#include "Martien.h"



class Timer {
private:
	clock_t date;
	int delai;

public:
	Timer(int delai) {
		this->delai = delai;
		date = clock();
	}

	bool estPret() {
		bool result = false;
		if (clock() >= date + delai) {
			date = clock();
			result = true;
		}

		return result;
	}

};

//	nombre maximum de lasers a l'ecran
#define MAX_LASERS 15

void testerCollision(Laser & unLaser, Martien & unMartien) {
	//	test de collision du laser avec le martien
	if (unLaser.coord.getPositionX() == unMartien.coord.getPositionX() &&
		unLaser.coord.getPositionY() == unMartien.coord.getPositionY()) {
		unMartien.isAlive = false;	//	le martien est mort

		unLaser.killLaser();	//	le laser est mort

		unMartien.resetExtraTerrestre();	//	on cree un autre martien a l'ecran
	}
}
//	fonction principale
int main()
{
	srand((unsigned int)time(NULL));

	bool finDeJeu = false;

	//	donnees pour le vaisseau
	Vaisseau monVaisseau;		//	instance de Vaisseau pour modeliser le vaisseau du joueur
	Timer timerVaisseau(55);	//	timer pour les deplacements et le tir du vaisseau
	Timer timerMissile(40);		//	timer pour le deplacement des missiles

	//	donnees pour le laser
	Laser mesLasers[MAX_LASERS];
	for (int i = 0; i < MAX_LASERS; i++)
		mesLasers[i].isAlive = false;

	//	un ennemi
	Martien martien('W', 5);
	martien.resetExtraTerrestre();
	Timer timerMartien(1000);

	//	des ennemis (exemple a terminer...)
	Martien alien('U', 10);
	Martien lesMartiens[] = { alien, alien, alien, alien, alien };

	//	on efface le curseur
	UIKit::curseurVisible(false);

	//déterminer la dimension de la fenetre
	UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN, LIGNE_FIN);

	//	boucle principale de jeu 
	while (!finDeJeu) {

		UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 1, LIGNE_FIN - 1, 2);

		//	deplacements et tir du vaisseau (timer timerVaisseau)
		if (timerVaisseau.estPret()) {
			if (GetAsyncKeyState('K') != 0) monVaisseau.modifierPosition('k');
			if (GetAsyncKeyState('L') != 0) monVaisseau.modifierPosition('l');
			if (GetAsyncKeyState(' ') != 0) { //	recherche d'une instance de Laser non utilisee
				int i = 0;
				while (i < MAX_LASERS && mesLasers[i].isAlive)
					i++;

				if (i < MAX_LASERS)
					mesLasers[i].startLaser(monVaisseau.coord.getPositionX());
			}
			monVaisseau.modifierPosition('\0');
		}

		//	deplacement des lasers (timer timerMissile)
		if (timerMissile.estPret()) {
			for (int i = 0; i < MAX_LASERS; i++)
				if (mesLasers[i].isAlive) {
					mesLasers[i].moveLaser();

					//	test de collision avec le martien
					testerCollision(mesLasers[i], martien);
				}
		}

		//	deplacement du martien (timer timerMartien)
		if (martien.isAlive && timerMartien.estPret()) {
			martien.jiggleMartien();

			//	test de collision avec les lasers
			for (int i = 0; i < MAX_LASERS; i++)
				if (mesLasers[i].isAlive) {
					//	test de collision avec le martien
					testerCollision(mesLasers[i], martien);
				}
		}
	}

	return 0;
}