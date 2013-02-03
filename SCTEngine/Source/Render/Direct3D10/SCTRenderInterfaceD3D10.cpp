#include <SCTCore.h>
#include <SCTMath.h>

#include <Resource\SCTSkybox.h>
#include <Render\Direct3D10\SCTRenderInterfaceD3D10.h>
#include <Render\Direct3D10\SCTRenderTextureD3D10.h>
#include <Render\Direct3D10\SCTRenderWindowD3D10.h>
#include <Render\Direct3D10\SCTInputLayoutD3D10.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Resource\Direct3D10\SCTShaderD3D10.h>

// Default Shaders
#include <Resource\SCTTexture.h>
#include <Resource\SCTMaterial.h>
#include <Resource\Direct3D10\SCTShaderD3D10Wireframe.h>
#include <Resource\Direct3D10\SCTShaderD3D10Lighting.h>
#include <Resource\Direct3D10\SCTShaderD3D10SkyBox.h>
#include <Resource\Direct3D10\SCTShaderD3D10PostEffect.h>
#include <Resource\Direct3D10\SCTTextureD3D10.h>

using namespace Math;


namespace SCT
{
namespace Render
{

SCTRenderInterfaceD3D10::SCTRenderInterfaceD3D10()
:	mpDevice (NULL),
	mpSwapChain (NULL),
	mpRenderTargetView (NULL),
	mpFont (NULL),
	mpDepthStencilBuffer (NULL),
	mpDepthStencilView (NULL),
	mpDepthStencilState (NULL),
	mpDepthDisabledStencilState (NULL),
	mpDebugBlendState (NULL),
	mpTextureUVDebug (NULL),
	mpRasterizerState (NULL),
	mTexturingMode (eStandard)
{
	//	mpDepthStencilTexture (NULL)
}

SCTRenderInterfaceD3D10::~SCTRenderInterfaceD3D10()
{
	Shutdown();
}

SCTReturn SCTRenderInterfaceD3D10::Initialize(const SCTConfigRender &config)
{
	SCTReturn	ret;

	// Get the configuration struct
	mConfig			= config;

	// DirectX
	mD3DDriverType	= D3D10_DRIVER_TYPE_HARDWARE;

	// Initialize DirectX 10 device
	ret	= InitializeDevice();
	
	// Create Render target(s)
	ret	= CreateRenderTarget(&mpRenderTargetView);

	// Create Depth buffer
	ret	= CreateDepthBuffer(&mpDepthStencilBuffer, &mpDepthStencilView);

	//mpDepthStencilTexture	= new Resource::SCTTextureD3D10(mpDevice);
	mpDevice->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

	// Create Depth Stencil States 
	ret	= CreateDepthStencilState(&mpDepthStencilState, true);
	ret	= CreateDepthStencilState(&mpDepthDisabledStencilState, false);

	// Create the Viewport ---
	D3D10_VIEWPORT vp;

	vp.TopLeftX		= 0;
	vp.TopLeftY		= 0;
	vp.Width		= mConfig.Resolution.width;
	vp.Height		= mConfig.Resolution.height;
	vp.MinDepth		= 0.0f;
	vp.MaxDepth		= 1.0;

	mpDevice->RSSetViewports(1, &vp);

	// Initialize Font -- Temp, replace with TextManager
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 17;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    wcscpy(fontDesc.FaceName, L"Verdana");

	D3DX10CreateFontIndirect(mpDevice, &fontDesc, &mpFont);
	//\ Eof Font

	// Initialize the depth texture

	//\ Eof Dx10 initialization

	return ret;
}

SCTReturn SCTRenderInterfaceD3D10::RenderSubmesh(Resource::SCTSubmesh *submesh, Resource::SCTShader *shader, bool renderDepth)
{
	// Convert the input shader to lighting shader
	Resource::SCTShaderD3D10Lighting *shaderLighting = static_cast<Resource::SCTShaderD3D10Lighting*>(shader);

	// Reset the rasterizer state
	mpDevice->RSSetState(0);
	
	D3D10_TECHNIQUE_DESC techniqueDesc;

	// Check the texturing mode
	ID3D10ShaderResourceView *textures[MAX_TEXTURES];
	for(SUInt32 i = 0; i < MAX_TEXTURES; i++)
		textures[i] = NULL;


	switch(mTexturingMode)
	{
	case eStandard:
		{
			shaderLighting->SetRenderTextures(true);
		} break;
	case eDebugUV:
		{
			shaderLighting->SetRenderTextures(true);
			textures[eAmbient]	= mpTextureUVDebug->mpShaderResourceView;
			textures[eDiffuse]	= mpTextureUVDebug->mpShaderResourceView;
			textures[eSpecular] = mpTextureUVDebug->mpShaderResourceView;
		} break;
	case eNoTexture:
		{
			shaderLighting->SetRenderTextures(false);
		} break;
	};


	// Get the submeshes material
	Resource::SCTMaterial *material = submesh->GetMaterial();

	// Send it's Vertex and Index buffers to the IA
	submesh->Render();

	// Get it's textures
	if( (mTexturingMode == eStandard) && material)
	{
		Resource::SCTTextureD3D10* matTexture[MAX_TEXTURES];
		matTexture[eAmbient]	= static_cast<Resource::SCTTextureD3D10*>(material->mAmbientMap);
		matTexture[eDiffuse]	= static_cast<Resource::SCTTextureD3D10*>(material->mDiffuseMap);
		matTexture[eSpecular]	= static_cast<Resource::SCTTextureD3D10*>(material->mSpecularMap);
		matTexture[eAlpha]		= static_cast<Resource::SCTTextureD3D10*>(material->mAlphaMap);
		matTexture[eBump]		= static_cast<Resource::SCTTextureD3D10*>(material->mBumpMap);

		for(SUInt32 itTex = 0; itTex < MAX_TEXTURES; itTex++)
		{
			if(matTexture[itTex])
				textures[itTex]	= matTexture[itTex]->mpShaderResourceView;
		}
	}

	// Send the textures to the shader
	shaderLighting->SetTextures(textures);

	// Send the material properties to the shader
	shaderLighting->SetMaterialProperties(material->mMaterialProperties);

	mpDevice->IASetInputLayout(shaderLighting->mpInputLayout->mpDXInputLayout);	// Set the Input Layout
	
	// Choose the technique (Alpha, No alpha)
	if(material && (material->mbEnableAlpha))
		shaderLighting->mpTechnique = shaderLighting->mpShaderEffect->GetTechniqueByName("LightAlphaEnabled");
	else
		shaderLighting->mpTechnique = shaderLighting->mpShaderEffect->GetTechniqueByName("LightNoAlpha");

	//techniqueName
	if(renderDepth)
		shaderLighting->mpTechnique = shaderLighting->mpShaderEffect->GetTechniqueByName("RenderDepth");

	shaderLighting->mpTechnique->GetDesc(&techniqueDesc);
		
	// Go through each pass in the technique (should be just one currently) and render the triangles.
	for(SUInt32 i = 0; i<techniqueDesc.Passes; ++i)
	{
		shaderLighting->mpTechnique->GetPassByIndex(i)->Apply(0);
		mpDevice->DrawIndexed(submesh->GetIndexCount(), 0, 0);
	}

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::RenderGUI()
{
	mpDevice->RSSetState(0);

	RECT R = {5, 5, 0, 0};
	mpFont->DrawText(0, mRenderStats.c_str(), -1, &R, DT_NOCLIP, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	
	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::RenderSprite(Render::SCTRenderWindow *window, Resource::SCTShaderD3D10 *shader, const char* technique)
{
	mpDevice->RSSetState(0);

	D3D10_TECHNIQUE_DESC techniqueDesc;

	SCTRenderWindowD3D10 *wDD10	= (SCTRenderWindowD3D10 *)window;
	Resource::SCTShaderD3D10PostEffect *peShader	= static_cast<Resource::SCTShaderD3D10PostEffect*>(shader);	

	// Set the Input Layout
	mpDevice->IASetInputLayout(shader->mpInputLayout->mpDXInputLayout);

	peShader->mpTechnique = peShader->mpShaderEffect->GetTechniqueByName(technique);
	peShader->mpTechnique->GetDesc(&techniqueDesc);

	// Render all the submeshes of this mesh
	for(SUInt32 i = 0; i<techniqueDesc.Passes; ++i)
	{
		peShader->mpTechnique->GetPassByIndex(i)->Apply(0);
		mpDevice->DrawIndexed(wDD10->mIndexCount, 0, 0);
	}

	
	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::RenderSkybox(Resource::SCTSkyBox *skybox, Resource::SCTShader *shader)
{
	mpDevice->RSSetState(0);

	D3D10_TECHNIQUE_DESC techniqueDesc;

	// test
	Resource::SCTShaderD3D10SkyBox	*shaderSkybox	= static_cast<Resource::SCTShaderD3D10SkyBox*>(shader);

	// Send the skybox geometry to the IA
	skybox->Render();

	mpDevice->IASetInputLayout(shaderSkybox->mpInputLayout->mpDXInputLayout);
	shaderSkybox->mpTechnique->GetDesc(&techniqueDesc);

	for(SUInt32 i = 0; i<techniqueDesc.Passes; ++i)
	{
		shaderSkybox->mpTechnique->GetPassByIndex(i)->Apply(0);
		mpDevice->DrawIndexed(skybox->GetIndexCount(), 0, 0);
	}


	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(mConfig.Vsync)
	{
		// Lock to screen refresh rate.
		mpSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		mpSwapChain->Present(0, 0);
	}

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::Shutdown()
{
	// Delete the SCT Depth stencil texture
	//delete mpDepthStencilTexture;
	//mpDepthStencilTexture	= NULL;

	// Return to windowed mode
	if(mpSwapChain)
		mpSwapChain->SetFullscreenState(false, NULL);

	// Release the Depth stencil View and states
	ReleaseCOM(mpDepthStencilView);
	ReleaseCOM(mpDepthStencilState);
	ReleaseCOM(mpDepthDisabledStencilState);
	ReleaseCOM(mpDepthStencilBuffer);

	// Release the Render Target view
	ReleaseCOM(mpRenderTargetView);

	// Release the Swap Chain
	ReleaseCOM(mpSwapChain);

	// Finally, release the Device
	ReleaseCOM(mpDevice);	

	return OK;
}

void SCTRenderInterfaceD3D10::TurnZBufferOn()
{
	mpDevice->OMSetDepthStencilState(mpDepthStencilState, 1);

	// Bind the depth buffer
	mpDevice->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);
}

void SCTRenderInterfaceD3D10::TurnZBufferOff()
{
	mpDevice->OMSetDepthStencilState(mpDepthDisabledStencilState, 1);

	// Un-Bind the depth buffer
	mpDevice->OMSetRenderTargets(1, &mpRenderTargetView, NULL);
}

void SCTRenderInterfaceD3D10::SetDebugTexture(Resource::SCTTexture *texture)
{
	mpTextureUVDebug = static_cast<Resource::SCTTextureD3D10*>(texture);
}

/* --------------------------------------------------------------------------
	Render Targets
-------------------------------------------------------------------------- */
SCTReturn SCTRenderInterfaceD3D10::RenderToTarget(SCTRenderTexture *renderTarget)
{
	SCTRenderTextureD3D10 *renderTargetD3D10	= (SCTRenderTextureD3D10*)renderTarget;
	mpDevice->OMSetRenderTargets(1, &renderTargetD3D10->mpRenderTargetView, mpDepthStencilView);

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::RenderToBackBuffer()
{
	mpDevice->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::ClearRenderTarget(const Math::ColorRGBA &color, SCTRenderTexture *renderTarget)
{
	// Clears the render target with color 
	float bgColor[] = {color.r, color.g, color.b, color.a};

	// Clear the depth stencil
	mpDevice->ClearDepthStencilView(mpDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	// Clear the render target
	if(renderTarget)
	{
		SCTRenderTextureD3D10 *renderTargetD3D10	= (SCTRenderTextureD3D10*)renderTarget;
		mpDevice->ClearRenderTargetView(renderTargetD3D10->mpRenderTargetView, bgColor);

		return OK;
	}

	// If no render target set, clear the back buffer
	mpDevice->ClearRenderTargetView(mpRenderTargetView, bgColor);

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::SetRenderTarget(SCTRenderTexture *renderTarget)
{
	return OK;
}


/* --------------------------------------------------------------------------
	Private Functions
   -------------------------------------------------------------------------- */
SCTReturn SCTRenderInterfaceD3D10::InitializeDevice()
{
	HRESULT hr;

	// --- Direct3D device and Swap Chain description ---
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;

	sd.BufferDesc.Width  = mConfig.Resolution.width;;
	sd.BufferDesc.Height = mConfig.Resolution.height;
	
	// if VSYNC, lock refresh rate to system settings
	if(mConfig.Vsync)
	{
		sd.BufferDesc.RefreshRate.Numerator		= mConfig.Resolution.refreshRate;
		sd.BufferDesc.RefreshRate.Denominator	= 1;
	}
	else
	{
		sd.BufferDesc.RefreshRate.Numerator		= 0;
		sd.BufferDesc.RefreshRate.Denominator	= 1;
	}

	sd.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering				= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling						= DXGI_MODE_SCALING_UNSPECIFIED;

	// No multisampling.
	sd.SampleDesc.Count							= 1;
	sd.SampleDesc.Quality						= 0;

	sd.BufferUsage								= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount								= 1;
	sd.OutputWindow								= Core::SCTWindows::getSingletonPtr()->GetRenderWindow();
	
	if(mConfig.Fullscreen)
		sd.Windowed								= false;
	else
		sd.Windowed								= true;

	sd.SwapEffect								= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags									= 0;

	
	UINT createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif

	// --- Create the D3D10 device ---
	hr = D3D10CreateDeviceAndSwapChain(0, mD3DDriverType, 0, createDeviceFlags, D3D10_SDK_VERSION, &sd, &mpSwapChain, &mpDevice);	
	Core::SCTLogManager::getSingleton().displayError(hr);
	
	if(hr != S_OK)
		return CREATE_DX10_DEV_FAILED;

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::CreateDepthBuffer(ID3D10Texture2D **depthStencilBuffer, ID3D10DepthStencilView **depthStencilView)
{
	// --- Create the Depth Buffer ---
	if(mpDevice)
	{
		HRESULT hr;

		D3D10_TEXTURE2D_DESC				depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width				= mConfig.Resolution.width;
		depthBufferDesc.Height				= mConfig.Resolution.height;
		depthBufferDesc.MipLevels			= 1;
		depthBufferDesc.ArraySize			= 1;
		depthBufferDesc.Format				= DXGI_FORMAT_R32_TYPELESS;//DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count	= 1;
		depthBufferDesc.SampleDesc.Quality	= 0;
		depthBufferDesc.Usage				= D3D10_USAGE_DEFAULT;
		depthBufferDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
		depthBufferDesc.CPUAccessFlags		= 0;
		depthBufferDesc.MiscFlags			= 0;

		// Create the Depth buffer texture according to the above specs
		hr = mpDevice->CreateTexture2D(&depthBufferDesc, 0, depthStencilBuffer);
		Core::SCTLogManager::getSingleton().displayError(hr);


		D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format				= DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension		= D3D10_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		hr = mpDevice->CreateDepthStencilView(*depthStencilBuffer, &depthStencilViewDesc, depthStencilView);
		Core::SCTLogManager::getSingleton().displayError(hr);
	}

	return OK;
}

// Create render target
// type: Backbuffer, Texture
SCTReturn SCTRenderInterfaceD3D10::CreateRenderTarget(ID3D10RenderTargetView **renderTargetView)
{
	// --- Create Render Target
	if(mpDevice)
	{
		HRESULT hr;

		ID3D10Texture2D* backBuffer;
		
		hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer));
		Core::SCTLogManager::getSingleton().displayError(hr);

		hr = mpDevice->CreateRenderTargetView(backBuffer, 0, renderTargetView);
		Core::SCTLogManager::getSingleton().displayError(hr);

		ReleaseCOM(backBuffer);
	}
	else
	{
		Core::SCTLogManager::getSingleton().PrintDebug("[CreateRenderTarget] D3D10 Device has not been initialized");
	}

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::CreateDepthStencilState(ID3D10DepthStencilState **depthStencilState, bool DepthTesting)
{
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	if(DepthTesting)
		depthStencilDesc.DepthEnable					= true;
	else
		depthStencilDesc.DepthEnable					= false;

	depthStencilDesc.DepthWriteMask					= D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc						= D3D10_COMPARISON_LESS;
	depthStencilDesc.StencilEnable					= true;
	depthStencilDesc.StencilReadMask				= 0xFF;
	depthStencilDesc.StencilWriteMask				= 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp		= D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D10_STENCIL_OP_KEEP;
	depthStencilDesc .FrontFace.StencilFunc			= D3D10_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp			= D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D10_COMPARISON_ALWAYS;

	// Create the state using the device.
	HRESULT hr = mpDevice->CreateDepthStencilState(&depthStencilDesc, depthStencilState);
	if(hr != S_OK)
	{
		return FAIL;
	}

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::CreateRasterizerState(ID3D10RasterizerState **rasterizerState)
{
	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::CreateBlendState(ID3D10BlendState **blendState)
{
	return OK;
}

void SCTRenderInterfaceD3D10::OnResize()
{
}

/* --------------------------------------------------------------------------
	Debug Rendering Functions
-------------------------------------------------------------------------- */
SCTReturn SCTRenderInterfaceD3D10::RenderMeshWireframe(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;

	shader->mpTechnique->GetDesc(&techniqueDesc);

	// Set the Input Layout
	mpDevice->IASetInputLayout(shader->mpInputLayout->mpDXInputLayout);
	shader->mpTechnique->GetDesc(&techniqueDesc);

	// Send Submesh Geometry to Shader

	// Render all the submeshes of this mesh
	for(SUInt32 i = 0; i < mesh->GetNumberOfSubmeshes(); i++)
	{
		mesh->GetSubmesh(i)->Render();

		// Render Mesh again but in wireframe mode
		for(SUInt32 i = 0; i<techniqueDesc.Passes; ++i)
		{
			shader->mpTechnique->GetPassByIndex(i)->Apply(0);
			mpDevice->DrawIndexed(mesh->GetSubmesh(i)->GetIndexCount(), 0, 0);
		}
	}

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::RenderMeshNormals(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;

	// The Vertex buffer that will hold all the normals of this mesh
	std::vector<Render::SCTVertexPos> normalVertices;

	// For every submesh of this mesh
	for(SUInt32 i = 0; i < mesh->GetNumberOfSubmeshes(); i++)
	{
		Render::SCTVertexPosTangBiNormTex	*vertexBuffer = NULL;	// Vertex Buffer
		SUInt32								vertexCount;

		// Get this submeshe's vertex buffer
		mesh->GetSubmesh(i)->GetVertexBuffer(&vertexBuffer, vertexCount);

		SUInt32 index	= 0;
		for(SUInt32 j = 0; j < vertexCount; j++)
		{
			Render::SCTVertexPos	vertex1, vertex2;

			vertex1.position	= vertexBuffer[j].position;
			vertex2.position	= (vertexBuffer[j].position + vertexBuffer[j].normal);
			
			normalVertices.push_back(vertex1);
			normalVertices.push_back(vertex2);
		}
	}
	
	ID3D10Buffer					*DXVertexBuffer;

	D3D10_BUFFER_DESC				vertexBufferDesc;
	D3D10_SUBRESOURCE_DATA			vertexData;
	HRESULT							result;

	vertexBufferDesc.Usage			= D3D10_USAGE_DEFAULT;	
	vertexBufferDesc.ByteWidth		= sizeof(Render::SCTVertexPos) * normalVertices.size();
	vertexBufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem				= &normalVertices[0];

	// Now finally create the vertex buffer.
	result = mpDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &DXVertexBuffer);
	if(result != S_OK)
	{
		Core::SCTLogManager::getSingletonPtr()->PrintDebug("Failed to create the Directx Vertex Buffer");
	}

	// Send the buffer to the IA stage
	SUInt32 offset	= 0;
	SUInt32 stride	= sizeof(Render::SCTVertexPos);
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	mpDevice->IASetVertexBuffers(0, 1, &DXVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	mpDevice->IASetIndexBuffer(DXVertexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	mpDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);


	// Render the normals
	// Set the Input Layout
	mpDevice->IASetInputLayout(shader->mpInputLayout->mpDXInputLayout);
	shader->mpTechnique->GetDesc(&techniqueDesc);

	// Render Mesh again but in wireframe mode
	for(SUInt32 i = 0; i<techniqueDesc.Passes; ++i)
	{
		shader->mpTechnique->GetPassByIndex(i)->Apply(0);
		mpDevice->Draw(normalVertices.size(), 0);
	}

	normalVertices.clear();

	return OK;
}

SCTReturn SCTRenderInterfaceD3D10::RenderMeshBoundbox(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader, const Math::Frustum &frustum)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;

	// The Vertex buffer that will hold all the normals of this mesh
	std::vector<Render::SCTVertexPos>	verticesBB;
	SUInt32	indexCount	= 24 * mesh->GetNumberOfSubmeshes();
	SUInt32	*indicesBB	= new SUInt32[indexCount];

	// For every submesh of this mesh
	// Create the vertex and index buffers
	SUInt32 submeshVertices = 0;
	
	for(SUInt32 i = 0; i < mesh->GetNumberOfSubmeshes(); i++)
	{
		Resource::SCTSubmesh *submesh = mesh->GetSubmesh(i);

		Math::BoundingBox &aabb = submesh->GetAABB();

		Render::SCTVertexPos vertex;

		// Bottom
		vertex.position.x = aabb.mMin.x;
		vertex.position.y = aabb.mMin.y;
		vertex.position.z = aabb.mMin.z;
		verticesBB.push_back(vertex);

		vertex.position.x = aabb.mMin.x;
		vertex.position.y = aabb.mMin.y;
		vertex.position.z = aabb.mMax.z;
		verticesBB.push_back(vertex);

		vertex.position.x = aabb.mMax.x;
		vertex.position.y = aabb.mMin.y;
		vertex.position.z = aabb.mMax.z;
		verticesBB.push_back(vertex);

		vertex.position.x = aabb.mMax.x;
		vertex.position.y = aabb.mMin.y;
		vertex.position.z = aabb.mMin.z;
		verticesBB.push_back(vertex);

		
		// Top
		vertex.position.x = aabb.mMax.x;
		vertex.position.y = aabb.mMax.y;
		vertex.position.z = aabb.mMax.z;
		verticesBB.push_back(vertex);

		vertex.position.x = aabb.mMax.x;
		vertex.position.y = aabb.mMax.y;
		vertex.position.z = aabb.mMin.z;
		verticesBB.push_back(vertex);

		vertex.position.x = aabb.mMin.x;
		vertex.position.y = aabb.mMax.y;
		vertex.position.z = aabb.mMin.z;
		verticesBB.push_back(vertex);

		vertex.position.x = aabb.mMin.x;
		vertex.position.y = aabb.mMax.y;
		vertex.position.z = aabb.mMax.z;
		verticesBB.push_back(vertex);
		

		// Construct the index buffer -- line list
		// Bottom
		indicesBB[24*i + 0] = submeshVertices + 0; indicesBB[24*i + 1] = submeshVertices + 1;
		indicesBB[24*i + 2] = submeshVertices + 1; indicesBB[24*i + 3] = submeshVertices + 2;
		indicesBB[24*i + 4] = submeshVertices + 2; indicesBB[24*i + 5] = submeshVertices + 3;
		indicesBB[24*i + 6] = submeshVertices + 3; indicesBB[24*i + 7] = submeshVertices + 0;

		// Top
		indicesBB[24*i + 8]  = submeshVertices + 4; indicesBB[24*i + 9]  = submeshVertices + 5;
		indicesBB[24*i + 10] = submeshVertices + 5; indicesBB[24*i + 11] = submeshVertices + 6;
		indicesBB[24*i + 12] = submeshVertices + 6; indicesBB[24*i + 13] = submeshVertices + 7;
		indicesBB[24*i + 14] = submeshVertices + 7; indicesBB[24*i + 15] = submeshVertices + 4;

		// Sides
		indicesBB[24*i + 16] = submeshVertices + 0; indicesBB[24*i + 17] = submeshVertices + 6;
		indicesBB[24*i + 18] = submeshVertices + 1; indicesBB[24*i + 19] = submeshVertices + 7;
		indicesBB[24*i + 20] = submeshVertices + 2; indicesBB[24*i + 21] = submeshVertices + 4;
		indicesBB[24*i + 22] = submeshVertices + 3; indicesBB[24*i + 23] = submeshVertices + 5;

		// So we can get a correct index buffer
		submeshVertices += 8;
	}

	// Frustum



	// Create the DX Vertex Buffer for the bound box
	ID3D10Buffer					*DXVertexBuffer;

	D3D10_BUFFER_DESC				vertexBufferDesc;
	D3D10_SUBRESOURCE_DATA			vertexData;
	HRESULT							result;

	vertexBufferDesc.Usage			= D3D10_USAGE_DEFAULT;	
	vertexBufferDesc.ByteWidth		= sizeof(Render::SCTVertexPos) * verticesBB.size();
	vertexBufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem				= &verticesBB[0];

	// Now finally create the vertex buffer.
	result = mpDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &DXVertexBuffer);
	if(result != S_OK)
	{
		Core::SCTLogManager::getSingletonPtr()->PrintDebug("Failed to create the Directx Vertex Buffer");
	}

	// Create the DX Index buffer
	ID3D10Buffer					*DXIndexBuffer;
	D3D10_BUFFER_DESC				indexBufferDesc;
	D3D10_SUBRESOURCE_DATA			indexData;

	// Set up the description of the index buffer.
	indexBufferDesc.Usage			= D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth		= sizeof(SUInt32) * indexCount;
	indexBufferDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags	= 0;
	indexBufferDesc.MiscFlags		= 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem				= indicesBB;

	// Create the index buffer.
	result = mpDevice->CreateBuffer(&indexBufferDesc, &indexData, &DXIndexBuffer);
	if(result != S_OK)
	{
		Core::SCTLogManager::getSingletonPtr()->PrintDebug("Failed to create the Directx Index Buffer");
	}

	// --- Render the Bound boxes of this mesh ---
	// Send the buffer to the IA stage
	SUInt32 offset	= 0;
	SUInt32 stride	= sizeof(Render::SCTVertexPos);
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	mpDevice->IASetVertexBuffers(0, 1, &DXVertexBuffer, &stride, &offset);
	mpDevice->IASetIndexBuffer(DXIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the index buffer to active in the input assembler so it can be rendered.
	mpDevice->IASetIndexBuffer(DXIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	mpDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);


	// Render the normals
	// Set the Input Layout
	mpDevice->IASetInputLayout(shader->mpInputLayout->mpDXInputLayout);
	shader->mpTechnique->GetDesc(&techniqueDesc);

	// Render Mesh again but in wireframe mode
	for(SUInt32 i = 0; i<techniqueDesc.Passes; ++i)
	{
		shader->mpTechnique->GetPassByIndex(i)->Apply(0);
		mpDevice->DrawIndexed(indexCount, 0, 0);
		//mpDevice->Draw(8,0);
	}

	delete [] indicesBB;
	indicesBB = NULL;


 	return OK;
}

}	// End of Namespace Render
}	// End of Namespace SCT