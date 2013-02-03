#include <BoundingBox.h>


namespace Math
{

void BoundingBox::Calculate(Vector3 *vertices, unsigned int numVertices)
{
	for(unsigned int i = 0; i < numVertices; i++)
	{
		if(vertices[i].x < mMin.x) mMin.x = vertices[i].x;
		if(vertices[i].x > mMax.x) mMax.x = vertices[i].x;
									
		if(vertices[i].y < mMin.y) mMin.y = vertices[i].y;
		if(vertices[i].y > mMax.y) mMax.y = vertices[i].y;
									
		if(vertices[i].z < mMin.z) mMin.z = vertices[i].z;
		if(vertices[i].z > mMax.z) mMax.z = vertices[i].z;
	}

	mCenter.x = (mMin.x + mMax.x) * 0.5f;
	mCenter.y = (mMin.y + mMax.y) * 0.5f;
	mCenter.z = (mMin.z + mMax.z) * 0.5f;
}

void BoundingBox::Translate(const Vector3 &translation)
{
	mMin		+= translation;
	mMax		+= translation;
	mCenter		+= translation;
}

bool BoundingBox::CheckIntersection(const BoundingBox &aabb)
{
	return false;
}

bool BoundingBox::CheckIntersection(const Vector3 &vector)
{
	return false;
}

}	// End of Math Namespace