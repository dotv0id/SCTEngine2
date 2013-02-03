#include <SCTCore.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>


using namespace Math;


namespace SCT
{
namespace Resource
{

SCTSubmeshD3D10::SCTSubmeshD3D10(ID3D10Device *device)
:	mpDevice (device),
	mDXVertexBuffer (NULL),
	mDXIndexBuffer (NULL)
{
	mpMaterial		= NULL;

	mbHasVertexBufferChanged	= false;
	mbHasIndexBufferChanged		= false;

	mVertexBuffer	= NULL;
	mIndexBuffer	= NULL;	
	
	mVertexCount	= 0;
	mIndexCount		= 0;
	mPolygonCount	= 0;
	
	// Highest priority by default
	mPriority = 1;
}
		
SCTSubmeshD3D10::~SCTSubmeshD3D10()
{
	Shutdown();
}

SCTReturn SCTSubmeshD3D10::Render()
{
	SUInt32 offset	= 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	mpDevice->IASetVertexBuffers(0, 1, &mDXVertexBuffer, &mStride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	mpDevice->IASetIndexBuffer(mDXIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	mpDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return OK;
}

SCTReturn SCTSubmeshD3D10::Shutdown()
{
	ReleaseCOM(mDXVertexBuffer);
	ReleaseCOM(mDXIndexBuffer);

	// Delete the generic Vertex Buffer
	if(mVertexBuffer)
	{
		switch(mVertexType)
		{
		case Render::ePosition:
			{
				delete[] (Render::SCTVertexPos*)mVertexBuffer;
			} break;
		case Render::ePosColor:
			{
				delete[] (Render::SCTVertexPosColor*)mVertexBuffer;
			} break;
		case Render::ePosNormTex:
			{
				delete[] (Render::SCTVertexPosNormTex*)mVertexBuffer;
			} break;
		case Render::ePosTangNormTex:
			{
				delete[] (Render::SCTVertexPosTangNormTex*)mVertexBuffer;
			} break;
		case Render::ePosTangBiNormTex:
			{
				delete[] (Render::SCTVertexPosTangBiNormTex*)mVertexBuffer;
			} break;
		}
		mVertexBuffer	= NULL;
	}

	// Delete the generic Index Buffer
	if(mIndexBuffer)
	{
		delete mIndexBuffer;
		mIndexBuffer	= NULL;
	}

	return OK;
}

void SCTSubmeshD3D10::UpdateVertexBuffer()
{
	D3D10_BUFFER_DESC		vertexBufferDesc;
	D3D10_SUBRESOURCE_DATA	vertexData;
	HRESULT					result;

	// Set up the description of the static vertex buffer.
	switch(mVertexType)
	{
	case Render::ePosition:
		{
			mStride		= sizeof(Render::SCTVertexPos);
		} break;
	case Render::ePosColor:
		{
			mStride		= sizeof(Render::SCTVertexPosColor);
		} break;
	case Render::ePosNormTex:
		{
 			mStride		= sizeof(Render::SCTVertexPosNormTex);
		} break;
	case Render::ePosTangNormTex:
		{
			mStride		= sizeof(Render::SCTVertexPosTangNormTex);
		} break;
	case Render::ePosTangBiNormTex:
		{
			mStride		= sizeof(Render::SCTVertexPosTangBiNormTex);
		} break;
	}

	vertexBufferDesc.Usage			= D3D10_USAGE_DEFAULT;	
	vertexBufferDesc.ByteWidth		= mStride * mVertexCount;
	vertexBufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem				= mVertexBuffer;

	// Now finally create the vertex buffer.
	result = mpDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &mDXVertexBuffer);
	if(result != S_OK)
	{
		Core::SCTLogManager::getSingletonPtr()->PrintDebug("Failed to create the Directx Vertex Buffer");
	}
}

void SCTSubmeshD3D10::UpdateIndexBuffer()
{
	D3D10_BUFFER_DESC		indexBufferDesc;
	D3D10_SUBRESOURCE_DATA	indexData;
	HRESULT					result;

	// Set up the description of the index buffer.
	indexBufferDesc.Usage			= D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth		= sizeof(SUInt32) * mIndexCount;
	indexBufferDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags	= 0;
	indexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem				= mIndexBuffer;

	// Create the index buffer.
	result = mpDevice->CreateBuffer(&indexBufferDesc, &indexData, &mDXIndexBuffer);
	if(result != S_OK)
	{
		Core::SCTLogManager::getSingletonPtr()->PrintDebug("Failed to create the Directx Index Buffer");
	}
}


/* ----------------------------------------------------------------
		SCTMeshD3D10 Implementation
------------------------------------------------------------------ */
SCTMeshD3D10::SCTMeshD3D10(ID3D10Device *device)
{
}

SCTMeshD3D10::~SCTMeshD3D10()
{
	Shutdown();
}

SCTReturn SCTMeshD3D10::Initialize()
{
	return OK;
}

SCTReturn SCTMeshD3D10::Update()
{
	return OK;
}

SCTReturn SCTMeshD3D10::RenderSubmesh(SUInt32 index)
{
	if(index <= mvSubmeshes.size())
	{
		mvSubmeshes[index]->Render();
	}

	return OK;
}

SCTReturn SCTMeshD3D10::Shutdown()
{
	// Delete all submeshes in the mesh
	std::vector<SCTSubmesh*>::iterator it;
	for(it = mvSubmeshes.begin(); it != mvSubmeshes.end(); it++)
	{
		if(*it)
			delete *it;
	}

	mvSubmeshes.clear();

	// Delete all the materials
	// ...

	

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT