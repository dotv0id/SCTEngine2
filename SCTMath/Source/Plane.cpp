#include <Plane.h>
#include <iostream>

namespace Math
{

bool Plane::CheckIntersectionAABB(const BoundingBox &aabb)
{
	Vector3		bbMin (aabb.mMin), bbMax (aabb.mMax);
	Vector3		min, max, normal(a, b, c);

	if(normal.x >= 0.0f)
	{
		min.x = bbMin.x;
		max.x = bbMax.x;
	}
	else
	{
		min.x = bbMax.x;
		max.x = bbMin.x;
	}
   
	if(normal.y >= 0.0f)
	{
		min.y = bbMin.y;
		max.y = bbMax.y;
	}
	else
	{
		min.y = bbMax.y;
		max.y = bbMin.y;
	}
   
	if(normal.z >= 0.0f)
	{
		min.z = bbMin.z;
		max.z = bbMax.z;
	}
	else
	{
		min.z = bbMax.z;
		max.z = bbMin.z;
	}
   
	if( (normal.Dot(min) + d) > 0.0f )
		return false;

	if( (normal.Dot(max) + d) >= 0.0f )
	{
		//std::cout<<"Intersection!\n";
		return true;
	}

	return false;
}

}	// End of Math Namespace