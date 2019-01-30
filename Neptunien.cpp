#include "Neptunien.h"
#include <stdlib.h>
#include "config.h"

Neptunien::Neptunien(int type,int valeur):ExtraTerrestre(type,valeur){
	isAlive=false;
}

void Neptunien::resetExtraTerrestre(){
	coord.setPositionX(rand()%(COL_FIN-2)+2);
	coord.setPositionY(rand()%((COL_FIN-2)/3-2)+(LIGNE_FIN-(COL_FIN-2)/3));
	isAlive = true;
}