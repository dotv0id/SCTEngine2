/**
----------------------------------------------------------------------------
	@File:			SCTRenderInterfaceD3D10.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Interface, Direct3D 10 implementation
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


#ifndef SCT_RENDER_INTERFACE_D3D10_H
#define SCT_RENDER_INTERFACE_D3D10_H


#include <Render\SCTRenderInterface.h>
#include <Render\Direct3D10\SCTRenderTextureD3D10.h>	


namespace SCT
{
	// Foreward Declarations
	class	SCTEngine;

	namespace Resource
	{
		class SCTMeshD3D10;
		class SCTShaderD3D10;
		class SCTTextureD3D10;
		class SCTResourceManager;
	}

	namespace Render
	{
		class SCTRenderWindow;
	}

	namespace Render
	{
		/// Class SCTRenderInterfaceD3D10
		//  ...
		class SCTRenderInterfaceD3D10 : public SCTRenderInterface, public Singleton<SCTRenderInterfaceD3D10>
		{
		public:
			SCTRenderInterfaceD3D10();
			~SCTRenderInterfaceD3D10();

			SCTReturn							Initialize(const SCTConfigRender &config);
			SCTReturn							RenderSubmesh(Resource::SCTSubmesh *submesh, Resource::SCTShader *shader, bool renderDepth = false);
			SCTReturn							RenderSprite(Render::SCTRenderWindow *window, Resource::SCTShaderD3D10 *shader, const char* technique);	// pass post effect shader
			SCTReturn							RenderSkybox(Resource::SCTSkyBox *skybox, Resource::SCTShader *shader);
			SCTReturn							RenderGUI();
			SCTReturn							EndScene();
			SCTReturn							Shutdown();

			void								OnResize();

			// Render Targets
			SCTReturn							RenderToTarget(SCTRenderTexture *renderTarget);
			SCTReturn							RenderToBackBuffer();
			SCTReturn							ClearRenderTarget(const Math::ColorRGBA &color, SCTRenderTexture *renderTarget);
			SCTReturn							SetRenderTarget(SCTRenderTexture *renderTarget);

			// Depth buffer on/off
			void								TurnZBufferOn();
			void								TurnZBufferOff();

			// Debug Rendering Functions
			SCTReturn							RenderMeshWireframe(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader);
			SCTReturn							RenderMeshNormals(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader);
			SCTReturn							RenderMeshBoundbox(	Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader, 
																	const Math::Frustum &frustum);
			
			void								SetDebugTexture(Resource::SCTTexture *texture);
			void								SetTexturingMode(SCTTexturingMode mode)
			{
				mTexturingMode	= mode;
			}

			// Get, Set Functions
			inline void							SetRenderStats(const std::wstring &renderString) { mRenderStats	= renderString; }
			inline ID3D10Device*				GetDevice() 
			{ 
				return mpDevice;
			}

		private:
			/// --- Private Variables ---
			SCTConfigRender						mConfig;

			// Direct3D variables
			ID3D10Device						*mpDevice;
			IDXGISwapChain						*mpSwapChain;
			ID3D10RenderTargetView				*mpRenderTargetView;			// Screen

			ID3DX10Font							*mpFont;						// TODO. Use text manager
			std::wstring						mRenderStats;
			
			// Depth Stencil View
			ID3D10Texture2D						*mpDepthStencilBuffer;
			ID3D10DepthStencilView				*mpDepthStencilView;
			//Resource::SCTTextureD3D10			*mpDepthStencilTexture;

			// Render States
			ID3D10DepthStencilState				*mpDepthStencilState;
			ID3D10DepthStencilState				*mpDepthDisabledStencilState;	// For font rendering			
			ID3D10BlendState					*mpDebugBlendState;
			ID3D10RasterizerState				*mpRasterizerState;

			D3D10_DRIVER_TYPE					mD3DDriverType;

			// Textures
			SCTTexturingMode					mTexturingMode;
			Resource::SCTTextureD3D10			*mpTextureUVDebug;


			/// --- Private Functions ---
			SCTReturn							InitializeDevice();				// Initialize the Direct3D 10.0 Device
			SCTReturn							CreateDepthBuffer(ID3D10Texture2D **depthStencilBuffer, ID3D10DepthStencilView **depthStencilView);
			SCTReturn							CreateRenderTarget(ID3D10RenderTargetView **renderTargetView);
			SCTReturn							CreateDepthStencilState(ID3D10DepthStencilState **depthStencilState, bool DepthTesting);
			SCTReturn							CreateRasterizerState(ID3D10RasterizerState **rasterizerState);
			SCTReturn							CreateBlendState(ID3D10BlendState **blendState);

		public:
			friend class	Resource::SCTResourceManager;
			friend class	SCTEngine;

		};
	}	// End of Render Namespace

	// Singlenton
	template<> Render::SCTRenderInterfaceD3D10* Singleton<Render::SCTRenderInterfaceD3D10>::ms_Singleton = 0;
}	// End of SCT Namespace

#endif