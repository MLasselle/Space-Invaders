#ifndef TIMER_H
#define TIMER_H

#include <time.h>

class Timer{
	clock_t date;
	int delai;
public:
	Timer();
	void set(int delai);
	bool estPret();
};

#endif