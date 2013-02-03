#include <SCTMath.h>


namespace Math
{
	float GetRandom(Range r)
	{
		return r.min + (r.max - r.min) * ( (float)rand()) / ((float)RAND_MAX );
	}

	float GetRandom(float min, float max)
	{
		return min + (max - min) * ( (float)rand()) / ((float)RAND_MAX );
	}

	
}