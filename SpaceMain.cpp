#include <conio.h>
#include <Windows.h>		//	pour utiliser GetAsynKeyState
#include <time.h>
#include "Timer.h"
#include "UIKit.h"

#include "config.h"
#include "Vaisseau.h"
#include "Laser.h"
#include "Martien.h"
#include "TirMartien.h"
#include "Venusien.h"
#include "TirVenusien.h"
#include "Neptunien.h"
#include "TirNeptunien.h"


bool testerCollision(Laser& unLaser, ExtraTerrestre* extraTerrestre, unsigned int & score);
void presentation();
void menu();
int saisirNiveau();
void ecrans_help();
void afficher_coeurs(int nb_vie);
bool testerTouche(Laser tirET, Vaisseau monVaisseau, unsigned short* nbVies);
bool rejouer(int score);

//	fonction principale
int main()
{

	//déterminer la dimension de la fenetre
	UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN + 40, LIGNE_FIN);//utile
	//on efface le curseur
	UIKit::curseurVisible(false);

	srand((unsigned int)time(NULL));

	unsigned short niveau=0;
	bool finDeJeu;

	unsigned int score;
	unsigned short nbVies;

		//donnees pour le vaisseau
	Vaisseau monVaisseau;		//instance de Vaisseau pour modeliser le vaisseau du joueur
	Timer timerVaisseau;	//timer pour les deplacements et le tir du vaisseau
	Laser* mesLasers;
	Timer timerLaser;		//timer pour le deplacement des missiles
	unsigned short maxLasers;

		//donnees pour les martiens
	Martien** martien;
	Timer* timerJiggleMartien;
	TirMartien* tirMartien;
	unsigned short nbMartiens;
	// timer spawn martien ?
	Timer timerTirMartien;
	unsigned short probaTirMartien;

		//donnees pour les venusiens
	Venusien** venusien;
	Timer* timerMoveVenusien;
	TirVenusien* tirVenusien;
	unsigned short nbVenusiens;
	Timer timerSpawnVenusien;
	Timer timerTirVenusien;
	unsigned short probaTirVenusien;

		//donnees pour les neptuniens
	Neptunien** neptunien;
	TirNeptunien* tirNeptunien;
	unsigned short nbNeptuniens;
	Timer timerSpawnNeptunien;
	Timer timerTirNeptunien;
	unsigned short probaTirNeptunien;


	system("cls");
	presentation();
	menu();

	//boucle principale de jeu
	do {

		system("cls");
		niveau=saisirNiveau();

		if(niveau==1){
				//donnees pour le vaisseau
			nbVies = 3;
			timerVaisseau.set(55);
			timerLaser.set(40);
			maxLasers=10;

				//donnees pour les martiens
			nbMartiens=10;
			// timer spawn martien ?
			timerTirMartien.set(100);
			probaTirMartien=50;

				//donnees pour les venusiens
			nbVenusiens=10;
			timerSpawnVenusien.set(500);
			timerTirVenusien.set(200);
			probaTirVenusien=50;

				//donnees pour les neptuniens
			nbNeptuniens=10;
			timerSpawnNeptunien.set(500);
			timerTirNeptunien.set(200);
			probaTirNeptunien=50;

		}else if(niveau==2){
			
		}else if(niveau==3){
			
		}else{
			return 1;
		}

		score = 0;

		mesLasers=new Laser[maxLasers];
		for (int i = 0; i < maxLasers; i++) {
			mesLasers[i].isAlive = false;
		}

			//donnees pour les martiens
		martien=new Martien*[nbMartiens];
		timerJiggleMartien=new Timer[nbMartiens];
		tirMartien=new TirMartien[nbMartiens];
		for (unsigned short i = 0; i < nbMartiens; i++) {
			martien[i] = new Martien('U', 5);
			martien[i]->resetExtraTerrestre();	// ou sinon ou pourrait attendre le timer ...
			martien[i]->putExtraTerrestre();	// ou sinon ou pourrait attendre le timer ...
			//martien[i]->isAlive=false;
			timerJiggleMartien[i].set((rand() % 500) + 500);
			tirMartien[i].isAlive = 0;
		}

		//	POURQUOI PAS METTRE LES TIMERS DANS LES CLASSES E.T. DIRECTEMENT ?

			//donnees pour les venusiens
		venusien=new Venusien*[nbVenusiens];
		timerMoveVenusien=new Timer[nbVenusiens];
		tirVenusien=new TirVenusien[nbVenusiens];
		for(unsigned short i=0;i<nbVenusiens;i++){
			venusien[i]=new Venusien('T',4);
			timerMoveVenusien[i].set((rand() % 500) + 500);
		}

			//donnees pour les neptuniens
		neptunien=new Neptunien*[nbNeptuniens];
		tirNeptunien=new TirNeptunien[nbNeptuniens];
		for(unsigned short i=0;i<nbNeptuniens;i++){
			neptunien[i]=new Neptunien('A',7);
		}

		system("cls");

		finDeJeu = false;

		//Pour afficher
		UIKit::cadre(COL_DEBUT - 1, LIGNE_DEBUT, COL_FIN, LIGNE_FIN - 1, 19);
		UIKit::color(14);
		UIKit::gotoXY(COL_FIN + 2, LIGNE_DEBUT + 2);
		std::cout << "Score : " << score;
		UIKit::gotoXY(COL_FIN + 2, LIGNE_DEBUT + 4);
		std::cout << "Vies : " << nbVies;
		afficher_coeurs(nbVies);
		UIKit::color(11);
		
		keybd_event(27, 0, 0, 0);
		keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
		while (!finDeJeu) {

			//deplacements et tir du vaisseau
			if (timerVaisseau.estPret()) {
				
				if (GetAsyncKeyState('K') && monVaisseau.coord.getPositionX() > COL_DEBUT + 2) {
					monVaisseau.modifierPosition('k');
					unsigned short i = 0;
					while (i < nbMartiens && !testerTouche(tirMartien[i], monVaisseau, &nbVies)) { i++; }
				}
				if (GetAsyncKeyState('L') && monVaisseau.coord.getPositionX() < COL_FIN - 2) {
					monVaisseau.modifierPosition('l');
					unsigned short i = 0;
					while (i < nbMartiens && !testerTouche(tirMartien[i], monVaisseau, &nbVies)) { i++; }
				}
				
				if (GetAsyncKeyState(' ')) { //recherche d'une instance de Laser non utilisee
					int i = 0;
					while (i < maxLasers && mesLasers[i].isAlive) {
						i++;
					}
					if (i < maxLasers) {

						mesLasers[i].startLaser(monVaisseau.coord.getPositionX());
					}
				}
				monVaisseau.modifierPosition(0);
			}

			//Deplacement du laser
			if (timerLaser.estPret()) {
				bool touche;
				unsigned short j;
				for (int i = 0; i < maxLasers; i++) {
					if (mesLasers[i].isAlive) {
						touche=0;
						mesLasers[i].moveLaser();
						j=0;
						while(!touche&&j<nbMartiens){
							if(testerCollision(mesLasers[i], martien[j], score)){touche=1;}
							j++;
						}
						j=0;
						while(!touche&&j<nbVenusiens){
							if(testerCollision(mesLasers[i], venusien[j], score)){touche=1;}
							j++;
						}
						j=0;
						while(!touche&&j<nbNeptuniens){
							if(testerCollision(mesLasers[i], neptunien[j], score)){touche=1;}
							j++;
						}
					}
				}
			}

			//deplacement des martiens
			for (int i = 0; i < nbMartiens; i++) {
				if (martien[i]->isAlive) {
					if(timerJiggleMartien[i].estPret()){martien[i]->jiggleMartien();}
					for (int j = 0; j < maxLasers; j++) {
						if (mesLasers[j].isAlive) {
							testerCollision(mesLasers[j], martien[i], score);
						}
					}
				}
			}
			//tirs des martiens
			if (timerTirMartien.estPret()){
				for (int i = 0; i < nbMartiens; i++) {
					if (tirMartien[i].isAlive) {
						tirMartien[i].moveLaser();
						testerTouche(tirMartien[i], monVaisseau, &nbVies);
					}
					else if (rand() % probaTirMartien == 0) {
						tirMartien[i].startLaser(martien[i]->coord.getPositionX(), martien[i]->coord.getPositionY());
					}
				}
			}

			//deplacement des venusiens
			for (int i = 0; i < nbVenusiens; i++) {
				if (venusien[i]->isAlive) {
					if(timerMoveVenusien[i].estPret()){venusien[i]->moveVenusien();}
					for (int j = 0; j < maxLasers; j++) {
						if (mesLasers[j].isAlive) {
							testerCollision(mesLasers[j], venusien[i], score);
						}
					}
				}
			}
			//spawn des venusiens
			if(timerSpawnVenusien.estPret()){
				unsigned short i=0;
				while(i<nbVenusiens&&venusien[i]->isAlive){i++;}
				if(i<nbVenusiens){
					venusien[i]->resetExtraTerrestre();
					venusien[i]->putExtraTerrestre();
				}
			}
			//tirs des venusiens
			if (timerTirVenusien.estPret()){
				for (int i = 0; i < nbVenusiens; i++) {
					if (tirVenusien[i].isAlive) {
						tirVenusien[i].moveLaser();
						testerTouche(tirVenusien[i], monVaisseau, &nbVies);
					}
					else if (venusien[i]->isAlive&&rand() % probaTirVenusien == 0) {
						tirVenusien[i].startLaser(venusien[i]->coord.getPositionX(), venusien[i]->coord.getPositionY());
					}
				}
			}

//chantier
			//rafraîchissement des neptuniens
			for (int i = 0; i < nbNeptuniens; i++) {
				if (tirNeptunien[i].isAlive) {
					neptunien[i]->putExtraTerrestre();
				}
			}
			//spawn des neptuniens
			if(timerSpawnNeptunien.estPret()){
				unsigned short i=0;
				while(i<nbNeptuniens&&neptunien[i]->isAlive){i++;}
				if(i<nbNeptuniens){
					neptunien[i]->resetExtraTerrestre();
					neptunien[i]->putExtraTerrestre();
				}
			}
			//tirs des neptuniens
			if (timerTirNeptunien.estPret()){
				for (int i = 0; i < nbNeptuniens; i++) {
					if (tirNeptunien[i].isAlive) {
						tirNeptunien[i].moveLaser();
						testerTouche(tirNeptunien[i], monVaisseau, &nbVies);
					}
					else if (neptunien[i]->isAlive&&rand() % probaTirNeptunien == 0) {
						tirNeptunien[i].startLaser(neptunien[i]->coord.getPositionX(), neptunien[i]->coord.getPositionY());
					}
				}
			}
//chantier

			if (!nbVies) { finDeJeu = true; }
		}
		delete[] mesLasers;
		for(unsigned short i=0;i<nbMartiens;i++){
			delete martien[i];
		}
		delete[] martien;
		delete[] timerJiggleMartien;
		delete[] tirMartien;
		
		for (int i = 0; i < nbVenusiens; i++) {
			delete venusien[i];
		}
		delete[] venusien;
		delete[] timerMoveVenusien;
		delete[] tirVenusien;

		for (int i = 0; i < nbNeptuniens; i++) {
			delete neptunien[i];
		}
		delete[] neptunien;
		delete[] tirNeptunien;

	} while (rejouer(score));
	system("cls");
	return 0;
}



bool testerCollision(Laser& unLaser, ExtraTerrestre* extraTerrestre, unsigned int & score) {
	//test de collision du laser avec un E.T.
	if (unLaser.coord.getPositionX() == extraTerrestre->coord.getPositionX() &&
		unLaser.coord.getPositionY() == extraTerrestre->coord.getPositionY()) {
		extraTerrestre->isAlive = false;	//E.T. est mort
		unLaser.killLaser();	//le laser est mort
		extraTerrestre->resetExtraTerrestre();	//on cree un autre E.T. l'ecran	// ou sinon ou pourrait attendre le timer ...
		score += extraTerrestre->ajouterPoints();

		
		UIKit::color(14);
		UIKit::gotoXY(COL_FIN + 2 + 8, LIGNE_DEBUT + 2);
		std::cout << score;
		UIKit::color(11);
		return 1;
	}
	return 0;
}


bool testerTouche(Laser tirET, Vaisseau monVaisseau, unsigned short* nbVies) {
	if (tirET.coord.getPositionX() == monVaisseau.coord.getPositionX() && tirET.coord.getPositionY() == monVaisseau.coord.getPositionY() && tirET.isAlive) {
		(*nbVies)--;
		UIKit::color(14);
		UIKit::gotoXY(COL_FIN + 2 + 7, LIGNE_DEBUT + 4);
		std::cout << *nbVies;
		afficher_coeurs(*nbVies);
		UIKit::color(11);
		return 1;
	}
	return 0;
}


void presentation() {
	//UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN, LIGNE_FIN);
	//UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 1, LIGNE_FIN - 1, 2);
	UIKit::color(10);
	Coord::gotoXY(30, 15); cout << endl;
	Coord::gotoXY(30, 16); cout << "                         / __| '_ \\ / _` |/ __/ _ \\                        " << endl;
	Coord::gotoXY(30, 17); cout << "                         \\__ \\ |_) | (_| | (_|  __/                        " << endl;
	Coord::gotoXY(30, 18); cout << "                         |___/ .__/ \\__,_|\\___\\___|                        " << endl;
	Coord::gotoXY(30, 19); cout << "                             |_|                                           " << endl;
	Coord::gotoXY(30, 20); cout << endl << endl;
	Coord::gotoXY(30, 21); cout << "       _____   __   _   _     _     ___   _____   _____   _____    _____  " << endl;
	Coord::gotoXY(30, 22); cout << "      |_   _| |  1 | | | |   / /   /   | |  _  1 | ____| |  _  1  /  ___/ " << endl;
	Coord::gotoXY(30, 23); cout << "        | |   |   1| | | |  / /   / /| | | | | | | |__   | |_| |  | |___  " << endl;
	Coord::gotoXY(30, 24); cout << "        | |   | |1   | | | / /   / /_| | | | | | |  __|  |  _  /  1___  1 " << endl;
	Coord::gotoXY(30, 25); cout << "       _| |_  | | 1  | | |/ /   / ,__  | | |_| | | |___  | | 1 1   ___| | " << endl;
	Coord::gotoXY(30, 26); cout << "      |_____| |_|  1_| |___/   /_/   |_| |_____/ |_____| |_|  1_1 /_____/ " << endl;
	Coord::gotoXY(30, 27); cout << endl << endl << endl; UIKit::color(9);
	Coord::gotoXY(30, 28); cout << "                      Programme par Maxime !    " << endl << endl;

	Coord::gotoXY(30, 29); cout << "                       Tapez sur une touche pour continuer" << endl;
	fflush(stdout);
	_getch();
	system("cls");

}



void menu()
{
	//UIKit::setDimensionFenetre(COL_DEBUT, LIGNE_DEBUT, COL_FIN, LIGNE_FIN);
	//UIKit::cadre(COL_DEBUT + 1, LIGNE_DEBUT, COL_FIN - 1, LIGNE_FIN - 1, 2);
	const char up(72), down(80), ok(13), shoot(32);
	UIKit::color(11);
	cout << endl;
	Coord::gotoXY(30, 15); cout << "               _____________________________________________              " << endl;
	Coord::gotoXY(30, 16); cout << "              |                                             |             " << endl;
	Coord::gotoXY(30, 18); cout << "              |         Etes vous pret a jouer ?            |             " << endl;
	Coord::gotoXY(30, 19); cout << "              |_____________________________________________|             " << endl;
	cout << endl << endl;
	UIKit::color(10);
	Coord::gotoXY(30, 20); cout << "                           Faites votre choix                           " << endl;
	Coord::gotoXY(30, 21); cout << "                                                                          " << endl;
	Coord::gotoXY(30, 22); cout << "                   ,---------------------------------,                    " << endl;
	Coord::gotoXY(30, 23); cout << "                   |       "; UIKit::color(10); cout << "      JOUER !    "; UIKit::color(10); cout << "         |                    " << endl;
	Coord::gotoXY(30, 24); cout << "                   '---------------------------------'                    " << endl;
	Coord::gotoXY(30, 25); cout << endl;
	Coord::gotoXY(30, 26); cout << "                   ,---------------------------------,                    " << endl;
	Coord::gotoXY(30, 27); cout << "                   |             Aide ?              |                    " << endl;
	Coord::gotoXY(30, 28); cout << "                   '---------------------------------'                    " << endl;


	UIKit::color(12);
	Coord::gotoXY(32, 23); cout << "-->";
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
			Coord::gotoXY(32, 23); cout << "-->";
			Coord::gotoXY(32, 27); cout << "   ";
			Coord::gotoXY(32, 31); cout << "   ";
		}

		else if (fleche == 2) {
			Coord::gotoXY(32, 23); cout << "   ";
			Coord::gotoXY(32, 27); cout << "-->";
			Coord::gotoXY(32, 31); cout << "   ";
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
	Coord::gotoXY(30, 30); cout << "                     Bienvenue dans HELLO INVADERS,                       " << endl << endl;
	Coord::gotoXY(30, 31); cout << "     Le but de ce jeu est de detruire le plus d'extraterrestre possible   " << endl << endl;
	Coord::gotoXY(30, 32); cout << "                           ,---------------,                              " << endl;
	Coord::gotoXY(30, 33); cout << "                           | les touches : |                              " << endl;
	Coord::gotoXY(30, 34); cout << "                           '---------------'                              " << endl << endl;
	Coord::gotoXY(30, 35); cout << "                             Droite :  'k'                                " << endl << endl;
	Coord::gotoXY(30, 36); cout << "                             Gauche :  'l'                                " << endl << endl;
	Coord::gotoXY(30, 37); cout << "                             Ok     :  ENTRE                              " << endl << endl;
	Coord::gotoXY(30, 38); cout << "                             Shoot  :  ESPACE                             " << endl << endl << endl;
	Coord::gotoXY(30, 39); cout << "                    Tapez sur une touche pour continuer                   " << endl << endl;

	fflush(stdout);
	_getch();
	//system("pause");
	system("cls");
	menu();
}

int saisirNiveau()
{

	int saisirNiveau, nbExtraterrestre;
	bool ex = false;
	/*
	cout << "                     `. ___" << endl;
	cout << "                    __,' __`.                _..----....____" << endl;
	cout << "        __...--.'``;.   ,.   ;``--..__     .'    ,-._    _.-'" << endl;
	cout << "  _..-''-------'   `'   `'   `'     O ``-''._   (,;') _,'" << endl;
	cout << ",'________________                          \`-._`-','" << endl;
	cout << " `._              ```````````------...___   '-.._'-:" << endl;
	cout << "    ```--.._      ,.                     ````--...__\-." << endl;
	cout << "            `.--. `-`                       ____    |  |`" << endl;
	cout << "              `. `.                       ,'`````.  ;  ;`" << endl;
	cout << "                `._`.        __________   `.      \'__/`" << endl;
	cout << "                   `-:._____/______/___/____`.     \  `" << endl;
	cout << "                               |       `._    `.    \"<<endl;
		cout << "                               `._________`-.   `.   `.___" << endl;
	cout << "                                             SSt  `------'`" << endl;
	*/
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
		cout << "  Saisir un niveau du jeu : ";
		cin >> saisirNiveau;

		if (cin.fail() || cin.peek() != '\n') {
			UIKit::color(4); cout << "  Ce niveau n'est pas dans le menu" << endl;
			cin.clear();
			cin.ignore(512, '\n');

		}
		else if ((saisirNiveau != 1) && (saisirNiveau != 2) && (saisirNiveau != 3)) {
			cout << "Choisir un niveau qui est dans le menu : " << endl;

		}
		else {
			ex = true;
		}

	} while (!ex);

	/* ***********A TERMINER AVEC ALPHA POUR NIVEAU DE DIFFICULTER***************** */

	return saisirNiveau;

}

void afficher_coeurs(int nb_vie)
{
	UIKit::color(13);
	Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6);

	
	switch (nb_vie) {
	case 3:
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 0); cout << " ,-. .-, " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 1);	cout << "[   '   ]" << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 2); cout << " ',   ,' " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 3); cout << "   '.'   " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 4); cout << " ,-. .-, " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 5); cout << "[   '   ]" << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 6); cout << " ',   ,' " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 7); cout << "   '.'   " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 8); cout << " ,-. .-, " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 9); cout << "[   '   ]" << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 10); cout << " ',   ,' " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 11); cout << "   '.'   " << endl;
		break;
	case 2:
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 0); cout << " ,-. .-, " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 1);	cout << "[   '   ]" << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 2); cout << " ',   ,' " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 3); cout << "   '.'   " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 4); cout << " ,-. .-, " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 5); cout << "[   '   ]" << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 6); cout << " ',   ,' " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 7); cout << "   '.'   " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 8); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 9); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 10); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 11); cout << "         " << endl;
		break;
	case 1:
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 0); cout << " ,-. .-, " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 1);	cout << "[   '   ]" << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 2); cout << " ',   ,' " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 3); cout << "   '.'   " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 4); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 5); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 6); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 7); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 8); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 9); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 10); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 11); cout << "         " << endl;
		break;
	case 0:
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 0); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 1);	cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 2); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 3); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 4); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 5); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 6); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 7); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 8); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 9); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 10); cout << "         " << endl;
		Coord::gotoXY(COL_FIN + 4, LIGNE_DEBUT + 6 + 11); cout << "         " << endl;
		break;
	}
	
}


bool rejouer(int score) {
	char rep = 0;
DEBUT:
	keybd_event(27, 0, 0, 0);
	keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
	system("cls");
	UIKit::color(12);
	cout << endl;
	cout << "         _____       ___       ___  ___   _____  " << endl;
	cout << "        /  ___|     /   |     /   |/   | | ____| " << endl;
	cout << "        | |        / /| |    / /|   /| | | |__   " << endl;
	cout << "        | | __    / / | |   / / |__/ | | |  __|  " << endl;
	cout << "        | |_| |  / /  | |  / /       | | | |___  " << endl;
	cout << "        1_____/ /_/   |_| /_/        |_| |_____| " << endl << endl << endl;
	cout << "                                 _____   _     _   _____   _____   " << endl;
	cout << "                                /  _  1 | |   / / | ____| |  _  1  " << endl;
	cout << "                                | | | | | |  / /  | |__   | |_| |  " << endl;
	cout << "                                | | | | | | / /   |  __|  |  _  /  " << endl;
	cout << "                                | |_| | | |/ /    | |___  | | 1 1  " << endl;
	cout << "                                1_____/ |___/     |_____| |_|  1_1 " << endl;
	cout << endl << endl << endl;
	cout << "                                 SCORE :  " << score << endl << endl << endl;
	cout << "                                                           " << endl << endl;
	cout << "                       Rejouer (Y/N) ? ";

	cin >> rep;
	switch (rep) {
	case 'y':
	case 'Y':return 1;
	case 'n':
	case 'N':return 0;
	default:
		//UIKit::gotoXY(39,16);
		goto DEBUT;
	}
	system("cls");
}
