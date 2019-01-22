#include <iostream>

#include "config.h"
#include "Vaisseau.h"



using namespace std;

void Vaisseau::removeVaisseau() const
{	
	coord.gotoXY(coord.getPositionX(),coord.getPositionY());
	cout << " ";
}

void Vaisseau::putVaisseau() const
{
	coord.gotoXY(coord.getPositionX(),coord.getPositionY());
	cout << char(19);
}

Vaisseau::Vaisseau()
{
	coord.setPositionX((COL_FIN - COL_DEBUT) /2 );
	coord.setPositionY(LIGNE_FIN -2 );
	putVaisseau();
}

void Vaisseau::modifierPosition(char key)
{	
	removeVaisseau();
	switch(key)
	{
		case 'k' :	
			if (coord.getPositionX() > COL_DEBUT + 2)
				coord.setPositionX(coord.getPositionX()-1);	
			break;
		case 'l' :	
			if (coord.getPositionX() < COL_FIN - 2)
				coord.setPositionX(coord.getPositionX()+1);
			break;
	}
	putVaisseau();
}









