#include <SCTMathDebug.h>

namespace Math
{
	/// Debug Functions
	void DumpVector3(const Vector3 &vec)
	{
		//#if defined(DEBUG) || defined(_DEBUG)
		std::cout << "x = " << vec.x << ", y = " << vec.y << ", z = " << vec.z << "\n";
		//#define
	}

	void DumpMatrix4(const Matrix4 &m)
	{
		std::cout << "----------------------------\n";
		std::cout << m(1,1) << "\t" << m(1,2) << "\t" << m(1,3) << "\t" << m(1,4) << "\n";
		std::cout << m(2,1) << "\t" << m(2,2) << "\t" << m(2,3) << "\t" << m(2,4) << "\n";
		std::cout << m(3,1) << "\t" << m(3,2) << "\t" << m(3,3) << "\t" << m(3,4) << "\n";
		std::cout << m(4,1) << "\t" << m(4,2) << "\t" << m(4,3) << "\t" << m(4,4) << "\n";
		std::cout << "----------------------------\n";
	}

}	// End of Math Namespace