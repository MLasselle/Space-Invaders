#include <conio.h>
#include <Windows.h>		//	pour utiliser GetAsynKeyState
#include <time.h>
#include"Timer.h"
#include "UIKit.h"

#include "config.h"
#include "Vaisseau.h"
#include "Laser.h"
#include "Martien.h"



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
void presentation();
void menue();
int saisirNiveau();
void ecrans_help();
//	fonction principale
int main()
{
	

	presentation();
	menue();
	system("cls");
	saisirNiveau();
	system("cls");

	
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
		//Determiner la dimension et la couleur du cadre (UIkit)
		UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 20, LIGNE_FIN - 1, 2);

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
	
	//system("pause");
	return 0;
}

void presentation() {
	//UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN, LIGNE_FIN);
	//UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 1, LIGNE_FIN - 1, 2);
		UIKit::color(10);
		cout << endl;
		cout << "                         / __| '_ \ / _` |/ __/ _ \                        " << endl;
		cout << "                         \__ \ |_) | (_| | (_|  __/                        " << endl;
		cout << "                         |___/ .__/ \__,_|\___\___|                        " << endl;
		cout << "                             |_|                                           " << endl;
		cout << endl << endl;
		cout << "       _____   __   _   _     _     ___   _____   _____   _____    _____  " << endl;
		cout << "      |_   _| |  1 | | | |   / /   /   | |  _  1 | ____| |  _  1  /  ___/ " << endl;
		cout << "        | |   |   1| | | |  / /   / /| | | | | | | |__   | |_| |  | |___  " << endl;
		cout << "        | |   | |1   | | | / /   / /_| | | | | | |  __|  |  _  /  1___  1 " << endl;
		cout << "       _| |_  | | 1  | | |/ /   / ,__  | | |_| | | |___  | | 1 1   ___| | " << endl;
		cout << "      |_____| |_|  1_| |___/   /_/   |_| |_____/ |_____| |_|  1_1 /_____/ " << endl;
		cout << endl << endl << endl; UIKit::color(9);
		cout << "                      Programme par Maxime,Pierre et Alpha    " << endl << endl;
		
		cout << "                       Tapez sur une touche pour continuer" << endl;
		fflush(stdout);
		_getch();
		system("cls");
	
}

void menue()
{
	//UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN, LIGNE_FIN);
	//UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 1, LIGNE_FIN - 1, 2);
	const char up(72), down(80), ok(13), shoot(32);
	UIKit::color(11);
	cout << endl;
	cout << "               _____________________________________________              " << endl;
	cout << "              |                                             |             " << endl;
	cout << "              |         Etes vous pret a jouer ?            |             " << endl;
	cout << "              |_____________________________________________|             " << endl;
	cout << endl << endl;
	UIKit::color(10);
	cout << "                           Faites votre choix                           " << endl;
	cout << "                                                                          " << endl;
	cout << "                   ,---------------------------------,                    " << endl;
	cout << "                   |       "; UIKit::color(10); cout << "      JOUER !    "; UIKit::color(10); cout << "         |                    " << endl;
	cout << "                   '---------------------------------'                    " << endl;
	cout << endl;
	cout << "                   ,---------------------------------,                    " << endl;
	cout << "                   |             Aide ?              |                    " << endl;
	cout << "                   '---------------------------------'                    " << endl;


	UIKit::color(12);
	Coord::gotoXY(12, 10); cout << "-->";
	int fleche(1);
	char touche(_getch());

	while (touche != ok)
	{

		if (touche == up) { fleche--; }
		if (touche == down) { fleche++; }

		if (fleche == 0) { fleche = 2; }
		if (fleche == 3) { fleche = 1; }
		/// la nouvelle valeur de la fleche a été donné

		UIKit::color(12);
		if (fleche == 1) {
			Coord::gotoXY(12, 10); cout << "-->";
			Coord::gotoXY(12, 14); cout << "   ";
			Coord::gotoXY(12, 18); cout << "   ";
		}

		else if (fleche == 2) {
			Coord::gotoXY(12, 10); cout << "   ";
			Coord::gotoXY(12, 14); cout << "-->";
			Coord::gotoXY(12, 18); cout << "   ";
		}


		else { cout << "erreur;  position fleche impossible"; }
		touche = _getch();
	} /// fin de la boucle du menu

	

	// on n'a pas besoin de preciser ce qui ce passe si fleche == 1
	if (fleche == 2) { ecrans_help(); }
	//else if (fleche == 1) {  }************Besoin du niveau de difficulter*******************

	UIKit::color(9);

}
void ecrans_help() {
	//UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN, LIGNE_FIN);
	//UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 1, LIGNE_FIN - 1, 2);
	UIKit::color(11);
	cout << endl;
	cout << "                     Bienvenue dans HELLO INVADERS,                       " << endl << endl;
	cout << "     Le but de ce jeu est de detruire le plus d'extraterrestre possible   " << endl << endl;
	cout << "                           ,---------------,                              " << endl;
	cout << "                           | les touches : |                              " << endl;
	cout << "                           '---------------'                              " << endl << endl;
	cout << "                             Droite :  'k'                                " << endl << endl;
	cout << "                             Gauche :  'l'                                " << endl << endl;
	cout << "                             Ok     :  ENTRE                              " << endl << endl;
	cout << "                             Shoot  :  ESPACE                             " << endl << endl << endl;
	cout << "                    Tapez sur une touche pour continuer                   " << endl << endl;

	fflush(stdout);
	_getch();
	//system("pause");
	system("cls");
	menue();
}

int saisirNiveau()
{


	int saisirNiveau, nbExtraterrestre;
	bool ex = false;
	UIKit::color(11);
	cout << endl;
	cout << endl;
	cout << "                      !!!!!!****SPACE INVADERS ****>> !!!!!!                        " << endl;

	cout << endl;
	cout << endl;

	cout << "                           ****** NIVEAU 1 // 1 ******                              " << endl;
	cout << "                           ****** NIVEAU 2 // 2 ******                              " << endl;
	cout << "                           ****** NIVEAU 3 // 3 ******                              " << endl;

	cout << endl;
	cout << endl;


	do {
		cout << "  Saisir un niveau du jeu :) : ";
		cin >> saisirNiveau;

		if (cin.fail() || cin.peek() != '\n') {
			cout << "  Ce niveau n'est pas dans le menu" << endl;
			cin.clear();
			cin.ignore(512, '\n');

		}
		else if ((saisirNiveau != 1) && (saisirNiveau != 2) && (saisirNiveau != 3)) {
			cout << "Choisir un niveau qui est dans le menu :-) : " << endl;

		}
		else {
			ex = true;
		}

	} while (!ex);

	/* ***********A TERMINER AVEC ALPHA POUR NIVEAU DE DIFFICULTER***************** */

	if (saisirNiveau == 1) {
		nbExtraterrestre = 4;

	}
	else if (saisirNiveau == 2) {
		nbExtraterrestre = 6;

	}
	else {
		nbExtraterrestre = 24;
	}

	return nbExtraterrestre;

}
