#ifndef TIRALIEN_H
#define TIRALIEN_H

//#include "UIKit.h"
//#include "Coord.h"
#include "Laser.h"

class TirMartien :public Laser {
	char styleLaser='|';
public:
	void startLaser(unsigned short x, unsigned short y);
	void putLaser() const;
	void moveLaser();
};

#endif