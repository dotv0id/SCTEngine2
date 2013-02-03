#include <Render\Direct3D10\SCTRenderWindowD3D10.h>
#include <Render\SCTVertexDefs.h>
//#include <Resource\Direct3D10\SCTTextureD3D10.h>


namespace SCT
{
namespace Render
{
SCTRenderWindowD3D10::SCTRenderWindowD3D10(ID3D10Device *device)
:	mpDevice (device),
	mpDXVertexBuffer (NULL),
	mpDXIndexBuffer (NULL),
	mVertexCount (4),
	mIndexCount (6),
	mScreenWidth (0),
	mScreenHeight (0),
	mWidth (0),
	mHeight (0),
	mPositionX (-1),
	mPositionY (-1)
{
}

SCTRenderWindowD3D10::~SCTRenderWindowD3D10()
{
	Shutdown();
}

SCTReturn SCTRenderWindowD3D10::Initialize(SInt32 screenWidth, SInt32 screenHeight, SInt32 windowWidth, SInt32 windowHeight)
{
	mScreenWidth	= screenWidth;
	mScreenHeight	= screenHeight;
	mWidth			= windowWidth;
	mHeight			= windowHeight;

	SCTVertexPosTexture			*vertices;
	SUInt32						*indices;
	D3D10_BUFFER_DESC			vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA		vertexData, indexData;
	
	HRESULT hr;

	// Allocate the vertex array
	vertices = new SCTVertexPosTexture[mVertexCount];

	// Allocate the index array
	indices = new SUInt32[mIndexCount];

	// Vertex buffer
	vertices[0].position	= Math::Vector3(0, 0, 0);
	vertices[0].texture		= Math::Vector2(0, 0);
	vertices[1].position	= Math::Vector3(0, 0, 0);
	vertices[1].texture		= Math::Vector2(1.0f, 0);
	vertices[2].position	= Math::Vector3(0, 0, 0);
	vertices[2].texture		= Math::Vector2(1.0f, 1.0f);
	vertices[3].position	= Math::Vector3(0, 0, 0);
	vertices[3].texture		= Math::Vector2(0, 1.0f);

	// Left triangle
	indices[0] = 0;		// Top left
	indices[1] = 1;		// Top right
	indices[2] = 3;		// Bottom left

	// Right triangle
	indices[3] = 1;		// Top right
	indices[4] = 2;		// Bottom right
	indices[5] = 3;		// Bottom left

	// Set up the description of the dynamic vertex buffer
    vertexBufferDesc.Usage			= D3D10_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth		= sizeof(SCTVertexPosTexture) * mVertexCount;
    vertexBufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the vertex data
    vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer
    hr = mpDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &mpDXVertexBuffer);
	if(hr != S_OK)
		return FAIL;


	// Set up the description of the index buffer
    indexBufferDesc.Usage			= D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth		= sizeof(SUInt32) * mIndexCount;
    indexBufferDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags	= 0;
    indexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the index data
    indexData.pSysMem = indices;

	// Create the index buffer.
	hr = mpDevice->CreateBuffer(&indexBufferDesc, &indexData, &mpDXIndexBuffer);
	if(hr != S_OK)
		return FAIL;


	// Release the arrays now that the vertex and index buffers have been created and loaded
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return OK;
}

SCTReturn SCTRenderWindowD3D10::Render(SInt32 positionX, SInt32 positionY, SInt32 windowWidth, SInt32 windowHeight)
{
	// Update the vertex buffer if neccessary
	UpdateBuffers(positionX, positionY, windowWidth, windowHeight);

	// Send the geometry to the devices
	SUInt32 stride;
	SUInt32 offset;

	// Set vertex buffer stride and offset
    stride = sizeof(SCTVertexPosTexture); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered
	mpDevice->IASetVertexBuffers(0, 1, &mpDXVertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered
    mpDevice->IASetIndexBuffer(mpDXIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
    mpDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return OK;
}

SCTReturn SCTRenderWindowD3D10::Shutdown()
{
	// Release the vertex buffer
	ReleaseCOM(mpDXVertexBuffer);

	// Release the index buffer
	ReleaseCOM(mpDXIndexBuffer);

	return OK;
}

SCTReturn SCTRenderWindowD3D10::UpdateBuffers(SInt32 positionX, SInt32 positionY, SInt32 windowWidth, SInt32 windowHeight)
{
	// Check if the window sprite has changed position
	if((mPositionX == positionX) && (mPositionY == positionY) && (windowWidth == windowWidth) && (windowHeight == windowHeight))
		return OK;

	// Update the position and size of the window sprite
	mPositionX	= positionX;
	mPositionY	= positionY;
	mWidth		= windowWidth;
	mHeight		= windowHeight;
	

	// The window has changed position, update the buffers
	HRESULT hr;

	SCTVertexPosTexture			*vertices;

	// Allocate the vertex array
	vertices = new SCTVertexPosTexture[mVertexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(SCTVertexPosTexture) * mVertexCount));

	// Calculate the positions
	float left		= float(-1.0f * mScreenWidth/2 + mPositionX);
	float right		= float(left + mWidth);
	float top		= float(mScreenHeight/2 - mPositionY);
	float bottom	= float(top - mHeight);

	// Update the vertices
	// 0: Top Left
	vertices[0].position	= Math::Vector3(left, top, 0);
	vertices[0].texture		= Math::Vector2(0, 0);

	// 1: Top Right
	vertices[1].position	= Math::Vector3(right, top, 0);
	vertices[1].texture		= Math::Vector2(1.0f, 0);

	// 2: Bottom Right
	vertices[2].position	= Math::Vector3(right, bottom, 0);
	vertices[2].texture		= Math::Vector2(1.0f, 1.0f);

	// 3: Bottom Left
	vertices[3].position	= Math::Vector3(left, bottom, 0);
	vertices[3].texture		= Math::Vector2(0, 1.0f);

	// Get the vertex buffer data pointer
	SCTVertexPosTexture *verticesPtr = NULL;
	hr = mpDXVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);

	// Pass the updated vertices to the buffer
	for(SUInt32 i = 0; i < mVertexCount; i++)
		*verticesPtr++	= vertices[i];

	mpDXVertexBuffer->Unmap();

	delete [] vertices;
	vertices = NULL;

	return OK;
}

}	// End of Render Namespace
}	// End of SCT Namespace