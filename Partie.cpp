#include "Partie.h"

Partie::Partie(int difficulte) {
	Partie::difficulte = difficulte >= 1 && difficulte <= 3 ? difficulte : 1;
}

/*
void Partie::demarrerPartie() {
	//CECI REMPLACE LE MAIN
}
*/