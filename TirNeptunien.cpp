#include "TirNeptunien.h"
#include <iostream>
#include <stdlib.h>
#include "config.h"

TirNeptunien::TirNeptunien(){isAlive=false;}

void TirNeptunien::startLaser(unsigned short x,unsigned short y){
	if(x==2){
		droite=1;
	}else if(x==COL_FIN-1){
		droite=0;
	}else{
		droite=rand()%2;
	}

	if(droite){
		coord.setPositionX(x+1);
	}else{
		coord.setPositionX(x-1);
	}
	coord.setPositionY(y+1);
	putLaser();
	isAlive=true;
}

void TirNeptunien::putLaser()const{
	coord.gotoXY(coord.getPositionX(),coord.getPositionY());
	if(droite){
		std::cout<<styleLaserDroite;
	}else{
		std::cout<<styleLaserGauche;
	}
}

void TirNeptunien::moveLaser(){
	auto posX=coord.getPositionX();
	auto posY=coord.getPositionY();
	removeLaser();
	if(posX>2&&posX<COL_FIN-1&&posY<LIGNE_FIN-2){
		if(droite){
			coord.setPositionX(coord.getPositionX()+1);
		}else{
			coord.setPositionX(coord.getPositionX()-1);
		}
		coord.setPositionY(coord.getPositionY()+1);
		putLaser();
	}
	else {
		isAlive=false;
	}
}