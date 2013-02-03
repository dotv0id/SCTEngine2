#include <Scene\SCTCamera.h>
#include <SCTMathDebug.h>


namespace SCT
{

using namespace Math;

namespace Scene
{

SCTCamera::SCTCamera()
:	mPosition (Vector3(0, 0, 0)),
	mLookAt (Vector3(0, 0, 1.0f)),
	mRight (Vector3(1.0f, 0, 0)),
	mUp (Vector3(0, 1.0f, 0)),
	mbFrustumEnabled (false),
	mAspectRatio ((float)800/600),
	mFovAngle ((float)Math::HALFPI/2),
	mNearPlane (1.0f),
	mFarPlane (1000.0f)
{
	// Initialize the orientation quaternion
	mRotation.SetFromAxisAngle(0, Vector3(0, 1.0f, 0));

	// Default perspective projection matrix
	mProjectionMatrix.SetMatrixPerspectiveFovLH(mFovAngle, mAspectRatio, mNearPlane, mFarPlane);
	mInvProjectionMatrix	= mProjectionMatrix.Inverse();

	yaw		= 0;
	pitch	= 0;
}

SCTReturn SCTCamera::Update()
{
	// Update the camera view matrix every frame
	Vector3	lookAt;

	Math::Quaternion rot;
	mRotation.SetRotationYawPitchRoll(yaw, pitch, 0);

	mLookAt	= mRotation * Math::Vector3(0, 0, 1.0f);
	mUp		= mRotation * Math::Vector3(0, 1.0f, 0);
	mRight	= mRotation * Math::Vector3(1.0f, 0, 0);

	mLookAt.Normalize();
	mUp.Normalize();
	mRight.Normalize();

	// Transform LookAt vector by camera position
	lookAt	= mLookAt + mPosition;

	// Reconstruct the Camera's view matrix
	mViewMatrix.SetViewMatrixLH(mPosition, lookAt, mUp);

	// Recalculte the Frustum
	if(mbFrustumEnabled)
	{
		//mFrustum.Calculate(5000.0f, mViewMatrix, mProjectionMatrix);
		mFrustum.Calculate(mFovAngle, mAspectRatio, mNearPlane, mFarPlane, mPosition, lookAt, mUp);
	}
	// ! DEBUG 
	//Math::DumpMatrix4(rotation);
	//Math::DumpVector3(lookAt);

	return OK;
}

SCTReturn SCTCamera::Shutdown()
{
	return OK;
}

void SCTCamera::Reset()
{
	mPosition	= Vector3(0, 0, -10.0f);
	mLookAt		= Vector3(0, 0, 1.0f);
	mRight		= Vector3(1.0f, 0, 0);
	mUp			= Vector3(0, 1.0f, 0);
}

void SCTCamera::Move(float distance)
{
	mPosition += mLookAt * distance;
}

void SCTCamera::MovePerpendicular(float distanceX, float distanceY)
{
	mPosition += mRight * distanceX;
	mPosition += mUp * distanceY;
}

void SCTCamera::RotateYaw(float angle)
{
	yaw += angle;
}

void SCTCamera::RotatePitch(float angle)
{
	pitch += angle;
}

void SCTCamera::RotateRoll(float angle)
{

}

}	// End of Scene Namespace
}	// End oc SCT Namespace