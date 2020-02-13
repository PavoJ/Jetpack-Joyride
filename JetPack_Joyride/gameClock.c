#include<time.h>
#include<stdbool.h>
#include<stdlib.h>

#include<stdio.h>

#define tick (CLOCKS_PER_SEC)/60

bool elapsedTick()
{
	static clock_t clk;
	static bool firstTime;

	bool ret;

	if (!firstTime)
	{
		clk = clock();
		firstTime = true;
	}


	//se è passato 1 tick
	if ((clock() - clk) > tick)
	{
		clk = clock();//resetto l'orologio
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;
}