#include <Resource\Direct3D10\SCTSkyBoxD3D10.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Render\Direct3D10\SCTRenderInterfaceD3D10.h>
#include <Resource\Direct3D10\SCTTextureD3D10.h>
#include <PCG\SCTPCGFunctions.h>


namespace SCT
{
namespace Resource
{

SCTSkyBoxD3D10::SCTSkyBoxD3D10(ID3D10Device *device)
:	mpDevice (device)
{
	mpGeometry			= NULL;
	mpCubeMapTexture	= NULL;
}

SCTSkyBoxD3D10::~SCTSkyBoxD3D10()
{
	Shutdown();
}

SCTReturn SCTSkyBoxD3D10::Initialize(Resource::SCTTexture *cubeMapTexture, float radius)
{
	// Set the Skybox texture
	mpCubeMapTexture	= cubeMapTexture;

	// Create the geometry of the Skybox
	// Not good practice...
	mpGeometry	= new Resource::SCTSubmeshD3D10(mpDevice);
	
	std::vector<Math::Vector3>	vertices;
	std::vector<SUInt32>		indices;

	PCG::BuildGeoSphere(2, radius, vertices, indices);

	// Vertex buffer
	SUInt32 vertexCount	= vertices.size();
	Render::SCTVertexPos *vertexBuffer	= new Render::SCTVertexPos[vertexCount];

	for(SUInt32 i = 0; i < vertexCount; i++)
	{
		// Scale on y-axis to turn into an ellipsoid to make a flatter Sky surface
		vertexBuffer[i].position = vertices[i] * 0.5f;
	}

	// Index buffer
	SUInt32 indexCount	= indices.size();
	SUInt32	*indexBuffer	= new SUInt32[indexCount];

	for(SUInt32 i = 0; i < indexCount; i++)
	{
		indexBuffer[i]	= indices[i];
	}

	mpGeometry->SetVertexBuffer(vertexBuffer, vertexCount, Render::ePosition);
	mpGeometry->SetIndexBuffer(indexBuffer, indexCount);

	vertices.clear();
	indices.clear();

	return OK;
}

SCTReturn SCTSkyBoxD3D10::Render()
{
	mpGeometry->Render();

	return OK;
}

SUInt32 SCTSkyBoxD3D10::GetIndexCount()
{
	return mpGeometry->GetIndexCount();
}

SCTReturn SCTSkyBoxD3D10::Shutdown()
{
	// Delete the skybox geometry
	delete mpGeometry;
	mpGeometry	= NULL;

	return OK;
}

}	// End of Scene Namespace
}	// End of SCT Namespace