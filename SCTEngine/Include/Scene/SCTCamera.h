/**
----------------------------------------------------------------------------
	@File:			SCTCamera.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Camera 
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Header file for Project X
					This file contains the defined types for Project X
					This is sometimes called the "Abstract" and may be
					followed by a section called "Notes".
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/

#ifndef SCT_CAMERA_H
#define SCT_CAMERA_H


#include <SCT.h>
#include <SCTMath.h>
#include <SCTPlatform.h>


namespace SCT
{
	namespace Scene
	{
		/// Class SCTCamera
		//  ...
		class SCTCamera
		{
		public:
			SCTCamera();
			virtual						~SCTCamera() { Shutdown(); };

			SCTReturn					Update();
			SCTReturn					Shutdown();
			void						Reset();

			// Camera functions
			void						Move(float distance);
			void						MovePerpendicular(float distanceX, float distanceY);
			void						RotateYaw(float angle);
			void						RotatePitch(float angle);
			void						RotateRoll(float angle);
			inline void					UseFrustum(bool mode)
			{
				mbFrustumEnabled = mode;
			}

			inline bool					IsFrustumEnabled()
			{
				return mbFrustumEnabled;
			}

			// Set, Get Functions
			inline void					SetPosition(const Math::Vector3 &position)
			{
				mPosition	= position;
			}

			inline void					SetPosition(float x, float y, float z)
			{
				mPosition	= Math::Vector3(x, y, z);
			}

			inline void					SetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
			{
				mProjectionMatrix.SetMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
				mInvProjectionMatrix	= mProjectionMatrix.Inverse();

				mAspectRatio	= aspectRatio;
				mFovAngle		= fov;
				mNearPlane		= nearPlane;
				mFarPlane		= farPlane;
			}

			inline Math::Matrix4&		GetViewMatrix()
			{
				return mViewMatrix;
			}

			inline Math::Matrix4&		GetProjectionMatrix()
			{
				return mProjectionMatrix;
			}

			inline Math::Matrix4&		GetInverseProjectionMatrix()
			{
				return mInvProjectionMatrix;
			}

			inline Math::Vector3&		GetPosition()
			{
				return mPosition;
			}

			inline Math::Quaternion&	GetRotation()
			{
				return mRotation;
			}

			inline Math::Frustum&		GetFrustum()
			{
				return mFrustum;
			}

		protected:

		private:
			Math::Quaternion		mRotation;			// Will hold the rotation
			Math::Matrix4			mViewMatrix,
									mProjectionMatrix,
									mInvProjectionMatrix;

			// Camera Properties
			Math::Vector3			mPosition;
			
			Math::Vector3			mLookAt,
									mRight,
									mUp;

			// Frustum
			Math::Frustum			mFrustum;
			bool					mbFrustumEnabled;
			float					mAspectRatio,
									mFovAngle,
									mNearPlane,
									mFarPlane;
			
			

			float	yaw, pitch;
		};
	}	// End of Scene Namespace
}	// End of SCT Namespace

#endif