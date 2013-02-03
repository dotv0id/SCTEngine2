/**
----------------------------------------------------------------------------
	@File:			SCTEngine.h
   
	System:         SCT Game Engine 
	Component Name:	SCT Engine
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


#ifndef SCT_ENGINE_H 
#define SCT_ENGINE_H

#include <SCT.h>
#include <SCTPlatform.h>
#include <SCTCore.h>

#include <Input\SCTInputManager.h>
#include <Render\SCTRenderInterface.h>
#include <Resource\SCTResourceManager.h>
#include <Resource\SCTMesh.h>
#include <Scene\SCTSceneManager.h>


// The libraries of the engine
#pragma comment(lib, "SCTMath.lib")
#pragma comment(lib, "SCTCore.lib")

// Assimp for model loading
#pragma comment(lib, "assimp.lib")


namespace SCT
{
	// Foreward declarations of classes
	namespace Resource
	{
		class SCTShaderD3D10Wireframe;
		class SCTShaderD3D10Lighting;
		class SCTShaderD3D10PostEffect;
		class SCTShaderD3D10SkyBox;
		class SCTSkyBox;
		//class SCTShaderD3D10Font;
	}

	namespace Render
	{
		class SCTRenderTexture;
		class SCTRenderWindow;
	}

	namespace Scene
	{
		class SCTCamera;
	}

	// Used to render the submeshes according to priority
	struct SCTRenderEntity
	{
		Resource::SCTSubmesh	*geometry;
		Math::Matrix4			lwtMatrix;
		float					distanceFromCamera;
		bool					bUseViewMatrix;
	};

	/// Class SCTEngine 
	/// The Engine ...
	class SCTEngine : public Singleton<SCTEngine>
	{
	public:
		SCTEngine();
		virtual ~SCTEngine() {};

		virtual SCTReturn					Initialize();
		virtual SCTReturn					HandleInput();
		virtual SCTReturn					Update(float elapsedTime);
		virtual SCTReturn					Render();
		virtual SCTReturn					Shutdown();

		SCTReturn							Configure(SCT::SCTConfigRender &config, HINSTANCE hInstance);
		SCTReturn							Run();
		void								EnableFrustumCulling(bool mode);
		
		void								ShowCursor(bool mode);
		
		// Get, Set functions
		void								SetRenderCamera(Scene::SCTCamera *camera);
		
		SCTReturn							SetFog(const Math::ColorRGBA &color, float start, float end);
		void								SetSkybox(Resource::SCTSkyBox *skybox, bool enabled = true);

		inline float						GetElapsedTime()
		{
			return mElapsedTime;
		}

		inline SCTResolution&				GetResolution()
		{
			return mConfig.Resolution;
		}

	protected:
		bool								mbDrawGeometry,
											mbDrawWireframe,
											mbDrawBounds,
											mbDrawNormals,
											mbDrawLocalAxis,
											mbDrawLights,
											mbShowStats,
											mbPEDebug;
			
		Math::ColorRGBA						mWireframeColor,
											mBoundsColor,
											mNormalsColor;

		SCT::SCTConfigRender				mConfig;

		bool								mbShowMouse;

	private:
		// --- Private Variables ---
		Scene::SCTSceneManager				*mpSceneManager;

		// Render Interface
		Render::SCTRenderInterface			*mpIRender;

		// Engine Configuration
		HINSTANCE							mhInstance;

		// Engine State
		bool								mbExit;

		// Active Render Camera
		Scene::SCTCamera					*mpCamera;

		// Default Shaders
		Resource::SCTShaderD3D10Wireframe	*mpShaderWireframe;
		Resource::SCTShaderD3D10Lighting	*mpShaderLighting;
		Resource::SCTShaderD3D10PostEffect	*mpShaderPostEffect;
		Resource::SCTShaderD3D10SkyBox		*mpShaderSkyBox;

		Render::SCTRenderTexture			*mpPERenderTarget;		// Render target used for Depth of Field
		Render::SCTRenderTexture			*mpDepthRenderTarget;	// Render target used for Depth of Field
		Render::SCTRenderTexture			*mpBlurRenderTarget;	// Render target used for Depth of Field

		Render::SCTRenderWindow				*mpPERenderWindow,		// Render window to render the post effect to
											*mpCrossSprite;
		
		Resource::SCTTexture				*mpCrossHud;

		bool								mbPostEffect;
		
		float								mElapsedTime;			// For use by the SCTApp

		// 2D rendering
		Math::Matrix4						mOrthographicMtx,
											mInvProjectionMtx,
											m2DViewMatrix;

		// Skybox
		Resource::SCTTexture				*mpTextureSkybox;
		Resource::SCTSkyBox					*mpSkybox;
		bool								mbSkybox;

		SUInt32								mTexturingMode;
		SUInt32								mSceneNodesRendered;

		// debug
		float	mExecTime,
				mPrevTime;
		// --- Private Functions ---
		void								UpdateEngineStats();
		SCTReturn							RenderScene(bool renderDepth = false);
		SCTReturn							RenderSkybox();
		SCTReturn							RenderSceneDebug(bool wireframe, bool normals, bool bounds);
	};

	// Singlenton
	template<> SCTEngine* Singleton<SCTEngine>::ms_Singleton = 0;

}	// End of SCT Namespace

#endif