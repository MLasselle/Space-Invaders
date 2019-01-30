#ifndef TIRNEPTUNIEN_H
#define TIRNEPTUNIEN_H

#include "Laser.h"

class TirNeptunien:public Laser{
	char styleLaserDroite='\\';
	char styleLaserGauche='/';
	bool droite;
public:
	TirNeptunien();
	void startLaser(unsigned short x, unsigned short y);
	void putLaser() const;
	void moveLaser();
};

#endif