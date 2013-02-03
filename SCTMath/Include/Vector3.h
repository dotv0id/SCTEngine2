/**
----------------------------------------------------------------------------
	@File:			Vector3.h
   
	System:         SCT Math Library 
	Component Name: SCTMath Vector3
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

#ifndef VECTOR3_H
#define VECTOR3_H


#include <SCTMathDefines.h>


namespace Math
{
	/// Class Vector3
	/// ...
	class Vector3
	{
	public:
		/// Constructors, Destructor
		Vector3() : x (0), y (0), z (0) {}
		Vector3(float fX, float fY, float fZ) : x (fX), y (fY), z (fZ) {}
		Vector3(const Vector3 &vec) : x (vec.x), y (vec.y), z (vec.z) {}

		~Vector3();
		
		/// Assignment Operators
		inline void				operator = (const Vector3 &vec)
		{
			x	= vec.x;
			y	= vec.y;
			z	= vec.z;
		}

		inline void				operator = (float scalar)
		{
			x	= scalar;
			y	= scalar;
			z	= scalar;
		}

		/// With Vector3
		inline Vector3&			operator += (const Vector3 &vec)
		{
			x	+= vec.x;
			y	+= vec.y;
			z	+= vec.z;

			return *this;
		}

		inline Vector3&			operator -= (const Vector3 &vec)
		{
			x	-= vec.x;
			y	-= vec.y;
			z	-= vec.z;

			return *this;
		}

		inline Vector3&			operator /= (const Vector3 &vec)
		{
			x	/= vec.x;
			y	/= vec.y;
			z	/= vec.z;

			return *this;
		}

		inline Vector3&			operator *= (const Vector3 &vec)
		{
			x	*= vec.x;
			y	*= vec.y;
			z	*= vec.z;

			return *this;
		}

		/// With Scalar
		inline Vector3&			operator += (const float scalar)
		{
			x	+= scalar;
			y	+= scalar;
			z	+= scalar;

			return *this;
		}

		inline Vector3&			operator -= (const float scalar)
		{
			x	-= scalar;
			y	-= scalar;
			z	-= scalar;

			return *this;
		}

		inline Vector3&			operator /= (const float scalar)
		{
			x	/= scalar;
			y	/= scalar;
			z	/= scalar;

			return *this;
		}

		inline Vector3&			operator *= (const float scalar)
		{
			x	*= scalar;
			y	*= scalar;
			z	*= scalar;

			return *this;
		}

		/// Test for equality
		inline bool				operator == (const Vector3 &vec) const
		{
			 return (x == vec.x && y == vec.y && z == vec.z);
		}

		inline bool				operator != (const Vector3 &vec) const
		{
			 return (x != vec.x || y != vec.y || z != vec.z);
		}

		/// Arithmetic Operators 
		inline Vector3			operator + (const Vector3 &vec2) const
		{
			return Vector3(x + vec2.x, y + vec2.y, z + vec2.z);
		}

		inline Vector3			operator - (const Vector3 &vec2) const
		{
			return Vector3(x - vec2.x, y - vec2.y, z - vec2.z);
		}

		inline Vector3			operator / (const Vector3 &vec2) const
		{
			return Vector3(x / vec2.x, y / vec2.y, z / vec2.z);
		}

		inline Vector3			operator * (const Vector3 &vec2) const
		{
			return Vector3(x * vec2.x, y * vec2.y, z * vec2.z);
		}

		inline const Vector3&	operator + () const
        {
            return *this;
        }

        inline Vector3			operator - () const
        {
            return Vector3(-x, -y, -z);
        }

		/// Arithmetic Operations - with scalar
		inline Vector3			operator + (float scalar) const
		{
			return Vector3(x + scalar, y + scalar, z + scalar);
		}

		inline Vector3			operator - (float scalar) const
		{
			return Vector3(x - scalar, y - scalar, z - scalar);
		}

		inline Vector3			operator / (float scalar) const
		{
			return Vector3(x / scalar, y / scalar, z / scalar);
		}

		inline Vector3			operator * (float scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		/// ...
		inline float			LengthSquared() const
		{
			return (float)(x*x + y*y + z*z);
		}

		inline float			Length() const
		{
			return (float)sqrt(LengthSquared());
		}		

		inline void				Negate()
		{
			x	= -x;
			y	= -y;
			z	= -z;
		}

		inline void				Normalize()
		{
			float length	= Length();

			if(length <= 0.00001)
				length = 1.0f;

			length	= 1 / length;

			x *= length;
			y *= length;
			z *= length;
		}

		inline float			Dot(const Vector3 &vec2) const
		{
			return (float)(x * vec2.x + y * vec2.y + z * vec2.z);
		}

		inline Vector3			Cross(const Vector3 &vec2) const
		{
			return Vector3(	(y * vec2.z - z * vec2.y), 
							(z * vec2.x - x * vec2.z),
							(x * vec2.y - y * vec2.x));
		}

		/// Public Variables
		float			x,
						y,
						z;

	};
}	// End of Namespace Math


#endif