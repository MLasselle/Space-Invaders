#include"Timer.h"

Timer::Timer(){
	
}

void Timer::set(int delai){
	this->delai=delai;
	date=clock();
}

bool Timer::estPret(){
	bool result=false;
	if (clock()>=date+delai){
		date=clock();
		result=true;
	}
	return result;
}