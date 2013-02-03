#include <PCG\SCTPCGFunctions.h>
#include <SCTMath.h>


namespace SCT
{
namespace PCG
{

	void BuildGeoSphere(SUInt32 numSubdivisions, float radius, std::vector<Math::Vector3> &outVertices, std::vector<SUInt32> &outIndices)
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = Math::Min(numSubdivisions, SUInt32(5));

	// Approximate a sphere by tesselating an icosahedron.

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	Math::Vector3 pos[12] = 
	{
		Math::Vector3(-X, 0.0f, Z),  Math::Vector3(X, 0.0f, Z),  
		Math::Vector3(-X, 0.0f, -Z), Math::Vector3(X, 0.0f, -Z),    
		Math::Vector3(0.0f, Z, X),   Math::Vector3(0.0f, Z, -X), 
		Math::Vector3(0.0f, -Z, X),  Math::Vector3(0.0f, -Z, -X),    
		Math::Vector3(Z, X, 0.0f),   Math::Vector3(-Z, X, 0.0f), 
		Math::Vector3(Z, -X, 0.0f),  Math::Vector3(-Z, -X, 0.0f)
	};

	SUInt32 k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	outVertices.resize(12);
	outIndices.resize(60);

	for(int i = 0; i < 12; ++i)
		outVertices[i] = pos[i];

	for(int i = 0; i < 60; ++i)
		outIndices[i] = k[i];

	for(SUInt32 i = 0; i < numSubdivisions; ++i)
		Subdivide(outVertices, outIndices);

	// Project vertices onto sphere and scale.
	for(size_t i = 0; i < outVertices.size(); ++i)
	{
		outVertices[i].Normalize();
		outVertices[i] *= radius;
	}
}

void Subdivide(std::vector<Math::Vector3> &outVertices, std::vector<SUInt32> &outIndices)
{
	std::vector<Math::Vector3> vin	= outVertices;
	std::vector<SUInt32>  iin		= outIndices;

	outVertices.resize(0);
	outIndices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	SUInt32 numTris = (SUInt32)iin.size()/3;
	for(SUInt32 i = 0; i < numTris; ++i)
	{
		Math::Vector3 v0 = vin[ iin[i*3+0] ];
		Math::Vector3 v1 = vin[ iin[i*3+1] ];
		Math::Vector3 v2 = vin[ iin[i*3+2] ];

		Math::Vector3 m0 = (v0 + v1) * 0.5f;
		Math::Vector3 m1 = (v1 + v2) * 0.5f;
		Math::Vector3 m2 = (v0 + v2) * 0.5f;

		outVertices.push_back(v0); // 0
		outVertices.push_back(v1); // 1
		outVertices.push_back(v2); // 2
		outVertices.push_back(m0); // 3
		outVertices.push_back(m1); // 4
		outVertices.push_back(m2); // 5
 
		outIndices.push_back(i*6+0);
		outIndices.push_back(i*6+3);
		outIndices.push_back(i*6+5);
		
		outIndices.push_back(i*6+3);
		outIndices.push_back(i*6+4);
		outIndices.push_back(i*6+5);
		
		outIndices.push_back(i*6+5);
		outIndices.push_back(i*6+4);
		outIndices.push_back(i*6+2);
		
		outIndices.push_back(i*6+3);
		outIndices.push_back(i*6+1);
		outIndices.push_back(i*6+4);
	}
}

}	// End of PCG Namespace
}	// End of SCT Namespace