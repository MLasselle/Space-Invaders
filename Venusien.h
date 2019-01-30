#ifndef VENUSIEN_H
#define VENUSIEN_H

#include "ExtraTerrestre.h"

class Venusien : public ExtraTerrestre{
	bool goRight;
public:
	Venusien(int,int);
	void moveVenusien();
	void resetExtraTerrestre();
};

#endif