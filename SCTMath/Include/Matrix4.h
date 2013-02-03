/**
----------------------------------------------------------------------------
	@File: Matrix4.h
   
	System:         SCTMath Library 
	Component Name: SCTMath Matrix4x4
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


#ifndef MATRIX4_H
#define MATRIX4_H


#include "SCTMathDefines.h"
#include "Vector3.h"


namespace Math
{
	/// Class Matrix4
	/// ...
	class Matrix4
	{
	public:
		Matrix4()
		{
			m[0][0] = 1.0f;  m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
			m[1][0] = 0;	 m[1][1] = 1.0f;	m[1][2] = 0;		m[1][3] = 0;
			m[2][0] = 0;	 m[2][1] = 0;		m[2][2] = 1.0f;		m[2][3] = 0;
			m[3][0] = 0;	 m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1.0f;
		}

		Matrix4(float f11, float f12, float f13, float f14,
									float f21, float f22, float f23, float f24,
									float f31, float f32, float f33, float f34,
									float f41, float f42, float f43, float f44)
		{
			m[0][0] = f11;  m[0][1] = f12;	m[0][2] = f13;	m[0][3] = f14;
			m[1][0] = f21;	m[1][1] = f22;  m[1][2] = f23;	m[1][3] = f24;
			m[2][0] = f31;	m[2][1] = f32;	m[2][2] = f33;	m[2][3] = f34;
			m[3][0] = f41;	m[3][1] = f42;	m[3][2] = f43;	m[3][3] = f44;
		}

		Matrix4(const Matrix4 &rM)
		{
			m[0][0] = rM(1, 1); m[0][1] = rM(1, 2); m[0][2] = rM(1, 3); m[0][3] = rM(1, 4);
			m[1][0] = rM(2, 1); m[1][1] = rM(2, 2); m[1][2] = rM(2, 3); m[1][3] = rM(2, 4);
			m[2][0] = rM(3, 1); m[2][1] = rM(3, 2); m[2][2] = rM(3, 3); m[2][3] = rM(3, 4);
			m[3][0] = rM(4, 1); m[3][1] = rM(4, 2); m[3][2] = rM(4, 3); m[3][3] = rM(4, 4);
		}

		~Matrix4();

		/// Public Functions
		/// Access Grants
		inline float		&operator()(unsigned int Row, unsigned int Column)
		{
			return m[Row - 1][Column - 1];
		}

		inline float const	&operator()(unsigned int Row, unsigned int Column) const
		{
			return m[Row - 1][Column - 1];
		}

		/// Assignment Operators with matrix
		inline void			operator = (const Matrix4 &rM)
		{
			m[0][0] = rM(1, 1); m[0][1] = rM(1, 2); m[0][2] = rM(1, 3); m[0][3] = rM(1, 4);
			m[1][0] = rM(2, 1); m[1][1] = rM(2, 2); m[1][2] = rM(2, 3); m[1][3] = rM(2, 4);
			m[2][0] = rM(3, 1); m[2][1] = rM(3, 2); m[2][2] = rM(3, 3); m[2][3] = rM(3, 4);
			m[3][0] = rM(4, 1); m[3][1] = rM(4, 2); m[3][2] = rM(4, 3); m[3][3] = rM(4, 4);
		}

		inline Matrix4&		operator *= (const Matrix4 &rM)
		{
			(*this) = (*this) * rM;

			return *this;
		}

		inline Matrix4&		operator += (const Matrix4 &rM)
		{
			m[0][0] += rM(1, 1); m[0][1] += rM(1, 2); m[0][2] += rM(1, 3); m[0][3] += rM(1, 4);
			m[1][0] += rM(2, 1); m[1][1] += rM(2, 2); m[1][2] += rM(2, 3); m[1][3] += rM(2, 4);
			m[2][0] += rM(3, 1); m[2][1] += rM(3, 2); m[2][2] += rM(3, 3); m[2][3] += rM(3, 4);
			m[3][0] += rM(4, 1); m[3][1] += rM(4, 2); m[3][2] += rM(4, 3); m[3][3] += rM(4, 4);

			return *this;
		}

		inline Matrix4&		operator -= (const Matrix4 &rM)
		{
			m[0][0] -= rM(1, 1); m[0][1] -= rM(1, 2); m[0][2] -= rM(1, 3); m[0][3] -= rM(1, 4);
			m[1][0] -= rM(2, 1); m[1][1] -= rM(2, 2); m[1][2] -= rM(2, 3); m[1][3] -= rM(2, 4);
			m[2][0] -= rM(3, 1); m[2][1] -= rM(3, 2); m[2][2] -= rM(3, 3); m[2][3] -= rM(3, 4);
			m[3][0] -= rM(4, 1); m[3][1] -= rM(4, 2); m[3][2] -= rM(4, 3); m[3][3] -= rM(4, 4);

			return *this;
		}

		/// Assignment Operators with Scalar
		inline Matrix4&		operator *= (float scalar)
		{
			m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar; m[0][3] *= scalar;
			m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar; m[1][3] *= scalar;
			m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar; m[2][3] *= scalar;
			m[3][0] *= scalar; m[3][1] *= scalar; m[3][2] *= scalar; m[3][3] *= scalar;

			return *this;
		}

		inline Matrix4&		operator /= (float scalar)
		{
			m[0][0] /= scalar; m[0][1] /= scalar; m[0][2] /= scalar; m[0][3] /= scalar;
			m[1][0] /= scalar; m[1][1] /= scalar; m[1][2] /= scalar; m[1][3] /= scalar;
			m[2][0] /= scalar; m[2][1] /= scalar; m[2][2] /= scalar; m[2][3] /= scalar;
			m[3][0] /= scalar; m[3][1] /= scalar; m[3][2] /= scalar; m[3][3] /= scalar;

			return *this;
		}

		/// Arithmetic Operators
		inline Matrix4 operator * (const Matrix4 &rM2)
		{
			Matrix4 r;

			// Row 1
			r(1,1)	= m[0][0]*rM2(1,1) + m[0][1]*rM2(2,1)  + m[0][2]*rM2(3,1) + m[0][3]*rM2(4,1);
			r(1,2)	= m[0][0]*rM2(1,2) + m[0][1]*rM2(2,2)  + m[0][2]*rM2(3,2) + m[0][3]*rM2(4,2);
			r(1,3)	= m[0][0]*rM2(1,3) + m[0][1]*rM2(2,3)  + m[0][2]*rM2(3,3) + m[0][3]*rM2(4,3);
			r(1,4)	= m[0][0]*rM2(1,4) + m[0][1]*rM2(2,4)  + m[0][2]*rM2(3,4) + m[0][3]*rM2(4,4);

			// Row 2
			r(2,1)	= m[1][0]*rM2(1,1) + m[1][1]*rM2(2,1)  + m[1][2]*rM2(3,1) + m[1][3]*rM2(4,1);
			r(2,2)	= m[1][0]*rM2(1,2) + m[1][1]*rM2(2,2)  + m[1][2]*rM2(3,2) + m[1][3]*rM2(4,2);
			r(2,3)	= m[1][0]*rM2(1,3) + m[1][1]*rM2(2,3)  + m[1][2]*rM2(3,3) + m[1][3]*rM2(4,3);
			r(2,4)	= m[1][0]*rM2(1,4) + m[1][1]*rM2(2,4)  + m[1][2]*rM2(3,4) + m[1][3]*rM2(4,4);

			// Row 3
			r(3,1)	= m[2][0]*rM2(1,1) + m[2][1]*rM2(2,1)  + m[2][2]*rM2(3,1) + m[2][3]*rM2(4,1);
			r(3,2)	= m[2][0]*rM2(1,2) + m[2][1]*rM2(2,2)  + m[2][2]*rM2(3,2) + m[2][3]*rM2(4,2);
			r(3,3)	= m[2][0]*rM2(1,3) + m[2][1]*rM2(2,3)  + m[2][2]*rM2(3,3) + m[2][3]*rM2(4,3);
			r(3,4)	= m[2][0]*rM2(1,4) + m[2][1]*rM2(2,4)  + m[2][2]*rM2(3,4) + m[2][3]*rM2(4,4);

			// Row 4
			r(4,1)	= m[3][0]*rM2(1,1) + m[3][1]*rM2(2,1)  + m[3][2]*rM2(3,1) + m[3][3]*rM2(4,1);
			r(4,2)	= m[3][0]*rM2(1,2) + m[3][1]*rM2(2,2)  + m[3][2]*rM2(3,2) + m[3][3]*rM2(4,2);
			r(4,3)	= m[3][0]*rM2(1,3) + m[3][1]*rM2(2,3)  + m[3][2]*rM2(3,3) + m[3][3]*rM2(4,3);
			r(4,4)	= m[3][0]*rM2(1,4) + m[3][1]*rM2(2,4)  + m[3][2]*rM2(3,4) + m[3][3]*rM2(4,4);

			return r;
		}

		inline Matrix4 operator + (const Matrix4 &rM2)
		{
			return Matrix4( m[0][0] + rM2(1, 1), m[0][1] + rM2(1, 2), m[0][2] + rM2(1, 3), m[0][3] + rM2(1, 4),
							m[1][0] + rM2(2, 1), m[1][1] + rM2(2, 2), m[1][2] + rM2(2, 3), m[1][3] + rM2(2, 4),
							m[2][0] + rM2(3, 1), m[2][1] + rM2(3, 2), m[2][2] + rM2(3, 3), m[2][3] + rM2(3, 4),
							m[3][0] + rM2(4, 1), m[3][1] + rM2(4, 2), m[3][2] + rM2(4, 3), m[3][3] + rM2(4, 4) );
		}

		inline Matrix4 operator - (const Matrix4 &rM2)
		{
			return Matrix4( m[0][0] - rM2(1, 1), m[0][1] - rM2(1, 2), m[0][2] - rM2(1, 3), m[0][3] - rM2(1, 4),
							m[1][0] - rM2(2, 1), m[1][1] - rM2(2, 2), m[1][2] - rM2(2, 3), m[1][3] - rM2(2, 4),
							m[2][0] - rM2(3, 1), m[2][1] - rM2(3, 2), m[2][2] - rM2(3, 3), m[2][3] - rM2(3, 4),
							m[3][0] - rM2(4, 1), m[3][1] - rM2(4, 2), m[3][2] - rM2(4, 3), m[3][3] - rM2(4, 4) );
		}

		inline Matrix4 operator * (float scalar)
		{
			return Matrix4( m[0][0] * scalar, m[0][1] * scalar, m[0][2] * scalar, m[0][3] * scalar,
							m[1][0] * scalar, m[1][1] * scalar, m[1][2] * scalar, m[1][3] * scalar,
							m[2][0] * scalar, m[2][1] * scalar, m[2][2] * scalar, m[2][3] * scalar,
							m[3][0] * scalar, m[3][1] * scalar, m[3][2] * scalar, m[3][3] * scalar );
		}

		inline Matrix4 operator / (float scalar)
		{
			return Matrix4( m[0][0] / scalar, m[0][1] / scalar, m[0][2] / scalar, m[0][3] / scalar,
							m[1][0] / scalar, m[1][1] / scalar, m[1][2] / scalar, m[1][3] / scalar,
							m[2][0] / scalar, m[2][1] / scalar, m[2][2] / scalar, m[2][3] / scalar,
							m[3][0] / scalar, m[3][1] / scalar, m[3][2] / scalar, m[3][3] / scalar );
		}

		/// Arithmetic operations with Vector3
		inline Vector3			operator * (const Vector3 &vec)
		{
			float wInv	= 1 / (m[3][0]*vec.x + m[3][1]*vec.y + m[3][2]*vec.z + m[3][3]);
			
			return Vector3(	(m[0][0]*vec.x + m[0][1]*vec.y + m[0][2]*vec.z + m[0][3]) * wInv,
							(m[1][0]*vec.x + m[1][1]*vec.y + m[1][2]*vec.z + m[1][3]) * wInv,
							(m[2][0]*vec.x + m[2][1]*vec.y + m[2][2]*vec.z + m[2][3]) * wInv );
		}

		inline friend Vector3	operator * (const Vector3 &vec, const Matrix4 &rM)
		{
			float wInv	= 1 / (vec.x*rM(1,4) + vec.y*rM(2,4) + vec.z*rM(3,4) + rM(4,4));
			
			return Vector3(	(vec.x*rM(1,1) + vec.y*rM(2,1) + vec.z*rM(3,1) + rM(4,1)) * wInv,
							(vec.x*rM(1,2) + vec.y*rM(2,2) + vec.z*rM(3,2) + rM(4,2)) * wInv,
							(vec.x*rM(1,3) + vec.y*rM(2,3) + vec.z*rM(3,3) + rM(4,3)) * wInv );
		}

		/// Test two matrices for equality
		inline bool		operator == (const Matrix4 &m)
		{
			return false;
		}

		inline bool		operator != (const Matrix4 &m)
		{
			return false;
		}


		/// Matrix4 functions
		inline void			Identity()
		{
			m[0][0] = 1.0f;  m[0][1] = 0;		m[0][2] = 0;	m[0][3] = 0;
			m[1][0] = 0;	 m[1][1] = 1.0f;	m[1][2] = 0;	m[1][3] = 0;
			m[2][0] = 0;	 m[2][1] = 0;		m[2][2] = 1.0f;	m[2][3] = 0;
			m[3][0] = 0;	 m[3][1] = 0;		m[3][2] = 0;	m[3][3] = 1.0f;
		}

		inline void			Zero()
		{
			m[0][0] = 0; m[0][1] = 0;	m[0][2] = 0; m[0][3] = 0;
			m[1][0] = 0; m[1][1] = 0;	m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0; m[2][1] = 0;	m[2][2] = 0; m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0;	m[3][2] = 0; m[3][3] = 0;
		}

		Matrix4		Inverse() const;

		/// Matrix Transformations
		/// Translation transformations
		inline void			SetTranslation(float fX, float fY, float fZ)
		{
			m[3][0] = fX; m[3][1] = fY; m[3][2] = fZ;
		}
		
		inline void			SetTranslation(const Vector3 &vec)
		{
			m[3][0] = vec.x; m[3][1] = vec.y; m[3][2] = vec.z;
		}

		inline Vector3		GetTranslation()
		{
			return Vector3(m[3][0], m[3][1], m[3][2]);
		}
		
		/// Scale transformations
		inline void			SetScale(float fX, float fY, float fZ)
		{
			m[0][0]	= fX;
			m[1][1]	= fY;
			m[2][2]	= fZ;
		}

		inline void			SetScale(const Vector3 &vec)
		{
			m[0][0]	= vec.x;
			m[1][1]	= vec.y;
			m[2][2]	= vec.z;
		}

		inline Vector3		GetScale()
		{
			return Vector3(m[0][0], m[1][1], m[2][2]);
		}

		/// Rotation transformations
		inline void			SetRotationX(float angle)
		{
			m[1][1]	= (float)cos(DEG_TO_RAD * angle);
			m[1][2]	= (float)sin(DEG_TO_RAD * angle);

			m[2][1]	= -m[1][2];
			m[2][2]	=  m[1][1];
		}
		
		inline void			SetRotationY(float angle)
		{
			m[0][0]	= (float)(cos(DEG_TO_RAD * angle));
			m[0][2]	= (float)(-sin(DEG_TO_RAD * angle));

			m[2][0]	= -m[0][2];
			m[2][2]	=  m[0][0];
		}

		inline void			SetRotationZ(float angle)
		{
			m[0][0]	= (float)cos(DEG_TO_RAD * angle);
			m[0][1]	= (float)sin(DEG_TO_RAD * angle);

			m[1][0]	= -m[0][1];
			m[1][1]	=  m[0][0];
		}

		inline void			SetRotationAroundAxis(float angle, const Vector3 &axis)
		{
			Vector3 lAxis(axis);
			lAxis.Normalize();

			float sinAngle = (float)sin(DEG_TO_RAD * angle);
			float cosAngle = (float)cos(DEG_TO_RAD * angle);
			float oneSubCos = 1.0f - cosAngle;

			m[0][0] = (lAxis.x * lAxis.x) * oneSubCos + cosAngle;
			m[1][0] = (lAxis.x * lAxis.y) * oneSubCos - (lAxis.z * sinAngle);
			m[2][0] = (lAxis.x * lAxis.z) * oneSubCos + (lAxis.y * sinAngle);

			m[0][1] = (lAxis.y * lAxis.x) * oneSubCos + (sinAngle * lAxis.z);
			m[1][1] = (lAxis.y * lAxis.y) * oneSubCos + cosAngle;
			m[2][1] = (lAxis.y * lAxis.z) * oneSubCos - (lAxis.x * sinAngle);
	
			m[0][2] = (lAxis.z * lAxis.x) * oneSubCos - (lAxis.y * sinAngle);
			m[1][2] = (lAxis.z * lAxis.y) * oneSubCos + (lAxis.x * sinAngle);
			m[2][2] = (lAxis.z * lAxis.z) * oneSubCos + cosAngle;
		}
		
		inline void			SetRotationYawPitchRoll(float yaw, float pitch, float roll)
		{
			double cosX = cos(pitch), cosY = cos(yaw), cosZ = cos(roll);
			double sinX = sin(pitch), sinY = sin(yaw), sinZ = sin(roll);

			m[0][0] = (float)(cosY * cosZ);
			m[0][1] = (float)(cosY * sinZ);
			m[0][2] = (float)(-sinY);

			m[1][0] = (float)((sinX * sinY) * cosZ - cosX * sinZ);
			m[1][1] = (float)((sinX * sinY) * sinZ + cosX * cosZ);
			m[1][2] = (float)(sinX * cosY);

			m[2][0] = (float)((cosX * sinY) * cosZ + sinX * sinZ);
			m[2][1] = (float)((cosX * sinY) * sinZ - sinX * cosZ);
			m[2][2] = (float)(cosX * cosY);
		}

		/// Projection transformations
		inline void			SetMatrixPerspectiveFovLH(float fov, float aspect, float zn, float zf)
		{
			float yScale	= 1 / tan(fov/2);
			float xScale	= yScale / aspect;

			m[0][0] = xScale;	m[0][1] = 0;		m[0][2] = 0;			  m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = yScale;	m[1][2] = 0;			  m[1][3] = 0;
			m[2][0] = 0;		m[2][1] = 0;		m[2][2] = zf/(zf-zn);	  m[2][3] = 1;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = -zn*zf/(zf-zn); m[3][3] = 0;
		}

		inline void			SetMatrixPerspectiveFovRH(float fov, float aspect, float zn, float zf)
		{
			float yScale	= 1 / tan(fov/2);
			float xScale	= yScale / aspect;

			m[0][0] = xScale;	m[0][1] = 0;		m[0][2] = 0;			  m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = yScale;	m[1][2] = 0;			  m[1][3] = 0;
			m[2][0] = 0;		m[2][1] = 0;		m[2][2] = zf/(zn-zf);	  m[2][3] = -1;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = zn*zf/(zn-zf);  m[3][3] = 0;
		}

		inline void			SetMatrixOrthographicLH(float width, float height, float zn, float zf)
		{
			m[0][0] = 2/width;	m[0][1] = 0;		m[0][2] = 0;			m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 2/height;	m[1][2] = 0;			m[1][3] = 0;
			m[2][0] = 0;		m[2][1] = 0;		m[2][2] = 1/(zf-zn);	m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = -zn/(zf-zn);  m[3][3] = 1;
		}

		inline void			SetMatrixOrthographicRH(float width, float height, float zn, float zf)
		{
			m[0][0] = 2/width;	m[0][1] = 0;		m[0][2] = 0;			m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 2/height;	m[1][2] = 0;			m[1][3] = 0;
			m[2][0] = 0;		m[2][1] = 0;		m[2][2] = 1/(zn-zf);	m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = zn/(zn-zf);   m[3][3] = 1;
		}

		/// Camera View matrix
		inline void			SetViewMatrixLH(const Vector3 &eye, const Vector3 &target, const Vector3 &up)
		{
			Vector3 zAxis = target - eye;
			zAxis.Normalize();
			
			Vector3 xAxis = up.Cross(zAxis);
			xAxis.Normalize();

			Vector3 yAxis = zAxis.Cross(xAxis);
			yAxis.Normalize();

			m[0][0] = xAxis.x;			m[0][1] = yAxis.x;			m[0][2] = zAxis.x;			m[0][3] = 0;
			m[1][0] = xAxis.y;			m[1][1] = yAxis.y;			m[1][2] = zAxis.y;			m[1][3] = 0;
			m[2][0] = xAxis.z;			m[2][1] = yAxis.z;			m[2][2] = zAxis.z;			m[2][3] = 0;
			m[3][0] = -xAxis.Dot(eye);	m[3][1] = -yAxis.Dot(eye);	m[3][2] = -zAxis.Dot(eye);	m[3][3] = 1;
		}

		inline void			SetViewMatrixRH(const Vector3 &eye, const Vector3 &target, const Vector3 &up)
		{
			Vector3 zAxis = eye - target;
			zAxis.Normalize();
			
			Vector3 xAxis = up.Cross(zAxis);
			xAxis.Normalize();

			Vector3 yAxis = zAxis.Cross(xAxis);
			yAxis.Normalize();

			m[0][0] = xAxis.x;			m[0][1] = yAxis.x;			m[0][2] = zAxis.x;			m[0][3] = 0;
			m[1][0] = xAxis.y;			m[1][1] = yAxis.y;			m[1][2] = zAxis.y;			m[1][3] = 0;
			m[2][0] = xAxis.z;			m[2][1] = yAxis.z;			m[2][2] = zAxis.z;			m[2][3] = 0;
			m[3][0] = xAxis.Dot(eye);	m[3][1] = yAxis.Dot(eye);	m[3][2] = zAxis.Dot(eye);	m[3][3] = 1;
		}

	//private:
		/// Private Variables
		float				m[4][4];
	};

}	// End of Math Namespace

#endif