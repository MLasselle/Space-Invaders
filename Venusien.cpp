#include "Venusien.h"
#include <stdlib.h>
#include "config.h"

Venusien::Venusien(int type,int valeur):ExtraTerrestre(type,valeur){
	isAlive=false;
}

void Venusien::moveVenusien(){
	removeExtraTerrestre();
	if(goRight&&coord.getPositionX()!=COL_FIN-1){
		coord.setPositionX(coord.getPositionX()+1);
	}else if(!goRight&&coord.getPositionX()!=2){
		coord.setPositionX(coord.getPositionX()-1);
	}else{
		coord.setPositionY(coord.getPositionY()+1);
		goRight=!goRight;
	}
	putExtraTerrestre();
}

void Venusien::resetExtraTerrestre(){
	coord.setPositionX(rand()%(COL_FIN-2)+2);
	coord.setPositionY(rand()%5+21);
	isAlive = true;
}