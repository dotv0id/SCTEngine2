#include <Frustum.h>
#include <SCTMathDebug.h>


namespace Math
{

//float angle, float ratio, float near, float far, Vector3 &camPos, Vector3 &lookAt, Vector3 &up
void Frustum::Calculate(float depth, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
{
	float zMinimum, r;
	Matrix4 matrix, projection (projectionMatrix), view (viewMatrix);

	
	// Calculate the minimum Z distance in the frustum.
	zMinimum		= -projectionMatrix(4,3) / projectionMatrix(3,3);
	r				= depth / (depth - zMinimum);
	projection(3,3) = r;
	projection(4,3) = -r * zMinimum;

	matrix = view * projection;
	
	// Clear the frustrum & add the 6 planes
	mFrustum.clear();

	Plane near;
	near.a = matrix(1,4) + matrix(1,3);
	near.b = matrix(2,4) + matrix(2,3);
	near.c = matrix(3,4) + matrix(3,3);
	near.d = matrix(4,4) + matrix(4,3);
	mFrustum.push_back(near);

	Plane far;
	far.a = matrix(1,4) - matrix(1,3); 
	far.b = matrix(2,4) - matrix(2,3);
	far.c = matrix(3,4) - matrix(3,3);
	far.d = matrix(4,4) - matrix(4,3);
	mFrustum.push_back(far);

	Plane left;
	left.a = matrix(1,4) + matrix(1,1); 
	left.b = matrix(2,4) + matrix(2,1);
	left.c = matrix(3,4) + matrix(3,1);
	left.d = matrix(4,4) + matrix(4,1);
	mFrustum.push_back(left);

	Plane right;
	right.a = matrix(1,4) - matrix(1,1); 
	right.b = matrix(2,4) - matrix(2,1);
	right.c = matrix(3,4) - matrix(3,1);
	right.d = matrix(4,4) - matrix(4,1);
	mFrustum.push_back(right);

	Plane top;
	top.a = matrix(1,4) - matrix(1,2); 
	top.b = matrix(2,4) - matrix(2,2);
	top.c = matrix(3,4) - matrix(3,2);
	top.d = matrix(4,4) - matrix(4,2);
	mFrustum.push_back(top);

	Plane bottom;
	bottom.a = matrix(1,4) + matrix(1,2);
	bottom.b = matrix(2,4) + matrix(2,2);
	bottom.c = matrix(3,4) + matrix(3,2);
	bottom.d = matrix(4,4) + matrix(4,2);
	mFrustum.push_back(bottom);
		
}

void Frustum::Calculate(float angle, float ratio, float near, float far, Vector3 &camPos, Vector3 &lookAt, Vector3 &up)
{
	Vector3		xVec, yVec, zVec;
	Vector3		vecN, vecF;
	Vector3		nearTopLeft, nearTopRight,
				nearBottomLeft, nearBottomRight;
	Vector3		farTopLeft, farTopRight,
				farBottomLeft, farBottomRight;

	float radians	= (float)tan( (angle)  );
	float nearH		= near  * radians;
	float nearW		= nearH * ratio;
	float farH		= far   * radians;
	float farW		= farH  * ratio;

	zVec = camPos - lookAt;
	zVec.Normalize();

	xVec = up.Cross(zVec);
	xVec.Normalize();

	yVec = zVec.Cross(xVec);

	vecN = camPos - zVec * near;
	vecF = camPos - zVec * far;

	nearTopLeft     = vecN + yVec * nearH - xVec * nearW;
	nearTopRight    = vecN + yVec * nearH + xVec * nearW;
	nearBottomLeft  = vecN - yVec * nearH - xVec * nearW;
	nearBottomRight = vecN - yVec * nearH + xVec * nearW;

	farTopLeft      = vecF + yVec * farH - xVec * farW;
	farTopRight     = vecF + yVec * farH + xVec * farW;
	farBottomLeft   = vecF - yVec * farH - xVec * farW;
	farBottomRight  = vecF - yVec * farH + xVec * farW;

	//DumpVector3(nearTopLeft);
	//DumpVector3(nearTopRight);
	//DumpVector3(nearBottomLeft);
	//DumpVector3(nearBottomRight);
	//std::cout<<"------------------------\n";

	// Clear the frustrum & add the 6 planes
	mFrustum.clear();

	Plane top(nearTopRight, nearTopLeft, farTopLeft);
	mFrustum.push_back(top);

	Plane bottom(nearBottomLeft, nearBottomRight, farBottomRight);
	mFrustum.push_back(bottom);

	Plane left(nearTopLeft, nearBottomLeft, farBottomLeft);
	mFrustum.push_back(left);

	Plane right(nearBottomRight, nearTopRight, farBottomRight);
	mFrustum.push_back(right);

	Plane nearPlane(nearTopLeft, nearTopRight, nearBottomRight);
	mFrustum.push_back(nearPlane);

	Plane farPlane(farTopRight, farTopLeft, farBottomLeft);
	mFrustum.push_back(farPlane);
}

bool Frustum::IsVisiblePoint(float x, float y, float z)
{
	for(int i = 0; i < (int)mFrustum.size(); i++)
	{
		if(mFrustum[i].GetDistance(x, y, z) < 0)
			return false;
	}

	return true;
}

bool Frustum::IsVisibleAABB(const BoundingBox &aabb)
{
	Vector3 min (aabb.mMin), max (aabb.mMax);

	if(IsVisiblePoint(min.x, min.y, min.z))
		return true;

	if(IsVisiblePoint(max.x, min.y, min.z))
		return true;

	if(IsVisiblePoint(min.x, max.y, min.z))
		return true;

	if(IsVisiblePoint(max.x, max.y, min.z))
		return true;

	if(IsVisiblePoint(min.x, min.y, max.z))
		return true;

	if(IsVisiblePoint(max.x, min.y, max.z))
		return true;

	if(IsVisiblePoint(min.x, max.y, max.z))
		return true;

	if(IsVisiblePoint(max.x, max.y, max.z))
		return true;

	return false;
}

}	// End of Math Namespace