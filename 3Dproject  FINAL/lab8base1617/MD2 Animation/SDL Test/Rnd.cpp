#include<ctime>
#include<cstdlib>
#include "Rnd.h"
void start_random()
{
	static bool seeded = false;
	if (!seeded) {
		srand((unsigned)time(NULL));
		seeded = true;
	}
}



float rnd(float rangemin, float rangemax)			// random float within supplied range...
{
	start_random();
	return rangemin + (float)rand()*(rangemax - rangemin) / 32768.0;
}


