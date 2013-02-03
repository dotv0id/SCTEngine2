#include <Ray.h>


namespace Math
{

// Checks if the ray intersects with a triangle defined by points p1, p2, p3
// Returns true if it intersects and the 't' distace of the intersection point
// from the ray origin 
bool Ray::CheckIntersection(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, float &t)
{
	Math::Vector3	vecAB = p2 - p1;
	Math::Vector3	vecAC = p3 - p1;
	
	Math::Vector3 cross;
	cross = mDirection.Cross(vecAC);
	
	float det = vecAB.Dot(cross);
	
	/*
	if(cull && det < 0.0001f)
	{
	   return false;
	}
	else
		*/
	if(det < 0.0001f && det > -0.0001f)
		return false;
	
	Math::Vector3 rayPointVec = mOrigin - p1;
	float test1 = rayPointVec.Dot(cross);
	
	if(test1 < 0.0f || test1 > det)
	   return false;
	
	Math::Vector3 cross2;
	cross2 = rayPointVec.Cross(vecAB);
	float test2 = mDirection.Dot(cross2);
	
	if(test2 < 0.0f || test1 + test2 > det)
	   return false;
	
	float inverseDet = 1.0f / det;
	
	t = vecAC.Dot(cross2);
	t *= inverseDet;
	
	return true;
}

bool Ray::CheckIntersection(const BoundingBox &aabb, Vector3 &point)
{
	Vector3 maxVal(-1, -1, -1);
	Vector3 bbMin(aabb.mMin), bbMax(aabb.mMax);
	bool	rayInside = false; // false

	// X check
	if(mOrigin.x < bbMin.x)
	{
		point.x		= bbMin.x;
		rayInside	= false;

		if(mDirection.x != 0)
			maxVal.x = (bbMin.x - mOrigin.x) / mDirection.x;
	}
	else if(mOrigin.x > bbMax.x)
	{
		point.x		= bbMax.x;
		rayInside	= false;

		if(mDirection.x != 0)
			maxVal.x = (bbMax.x - mOrigin.x) / mDirection.x;
	}

	// Y check
	if(mOrigin.y < bbMin.y)
	{
		point.y		= bbMin.y;
		rayInside	= false;

		if(mDirection.y != 0)
			maxVal.y = (bbMin.y - mOrigin.y) / mDirection.y;
	}
	else if(mOrigin.y > bbMax.y)
	{
		point.y		= bbMax.y;
		rayInside	= false;

		if(mDirection.y != 0)
			maxVal.y = (bbMax.y - mOrigin.y) / mDirection.y;
	}

	// Z check
	if(mOrigin.z < bbMin.z)
	{
		point.z		= bbMin.z;
		rayInside	= false;

		if(mDirection.z != 0)
			maxVal.z = (bbMin.z - mOrigin.z) / mDirection.z;
	}
	else if(mOrigin.z > bbMax.z)
	{
		point.z		= bbMax.z;
		rayInside	= false;

		if(mDirection.z != 0)
			maxVal.z = (bbMax.z - mOrigin.z) / mDirection.z;
	}

	if(rayInside)
	{
		point = mOrigin;
		return true;
	}

	int index = 0;
	float temp[3];
	temp[0] = maxVal.x; temp[1] = maxVal.y; temp[2] = maxVal.z;

	if(maxVal.y > temp[index])
		index = 1;

	if(maxVal.z > temp[index])
		index = 2;

	if(temp[index] < 0)
		return false;
   
	if(index != 0)
	{
		point.x = mOrigin.x + maxVal.x * mDirection.x;

		if(point.x < bbMin.x - 0.00001f || point.x < bbMax.x + 0.00001f)
			return false;
	}

	if(index != 1)
	{
		point.y = mOrigin.y + maxVal.y * mDirection.y;

		if(point.y < bbMin.y - 0.00001f || point.y < bbMax.y + 0.00001f)
			return false;
	}

	if(index != 2)
	{
		point.z = mOrigin.z + maxVal.z * mDirection.z;

		if(point.z < bbMin.z - 0.00001f || point.z < bbMax.z + 0.00001f)
			return false;
	}

	return true;
}

bool Ray::CheckIntersection(Vector3 &pos, float radius, float &dist)
{
   Vector3 RayToSphereDir;
   float RayToSphereLength = 0.0f;
   float IntersectPoint = 0.0f;
   float SquaredPoint = 0.0f;

   RayToSphereDir = pos - mOrigin;
   RayToSphereLength = RayToSphereDir.Dot(RayToSphereDir);

   IntersectPoint = RayToSphereDir.Dot(mDirection);

   if(IntersectPoint < 0 )
      return false;

   SquaredPoint = (radius * radius) - RayToSphereLength +
                  (IntersectPoint * IntersectPoint);

   if(SquaredPoint < 0)
      return false;

    dist = IntersectPoint - (float)sqrt(SquaredPoint);

   return true;
}

}	// End of Math Namespace