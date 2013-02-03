/**
----------------------------------------------------------------------------
	@File:			Quaternion.h
   
	System:         SCT Math Library 
	Component Name: Quaternion
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

#ifndef QUATERNION_H
#define QUATERNION_H


#include "SCTMathDefines.h"
#include "Matrix4.h"
#include "Vector3.h"

namespace Math
{
	/// Class Quaternion
	/// ...
	class Quaternion
	{
	public:
		/// Constructors, destructor
		inline				Quaternion() : x(0), y(0), z(0), w(0) {}
		inline				Quaternion(const Quaternion &rQ) : x(rQ.x), y(rQ.y), z(rQ.z), w(rQ.w) {}
		inline				Quaternion(float fX, float fY, float fZ, float fW) : x(fX), y(fY), z(fZ), w(fW) {}

		~Quaternion();
		
		/// Assignment operators
		inline void			operator = (const Quaternion &rQ)
		{
			x	= rQ.x;
			y	= rQ.y;
			z	= rQ.z;
			w	= rQ.w;
		}

		inline Quaternion&	operator += (const Quaternion &rQ)
		{
		}

		inline Quaternion&	operator -= (const Quaternion &rQ)
		{
		}

		inline Quaternion&	operator *= (const Quaternion &rQ)
		{
		}

		inline Quaternion&	operator *= (float scalar)
		{
		}

		inline Quaternion&	operator /= (float scalar)
		{
		}

		/// Arithmetic operations
		inline Quaternion	operator + (const Quaternion &rQ) const
		{
			return Quaternion(x + rQ.x, y + rQ.y, z + rQ.z, w + rQ.w);
		}

		inline Quaternion	operator - (const Quaternion &rQ) const
		{
			return Quaternion(x - rQ.x, y - rQ.y, z - rQ.z, w - rQ.w);
		}

		inline Quaternion	operator * (const Quaternion &rQ) const
		{
			Quaternion q;

			q.w = w * rQ.w - x * rQ.x - y * rQ.y - z * rQ.z;
			q.x	= w * rQ.x + x * rQ.w + y * rQ.z - z * rQ.y;
			q.y	= w * rQ.y + y * rQ.w + z * rQ.x - x * rQ.z;
			q.z	= w * rQ.z + z * rQ.w + x * rQ.y - y * rQ.x;

			return q;
		}

		/// With scalar
		inline Quaternion	operator * (float scalar) const
		{
			return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		inline friend Quaternion operator * (float scalar, const Quaternion &rQ)
		{
			return Quaternion(rQ.x * scalar, rQ.y * scalar, rQ.z * scalar, rQ.w * scalar);
		}

		/// With Vector
		inline Vector3	operator * (const Vector3 &vector)
		{
			Vector3 vn = vector;
			vn.Normalize();
			
			Quaternion vecQuat, resQuat;
			vecQuat.x = vn.x;
			vecQuat.y = vn.y;
			vecQuat.z = vn.z;
			vecQuat.w = 0.0f;
			
			resQuat = vecQuat * Quaternion(-x, -y, -z, w);
			resQuat = *this * resQuat;
			
			return Vector3(resQuat.x, resQuat.y, resQuat.z);
		}

		/// Unary operators
		inline Quaternion operator - () const
		{
			return Quaternion(-x, -y, -z, -w);
		}

		/// Test for equality
		inline bool			operator == (const Quaternion& rQ) const
		{
			return ( (rQ.x == x) && (rQ.y == y) && (rQ.z == z) && (rQ.w == w) );
		}

        inline bool			operator != (const Quaternion& rQ) const
		{
			return !operator==(rQ);
		}

		/// Quaternion functions
		inline void			SetFromRotationMatrix(const Matrix4 &rM)
		{
			float	trace, 
					sqrtTrace,
					sqrtTrace2;

			trace		= rM(1,1) + rM(2,2) + rM(3,3) + rM(4,4);
			sqrtTrace	= (float)sqrt(trace);

			sqrtTrace2	= 1.0f / (2.0f * sqrtTrace);

			w			= sqrtTrace * 0.5f;
			x			= (rM(2,3) - rM(3,2)) * sqrtTrace2;
			y			= (rM(3,1) - rM(1,3)) * sqrtTrace2;
			z			= (rM(1,2) - rM(2,1)) * sqrtTrace2;
		}

		inline void			GetRotationMatrix(Matrix4 &rM) const
		{
			rM(1,1)  = 1.0f - 2.0f * (y * y + z * z); 
			rM(1,2)  = 2.0f * (x * y + z * w);
			rM(1,3)  = 2.0f * (x * z - y * w);
			rM(1,4)  = 0.0f;  

			rM(2,1)  = 2.0f * (x * y - z * w);  
			rM(2,2)  = 1.0f - 2.0f * (x * x + z * z); 
			rM(2,3)  = 2.0f * (z * y + x * w);  
			rM(2,4)  = 0.0f;  

			rM(3,1)  = 2.0f * (x * z + y * w);
			rM(3,2)  = 2.0f * (y * z - x * w);
			rM(3,3) = 1.0f - 2.0f * (x * x + y * y);  
			rM(3,4) = 0.0f;  

			rM(4,1) = 0;  
			rM(4,2) = 0;  
			rM(4,3) = 0;  
			rM(4,4) = 1.0f;
		}

		inline void			SetFromAxisAngle(float angle, const Vector3 &axis)
		{
			// The quaternion representing the rotation is
			// q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		    float halfAngle	= (float)(DEG_TO_RAD * angle * 0.5f);
	        float s			= sin(halfAngle);

			w	= cos(halfAngle);
			x	= s * axis.x;
			y	= s * axis.y;
			z	= s * axis.z;
		}

		inline void			GetAngleAxis(float &angle, Vector3 &axis) const
		{
			float squareLength	= x*x+y*y+z*z;
			if (squareLength > 0.0)
			{
		        angle			= (float)(2.0 * acos(w));

				float invLength	= 1 / sqrt(squareLength);
				axis.x			= x * invLength;
				axis.y			= y * invLength;
				axis.z			= z * invLength;
		    }
		    else
			{
				// angle is 0 (mod 2*pi), so any axis will do
				angle	= 0;
				axis.x	= 1.0f;
				axis.y	= 0;
				axis.z	= 0;
			}
		}

		inline void				SetRotationYawPitchRoll(float yaw, float pitch, float roll)
		{
			float cosX = 0.5f * cosf(pitch);
			float cosY = 0.5f * cosf(yaw);
			float cosZ = 0.5f * cosf(roll);
   
			float sinX = 0.5f * sinf(pitch);
			float sinY = 0.5f * sinf(yaw);
			float sinZ = 0.5f * sinf(roll);

			w = cosZ * cosY * cosX + sinZ * sinY * sinX;
			x = cosZ * cosY * sinX - sinZ * sinY * cosX;
			y = cosZ * sinY * cosX + sinZ * cosY * sinX;
			z = sinZ * cosY * cosX - cosZ * sinY * sinX;
		}

		inline float			Dot(const Quaternion &rQ) const
		{
			return (w*rQ.w + x*rQ.x + y*rQ.y + z*rQ.z);
		}

		inline float			SquaredLength() const
		{
			return (w*w + x*x + y*y + z*z);
		}

		inline void				Normalize()
		{
			float len		= SquaredLength();
			float factor	= 1.0f / sqrt(len);
			*this			= *this * factor;
		}

		inline void				Inverse()
		{
			float norm = w*w + x*x + y*y + z*z;
			if (norm > 0.0)
			{
				float invNorm	= 1.0f / norm;
				
				w				=  w*invNorm;
				x				= -x*invNorm;
				y				= -y*invNorm;
				z				= -z*invNorm;
			}
			else
			{
				w	= 0;
				x	= 0;
				y	= 0;
				z	= 0;
		    }
		}

		inline void				Exp()
		{
		}

		/// Public Variables
		float		x, 
					y, 
					z, 
					w;
	};
}	// End of Namespace Math

#endif