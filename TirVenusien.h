#ifndef TIRVENUSIEN_H
#define TIRVENUSIEN_H

#include "Laser.h"

class TirVenusien:public Laser{
	char styleLaser='o';
public:
	TirVenusien();
	void startLaser(unsigned short x, unsigned short y);
	void putLaser() const;
	void moveLaser();
};

#endif