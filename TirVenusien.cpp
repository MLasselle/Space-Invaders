#include "TirVenusien.h"
#include <iostream>
#include "config.h"

TirVenusien::TirVenusien(){isAlive=false;}

void TirVenusien::startLaser(unsigned short x,unsigned short y){

	coord.setPositionX(x);
	coord.setPositionY(y+1);
	putLaser();
	isAlive=true;
}

void TirVenusien::putLaser()const{

	coord.gotoXY(coord.getPositionX(),coord.getPositionY());
	std::cout<<styleLaser;
}

void TirVenusien::moveLaser(){
	auto pos=coord.getPositionY();
	removeLaser();
	if(pos<LIGNE_FIN-2){
		coord.setPositionY(coord.getPositionY()+1);
		putLaser();
	}
	else {
		isAlive=false;
	}
}