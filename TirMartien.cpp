
#include "config.h"
#include "TirMartien.h"

void TirMartien::startLaser(unsigned short x, unsigned short y){

	coord.setPositionX(x);
	coord.setPositionY(y + 1);
	putLaser();
	isAlive = true;
}

void TirMartien::putLaser() const{

	coord.gotoXY(coord.getPositionX(), coord.getPositionY());
	std::cout<<styleLaser;
}

void TirMartien::moveLaser(){
	auto pos = coord.getPositionY();
	removeLaser();
	if (pos < LIGNE_FIN - 2) {
		coord.setPositionY(coord.getPositionY() + 1);
		putLaser();
	}else {
		isAlive = false;
	}
}