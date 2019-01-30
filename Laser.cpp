#include <iostream>
#include"UIKit.h"
#include "config.h"
#include "Laser.h"


using namespace std;

void Laser::startLaser(int x)
{
	
	coord.setPositionX(x);
	coord.setPositionY(47);
	
	putLaser();
	isAlive = true;
}

void Laser::removeLaser() const
{	
	coord.gotoXY(coord.getPositionX(),coord.getPositionY());
	cout << " ";
}

void Laser::putLaser() const
{	
	
	coord.gotoXY(coord.getPositionX(),coord.getPositionY());
	cout << char(15);		
}

void Laser::moveLaser()
{	
	auto pos = coord.getPositionY();
	removeLaser();
	if (pos > 0 && pos > LIGNE_DEBUT + 1) {
		coord.setPositionY(coord.getPositionY()-1);
		putLaser();
	}
	else {
		isAlive = false;
	}
}

void Laser::killLaser()
{
	removeLaser();
	isAlive = false;
}
