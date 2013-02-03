/**
----------------------------------------------------------------------------
	@File:			SCTRenderInterface.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Interface
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

#ifndef SCT_RENDER_INTERFACE_H
#define SCT_RENDER_INTERFACE_H


#include <Render\SCTRenderDefines.h>
#include <SCTSingleton.h>


namespace SCT
{
	// Foreward declarations of classes
	class SCTEngine;

	namespace Resource
	{
		class SCTMesh;
		class SCTSubmesh;
		class SCTShaderD3D10Lighting;
		class SCTShaderD3D10Wireframe;
		class SCTShaderD3D10;
		class SCTShader;
		class SCTSkyBox;
		class SCTTexture;
	}

	namespace Render
	{
		// Foreward declarations of classes
		class SCTRenderTexture;
		class SCTRenderWindow;

		// Texturing Mode -- Debug
		enum SCTTexturingMode
		{
			eStandard = 0,
			eDebugUV,
			eNoTexture,
			eMaxTexturingModes
		};

		/// Class SCTRenderInterface
		/// ...
		class SCTRenderInterface
		{
		public:
			virtual					~SCTRenderInterface() {};

			virtual SCTReturn		Initialize(const SCTConfigRender &config) = 0;
			virtual SCTReturn		RenderSubmesh(Resource::SCTSubmesh *submesh, Resource::SCTShader *shader, bool renderDepth = false) = 0;
			virtual SCTReturn		RenderSprite(Render::SCTRenderWindow *window, Resource::SCTShaderD3D10 *shader, const char* technique) = 0;
			virtual SCTReturn		RenderSkybox(Resource::SCTSkyBox *skybox, Resource::SCTShader *shader) = 0;
			virtual SCTReturn		RenderGUI() = 0;
			virtual SCTReturn		EndScene() = 0;
			virtual SCTReturn		Shutdown() = 0;
			
			virtual void			OnResize() = 0;

			// Render Targets
			virtual SCTReturn		RenderToTarget(SCTRenderTexture *renderTarget) = 0;
			virtual SCTReturn		RenderToBackBuffer() = 0;
			virtual SCTReturn		ClearRenderTarget(const Math::ColorRGBA &color, SCTRenderTexture *renderTarget = NULL) = 0;
			virtual SCTReturn		SetRenderTarget(SCTRenderTexture *renderTarget) = 0;

			// Depth buffer on/off
			virtual void			TurnZBufferOn() = 0;
			virtual void			TurnZBufferOff() = 0;

			// Debug Rendering Functions
			virtual SCTReturn		RenderMeshWireframe(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader) = 0;
			virtual SCTReturn		RenderMeshNormals(Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader) = 0;
			virtual SCTReturn		RenderMeshBoundbox( Resource::SCTMesh *mesh, Resource:: SCTShaderD3D10Wireframe *shader,
														const Math::Frustum &frustum) = 0;

			virtual void			SetDebugTexture(Resource::SCTTexture *texture) = 0;
			virtual void			SetTexturingMode(SCTTexturingMode mode) = 0;

			// Set Functions
			virtual void			SetRenderStats(const std::wstring &renderString) = 0;

		protected:

		public:
			friend class	SCTEngine;
		};
	}	// End of Render Namespace
}	// End of SCT Namespace

#endif