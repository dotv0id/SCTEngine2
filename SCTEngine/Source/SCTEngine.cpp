#include <SCTMath.h>
#include <SCTMathDebug.h>
#include <SCTCore.h>
#include <SCTEngine.h>

#include <Scene\SCTCamera.h>
#include <Resource\SCTSkyBox.h>

#include <Input\Win32\SCTInputManagerWin32.h>
#include <Input\Win32\SCTKeyboardWin32.h>
#include <Input\Win32\SCTMouseWin32.h>

#include <Render\Direct3D10\SCTRenderInterfaceD3D10.h>
#include <Render\Direct3D10\SCTRenderTextureD3D10.h>
#include <Render\Direct3D10\SCTRenderWindowD3D10.h>

// Shaders
#include <Resource\Direct3D10\SCTShaderD3D10Wireframe.h>
#include <Resource\Direct3D10\SCTShaderD3D10Lighting.h>
#include <Resource\Direct3D10\SCTShaderD3D10PostEffect.h>
#include <Resource\Direct3D10\SCTShaderD3D10SkyBox.h>

// __TEST
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Resource\SCTTexture.h>
#include <Resource\Direct3D10\SCTTextureD3D10.h>
//\ __EOF TEST

namespace SCT
{

SCTEngine::SCTEngine()
:	mbExit (false),
	mpSceneManager (NULL),
	mpIRender (NULL),
	mbDrawGeometry (true),
	mbDrawWireframe (false),
	mbDrawBounds (false),
	mbDrawNormals (false),
	mbDrawLocalAxis (false),
	mbDrawLights (false),
	mbShowStats (true),
	mWireframeColor (Math::ColorRGBA(0.5f, 0.5f, 0.75f, 1.0f)),
	mBoundsColor (Math::ColorRGBA(1.0f, 0, 0, 1.0f)),
	mNormalsColor (Math::ColorRGBA(0, 1.0f, 0, 1.0f)),
	mpShaderWireframe (NULL),
	mpShaderLighting (NULL),
	mpShaderSkyBox (NULL),
	mpShaderPostEffect (NULL),
	mpPERenderTarget (NULL),
	mpDepthRenderTarget (NULL),
	mpBlurRenderTarget (NULL),
	mpPERenderWindow (NULL),
	mpCamera (NULL),
	mElapsedTime (0),
	mbShowMouse (true),
	mTexturingMode (0),
	mpSkybox (NULL),
	mbSkybox (true),
	mbPEDebug (false),
	mSceneNodesRendered (0)
{
	mbPostEffect	= true;
}

SCTReturn SCTEngine::Initialize()
{
	SCTReturn ret;

	ret = Core::InitializeCore();

	// Tell the statistics manager how often to calculate the statistics
	Core::SCTStatistics::getSingleton().SetFpsCounterInterval(5);		// Seconds

	// 1. Create the render window
	ret = Core::SCTWindows::getSingleton().CreateRenderWindow(mhInstance, L"SCTEngine version 0.1 demo app", mConfig, false, false);

	// 2. Initialize the Resource Manager for D3D10 stuff (atm)
	Core::SCTLogManager::getSingleton().PrintDebug("Intitializing Resource Manager");
	Resource::SCTResourceManager *resourceMngr	= new Resource::SCTResourceManager();
	ret = resourceMngr->Initialize();

	// 3. Initialize the Input Manager
	Core::SCTLogManager::getSingleton().PrintDebug("Intitializing Input Manager [Win32]");
	Input::SCTInputManager *inputMngr	= new Input::SCTInputManagerWin32(mhInstance);
	ret = inputMngr->Initialize();

	ret = inputMngr->CreateKeyboardDevice();
	Core::SCTLogManager::getSingleton().PrintDebug("Keyboard Device has been initialized");

	ret = inputMngr->CreateMouseDevice();
	Core::SCTLogManager::getSingleton().PrintDebug("Mouse Device has been initialized");

	// 4. Initialize the Render Interface
	Core::SCTLogManager::getSingleton().PrintDebug("Intitializing Render Interface");
	mpIRender	= new Render::SCTRenderInterfaceD3D10;
	ret = mpIRender->Initialize(mConfig);
	
	// 5. Initialize the Scene Manager
	Core::SCTLogManager::getSingleton().PrintDebug("Intitializing Scene Manager");
	mpSceneManager	= new Scene::SCTSceneManager;
	ret = mpSceneManager->Initilize();

	// 2D rendering matrices
	mOrthographicMtx.SetMatrixOrthographicLH(float(mConfig.Resolution.width), float(mConfig.Resolution.height), 1.0f, 1000.0f);
	m2DViewMatrix.SetViewMatrixLH(Math::Vector3(0, 0, -10.0f), Math::Vector3(0, 0, 1.0f), Math::Vector3(0, 1.0f, 0));

	// Load the Default shaders
	Render::SCTRenderInterfaceD3D10 *rID3D10	= (Render::SCTRenderInterfaceD3D10*)mpIRender;

	mpShaderWireframe	= new Resource::SCTShaderD3D10Wireframe(rID3D10->GetDevice());	// Default Wireframe shader
	mpShaderLighting	= new Resource::SCTShaderD3D10Lighting(rID3D10->GetDevice());
	mpShaderPostEffect	= new Resource::SCTShaderD3D10PostEffect(rID3D10->GetDevice());
	mpShaderSkyBox		= new Resource::SCTShaderD3D10SkyBox(rID3D10->GetDevice());

	ret = mpShaderWireframe->Initialize(L"Shaders\\DefaultWireframeShader.fx", "WireframeTechnique", Render::ePosition);
	ret = mpShaderLighting->Initialize(L"Shaders\\DefaultLightShader.fx", "LightNoAlpha", Render::ePosTangBiNormTex);
	ret = mpShaderPostEffect->Initialize(L"Shaders\\ShaderPostEffect.fx", "RenderQuad", Render::ePosTexture);
	ret = mpShaderSkyBox->Initialize(L"Shaders\\DefaultSkyBoxShader.fx", "SkyTechnique", Render::ePosition);

	// Create Render Targets & Windows
	mpPERenderTarget	= new Render::SCTRenderTextureD3D10(rID3D10->GetDevice());
	ret = mpPERenderTarget->Initialize(mConfig.Resolution.width, mConfig.Resolution.height);

	mpDepthRenderTarget	= new Render::SCTRenderTextureD3D10(rID3D10->GetDevice());
	ret = mpDepthRenderTarget->Initialize(mConfig.Resolution.width, mConfig.Resolution.height);

	mpBlurRenderTarget	= new Render::SCTRenderTextureD3D10(rID3D10->GetDevice());
	ret = mpBlurRenderTarget->Initialize(mConfig.Resolution.width, mConfig.Resolution.height);

	mpPERenderWindow	= new Render::SCTRenderWindowD3D10(rID3D10->GetDevice());
	ret = mpPERenderWindow->Initialize(mConfig.Resolution.width, mConfig.Resolution.height, 100, 100);
	//\ End Of SCTEngine Initialization
	

	// Load the debug texture
	Resource::SCTTexture *debugTexture;
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\Textures\\Checker_Blue.png", &debugTexture);
	mpIRender->SetDebugTexture(debugTexture);


	// Load the cross Hud
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\Textures\\crosshair.dds", &mpCrossHud);

	// Use a render window to render the cross
	// TODO sprite 2d interface 
	mpCrossSprite	= new Render::SCTRenderWindowD3D10(rID3D10->GetDevice());
	ret = mpCrossSprite->Initialize(50, 50, 400, 300);

	return OK;
}

SCTReturn SCTEngine::HandleInput()
{
	Input::SCTInputManager *inputMngr	= Input::SCTInputManager::getSingletonPtr();

	inputMngr->Update();

	Input::SCTKeyboard *keyboard	= inputMngr->GetKeyboard();
	Input::SCTMouse *mouse			= inputMngr->GetMouse();

	// Exit the application when the ESC key is pressed
	if(keyboard->IsKeyPressed(Input::SCT_ESC))
		mbExit	= true;


	// Draw Debug (Bounds, Normals, Wireframe)
	if(keyboard->IsKeyPressed(Input::SCT_B))
	{
		std::cout<<"Rendering Bounds\n";
		mbDrawBounds	= !mbDrawBounds;
	}

	if(keyboard->IsKeyPressed(Input::SCT_N))
	{
		std::cout<<"Rendering Normals\n";
		mbDrawNormals = !mbDrawNormals;
	}

	if(keyboard->IsKeyPressed(Input::SCT_M))
	{
		std::cout<<"Rendering Wireframe\n";
		mbDrawWireframe	= !mbDrawWireframe;
	}

	if(keyboard->IsKeyPressed(Input::SCT_H))
	{
		std::cout<<"Render to texture\n";
		mbPostEffect	= !mbPostEffect;
	}

	if(keyboard->IsKeyPressed(Input::SCT_J))
	{
		std::cout<<"Camera reseted to default\n";
		mpCamera->Reset();
	}

	if(keyboard->IsKeyPressed(Input::SCT_K))
	{
		std::cout<<"Show stats\n";
		mbShowStats	= !mbShowStats;
	}

	if(keyboard->IsKeyPressed(Input::SCT_G))
	{
		mTexturingMode	= (mTexturingMode + 1) % Render::eMaxTexturingModes;
		mpIRender->SetTexturingMode(Render::SCTTexturingMode(mTexturingMode));

		switch(mTexturingMode)
		{
		case Render::eStandard:
			{
				SCTPrintD("Texturing mode: All textures");
			} break;
		case Render::eDebugUV:
			{
				SCTPrintD("Texturing mode: Debug UV texture");
			} break;
		case Render::eNoTexture:
			{
				SCTPrintD("Texturing mode: No textures");
			} break;
		}
	}

	if(keyboard->IsKeyPressed(Input::SCT_U))
	{
		std::cout<<"PostEffect whole screen\n";
		mbPEDebug	= !mbPEDebug;
	}
	
	keyboard->IsKeyReleased(Input::SCT_ESC);
	keyboard->IsKeyReleased(Input::SCT_B);
	keyboard->IsKeyReleased(Input::SCT_N);
	keyboard->IsKeyReleased(Input::SCT_M);
	keyboard->IsKeyReleased(Input::SCT_G);
	keyboard->IsKeyReleased(Input::SCT_H);
	keyboard->IsKeyReleased(Input::SCT_J);
	keyboard->IsKeyReleased(Input::SCT_K);
	keyboard->IsKeyReleased(Input::SCT_U);


	return OK;
}

SCTReturn SCTEngine::Update(float elapsedTime)
{
	// Update the scene
	mpSceneManager->Update(elapsedTime);

	return OK;
}

SCTReturn SCTEngine::Render()
{
	// 1. Render interface -> turnZ on - DEPTH
	mpIRender->TurnZBufferOn();

	// Configure the post effect shader
	Math::Matrix4 peWVP;
	peWVP	=  m2DViewMatrix * mOrthographicMtx;

	mpShaderPostEffect->SetWVPMatrix(peWVP);
	mpShaderPostEffect->SetProjectionMatrix(mInvProjectionMtx);
	mpShaderPostEffect->SetViewportSize(800, 600);

	// 2. Render scene to texture
	if(mbPostEffect)
	{
		// A: Render scene to target
		mpIRender->RenderToTarget(mpPERenderTarget);
		mpIRender->ClearRenderTarget(Math::ColorRGBA(0, 0.5f, 0.5f, 1.0f), mpPERenderTarget);
		
		{
			// Render the scene
			RenderScene();
		
			// Render the scene and the skybox to the target texture
			if(mbSkybox)
				RenderSkybox();
		}

		// Now render the depth buffer to texture
		mpIRender->RenderToTarget(mpDepthRenderTarget);
		mpIRender->ClearRenderTarget(Math::ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), mpDepthRenderTarget);
		RenderScene(true);

		// Pass the depth buffer texture to the post effect shader
		mpShaderPostEffect->SetDepthBufferTexture(((Render::SCTRenderTextureD3D10*)mpDepthRenderTarget)->mpRenderTargetTexture->mpShaderResourceView);

		// B: Render the Texture to the back buffer
		mpIRender->TurnZBufferOff();
		mpIRender->RenderToBackBuffer();
		mpIRender->ClearRenderTarget(Math::ColorRGBA(0, 0, 0, 1.0f));

		mpShaderPostEffect->SetPostEffectTexture(((Render::SCTRenderTextureD3D10*)mpPERenderTarget)->mpRenderTargetTexture->mpShaderResourceView);
		mpPERenderWindow->Render(0, 0, mConfig.Resolution.width, mConfig.Resolution.height);
		mpIRender->RenderSprite(mpPERenderWindow, mpShaderPostEffect, "RenderQuad");

		/*
		// C: Downscale the texture, to create a blured version of the same scene
		mpIRender->RenderToTarget(mpBlurRenderTarget);
		mpIRender->ClearRenderTarget(Math::ColorRGBA(0, 0, 0, 1.0f), mpBlurRenderTarget);

		// Set up the shader
		mpShaderPostEffect->SetDownscaleResolution(400, 300);
		mpShaderPostEffect->SetPostEffectTexture(((Render::SCTRenderTextureD3D10*)mpPERenderTarget)->mpRenderTargetTexture->mpShaderResourceView);

		mpPERenderWindow->Render(0, 0, mConfig.Resolution.width, mConfig.Resolution.height);
		mpIRender->RenderSprite(mpPERenderWindow, mpShaderPostEffect, "Downscale4");
		*/

		// Da: Apply gaussian blur - horizontal pass
		mpIRender->RenderToTarget(mpBlurRenderTarget);
		mpIRender->ClearRenderTarget(Math::ColorRGBA(0, 0, 0, 1.0f), mpBlurRenderTarget);
		
		mpShaderPostEffect->SetPostEffectTexture(((Render::SCTRenderTextureD3D10*)mpPERenderTarget)->mpRenderTargetTexture->mpShaderResourceView);
		
		mpPERenderWindow->Render(0, 0, mConfig.Resolution.width, mConfig.Resolution.height);
		mpIRender->RenderSprite(mpPERenderWindow, mpShaderPostEffect, "GaussianBlurHorizontal");

		// Db: Apply gaussian blur - vertical pass
		mpIRender->RenderToTarget(mpPERenderTarget);
		mpIRender->ClearRenderTarget(Math::ColorRGBA(0, 0, 0, 1.0f), mpPERenderTarget);

		mpShaderPostEffect->SetPostEffectTexture(((Render::SCTRenderTextureD3D10*)mpBlurRenderTarget)->mpRenderTargetTexture->mpShaderResourceView);

		mpPERenderWindow->Render(0, 0, mConfig.Resolution.width, mConfig.Resolution.height);
		mpIRender->RenderSprite(mpPERenderWindow, mpShaderPostEffect, "GaussianBlurVertical");

		// E: Apply Depth of Field effect
		mpIRender->RenderToBackBuffer();		// DO NOT CLEAR THE BACKBUFFER (will put blured texture with alpha, on the original)
		mpShaderPostEffect->SetDoFParameters(0, float(mConfig.Resolution.width), float(mConfig.Resolution.height), 10.0f);
		mpShaderPostEffect->SetPostEffectTexture(((Render::SCTRenderTextureD3D10*)mpPERenderTarget)->mpRenderTargetTexture->mpShaderResourceView);
		mpShaderPostEffect->SetDepthBufferTexture(((Render::SCTRenderTextureD3D10*)mpDepthRenderTarget)->mpRenderTargetTexture->mpShaderResourceView);

		if(mbPEDebug)
		{
			mpPERenderWindow->Render(590, 390, 200, 200);
			mpIRender->RenderSprite(mpPERenderWindow, mpShaderPostEffect, "RenderDoF");
		}
		else
		{
			mpPERenderWindow->Render(0, 0, mConfig.Resolution.width, mConfig.Resolution.height);
			mpIRender->RenderSprite(mpPERenderWindow, mpShaderPostEffect, "RenderDoF");
		}	
	}
	else
	{
		// Render the scene to the backbuffer - No post effects
		mpIRender->TurnZBufferOn();
		mpIRender->RenderToBackBuffer();
		mpIRender->ClearRenderTarget(Math::ColorRGBA(0, 0, 0, 1.0f));
	
		if(mbSkybox)
			RenderSkybox();

		RenderScene();

		// Render debug?
		if(mbDrawWireframe || mbDrawNormals || mbDrawBounds)
			RenderSceneDebug(mbDrawWireframe, mbDrawNormals, mbDrawBounds);
	}

	// Render HUD Elements
	mpShaderPostEffect->SetPostEffectTexture(static_cast<Resource::SCTTextureD3D10*>(mpCrossHud)->mpShaderResourceView);
	mpCrossSprite->Render(0, 0, 50, 50);
	mpIRender->RenderSprite(mpCrossSprite, mpShaderPostEffect, "RenderQuadAlpha");

	// Render the statistics
	if(mbShowStats)
		mpIRender->RenderGUI();

	return OK;
}

SCTReturn SCTEngine::Run()
{
	Core::SCTLogManager::getSingleton().PrintDebug("Entering engine's main loop...");

	mbExit	= false;

	MSG		msg;
	ZeroMemory(&msg, sizeof(MSG));

	float fps	= 0;
	float ms	= 0;
	
	while(!mbExit)
	{
		//mPrevTime = Core::SCTTime::getSingleton().GetElapsedTime();

		// If there are Window messages then process them.
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}

		// Quit Main loop
		if(msg.message == WM_QUIT)
		{
			mbExit	= true;
		}
		else
		{
			Core::SCTTime::getSingleton().Tick();
			mElapsedTime	= Core::SCTTime::getSingleton().GetElapsedTime();

			HandleInput();
			Update(mElapsedTime);

			if(!Core::SCTTime::getSingleton().IsPaused())
			{
				// Update the Statistics
				Core::SCTStatistics::getSingleton().Update();
				UpdateEngineStats();

				// Render
				Render();
			}
			else
				Sleep(50);

			// Post frame Rendering 
			mpIRender->EndScene();
		}

		// time
		//mExecTime = Core::SCTTime::getSingleton().GetElapsedTime() - mPrevTime;
		//std::cout<<"execution time: " << mExecTime << "\n";
	}


	return OK;
}

SCTReturn SCTEngine::Shutdown()
{
	// Shutdown and clean up the scene manager
	Scene::SCTSceneManager	*sceneMngr	= Scene::SCTSceneManager::getSingletonPtr();
	if(sceneMngr)
	{
		delete sceneMngr;
		sceneMngr	= NULL;
	}

	// Shutdown and clean up the resource manager
	Resource::SCTResourceManager *resourceMngr	= Resource::SCTResourceManager::getSingletonPtr();
	if(resourceMngr)
	{
		delete resourceMngr;
		resourceMngr	= NULL;
	}

	// Shutdown the Render interface
	delete mpPERenderTarget;
	mpPERenderTarget	= NULL;

	delete mpDepthRenderTarget;
	mpDepthRenderTarget	= NULL;

	delete mpBlurRenderTarget;
	mpBlurRenderTarget	= NULL;

	delete mpPERenderWindow;
	mpPERenderWindow	= NULL;

	delete (Render::SCTRenderInterfaceD3D10*)mpIRender;
	mpIRender	= NULL;

	// Delete the default shaders
	delete mpShaderWireframe;
	mpShaderWireframe	= NULL;

	delete mpShaderLighting;
	mpShaderLighting	= NULL;

	delete mpShaderPostEffect;
	mpShaderPostEffect	= NULL;

	delete mpShaderSkyBox;
	mpShaderSkyBox	= NULL;

	// Shutdown Input manager
	Input::SCTInputManager *inputMngr	= Input::SCTInputManagerWin32::getSingletonPtr();
	if(inputMngr)
	{
		delete inputMngr;
		inputMngr	= NULL;
	}

	// Shutdown the Core
	Core::ShutdownCore();


	return OK;
}

SCTReturn SCTEngine::Configure(SCT::SCTConfigRender &config, HINSTANCE hInstance)
{
	// Get the application hInstance
	mhInstance	= hInstance;

	mConfig	= config;

	return OK;
}

void SCTEngine::SetSkybox(Resource::SCTSkyBox *skybox, bool enabled)
{
	mpSkybox	= skybox;
	mbSkybox	= enabled;

	Resource::SCTTextureD3D10 *texture = static_cast<Resource::SCTTextureD3D10*>(skybox->GetTexture());
	
	mpShaderSkyBox->SetTexture(texture->mpShaderResourceView);
}

/*-------------------------------------------------------------------------------------
		Private Functions
  -------------------------------------------------------------------------------------- */
SCTReturn SCTEngine::RenderScene(bool renderDepth)
{
	// Get the camera's position
	Math::Vector3 camPosition = mpCamera->GetPosition();

	// !!! TODO: categorize submeshes according to their distance from the camera !!!

	// Render meshes according to render priority
	// 1 (HIGH - render first): no alpha blending
	// 0 (LOW): alpha blending
	std::vector<SCTRenderEntity> priorityList[3];

	// Parse the Scene tree
	// Get the data of each node and send it to the render interface
	Scene::SCTSceneNode *root	= mpSceneManager->GetRoot();

	std::vector<Scene::SCTSceneNode*> vNodeStack;
	vNodeStack.push_back(root);

	// Ugly hack...
	SUInt32 weaponIndex;

	mSceneNodesRendered = 0;

	// Traverse the nodes - Depth first [Dragonbreath]
	while(!vNodeStack.empty())
	{
		// Get the back
		Scene::SCTSceneNode *current	= vNodeStack.back();
		vNodeStack.pop_back();

		// Organize all the submeshes according to their render priority
		if(current->mbIsVisible)
		{
			mSceneNodesRendered++;

			Resource::SCTMesh *mesh;
			current->GetData(&mesh);

			if(mesh)
			{
				for(SUInt32 itSubmesh = 0; itSubmesh < mesh->GetNumberOfSubmeshes(); itSubmesh++)
				{
					Resource::SCTSubmesh *submesh = mesh->GetSubmesh(itSubmesh);

					if(submesh)
					{
						SUInt32 priority = submesh->GetRenderPriority();

						// If this is a node of alpha blending priority
						// Calculate distance from camera
						float distanceCam = 0;
						if(priority == 0)
						{
							Math::Vector3 nodePosition = current->GetLocalToWorldMatrix().GetTranslation();
							distanceCam = (camPosition - nodePosition).LengthSquared();
						}

						SCTRenderEntity renderEntity;
						renderEntity.geometry			= submesh;
						renderEntity.lwtMatrix			= current->GetLocalToWorldMatrix();
						renderEntity.distanceFromCamera	= distanceCam;
						renderEntity.bUseViewMatrix		= false;

						if(current->mName.find("PlayerWeapon") != -1)
						{
							renderEntity.bUseViewMatrix		= true;
							
							// lower priority
							priority = 2;		// "Special" hack, render last
						}

						// Add to the priority list
						priorityList[priority].push_back(renderEntity);
					}
				}
			}

			// Push all this node's children and put them in the stack
			std::list<Scene::SCTSceneNode*>::iterator it;			
			for(it = current->mChildren.begin(); it != current->mChildren.end(); it++)
			{
				vNodeStack.push_back(*it);
			}
		}
	}

	// and render the geometry according to the submesh
	// I can set the priority in the load mesh in resource manager, if that submeshes material uses
	// alpha map (texture)
	// Now render all the meshes
	for(SInt32 itPr = 1; itPr > -1; itPr--)
	{
		std::vector<SCTRenderEntity>::iterator itEntity;
		for(itEntity = priorityList[itPr].begin(); itEntity != priorityList[itPr].end(); itEntity++)
		{
			mpShaderLighting->SetViewMatrix(mpCamera->GetViewMatrix());
			mpShaderLighting->SetWorldMatrix((*itEntity).lwtMatrix);

			// Set up shader parameters
			mpShaderLighting->SetProjectionMatrix(mpCamera->GetProjectionMatrix());
			mpShaderLighting->SetCameraPosition(mpCamera->GetPosition());
			mpIRender->RenderSubmesh((*itEntity).geometry, mpShaderLighting, renderDepth);
		}
	}

	
	// Render weapon... last
	{
		// Use Identity view matrix
		Math::Matrix4 Identity, offset, rot;
		mpShaderLighting->SetViewMatrix(Identity);

		offset = priorityList[2][0].lwtMatrix;
		offset.SetScale(0.04f, 0.04f, 0.04f);
		rot.SetRotationYawPitchRoll(-float(Math::HALFPI), 0, 0);
		offset = offset * rot;
		offset.SetTranslation(Math::Vector3(2.0f, -2.0f, 6.0f));
		mpShaderLighting->SetWorldMatrix(offset);

		mpShaderLighting->SetProjectionMatrix(mpCamera->GetProjectionMatrix());
		mpShaderLighting->SetCameraPosition(mpCamera->GetPosition());
		mpIRender->RenderSubmesh(priorityList[2][0].geometry, mpShaderLighting, renderDepth);
	}
	

	return OK;
}

SCTReturn SCTEngine::RenderSkybox()
{
	// Render the skybox
	Math::Matrix4 skyboxWorldMatrix;
	skyboxWorldMatrix.SetTranslation(mpCamera->GetPosition());
	
	mpShaderSkyBox->SetWorldMatrix(skyboxWorldMatrix);
	mpShaderSkyBox->SetViewMatrix(mpCamera->GetViewMatrix());
	mpShaderSkyBox->SetProjectionMatrix(mpCamera->GetProjectionMatrix());
	mpIRender->RenderSkybox(mpSkybox, mpShaderSkyBox);

	return OK;
}

SCTReturn SCTEngine::RenderSceneDebug(bool wireframe, bool normals, bool bounds)
{
	// Parse the Scene tree
	// Get the data of each node and send it to the render interface
	Scene::SCTSceneNode *root	= mpSceneManager->GetRoot();

	std::vector<Scene::SCTSceneNode*> vNodeStack;
	vNodeStack.push_back(root);

	// Traverse the nodes - Depth first [Dragonbreath]
	while(!vNodeStack.empty())
	{
		// Get the back
		Scene::SCTSceneNode *current	= vNodeStack.back();
		vNodeStack.pop_back();

		// If node is visible; render it, put all his children in the stack
		if(current->mbIsVisible)
		{
			// Get this node's data and render it
			Resource::SCTMesh *mesh;
			current->GetData(&mesh);

			// Could be optimized by keeping a different tree for lights
			if(mesh)
			{
				Math::Matrix4 wvp;
				wvp = current->mLocalToWorldMatrix * mpCamera->GetViewMatrix() * mpCamera->GetProjectionMatrix();

				// Set up Debug shader parameters
				mpShaderWireframe->SetWVPMatrix(wvp);

				// Render Wireframe
				if(wireframe)
				{
					mpShaderWireframe->SetWireframeColor(mWireframeColor);
					mpIRender->RenderMeshWireframe(mesh, mpShaderWireframe);
				}

				// Render Normals
				if(normals)
				{
					mpShaderWireframe->SetWireframeColor(mNormalsColor);
					mpIRender->RenderMeshNormals(mesh, mpShaderWireframe);
				}

				// Render the bounds
				if(bounds)
				{
					mpShaderWireframe->SetWireframeColor(mBoundsColor);
					mpIRender->RenderMeshBoundbox(mesh, mpShaderWireframe, mpCamera->GetFrustum());
				}
			}

			// Push all this node's children and put them in the stack
			std::list<Scene::SCTSceneNode*>::iterator it;			
			for(it = current->mChildren.begin(); it != current->mChildren.end(); it++)
			{
				vNodeStack.push_back(*it);
			}
		}
	}

	return OK;
}

void SCTEngine::ShowCursor(bool mode)
{
	Core::SCTWindows::getSingleton().SetMouseCursor(mode);
}

void SCTEngine::UpdateEngineStats()
{
	Input::SCTMouse *mouse	= Input::SCTInputManager::getSingleton().GetMouse();

	std::wostringstream outs;
	outs.precision(6);

	float			fps, 
					mspf;
	SUInt32			cpu;
	static SInt32	mouseX, mouseY,
					mouseRelX, mouseRelY;
	Math::Vector3	cameraPos;

	cameraPos = mpCamera->GetPosition();

	Core::SCTStatistics::getSingleton().GetFrameStats(fps, mspf);
	Core::SCTStatistics::getSingleton().GetCpuPercentage(cpu);

	mouse->GetAbsolutePosition(mouseX, mouseY);

	if(mouse->GetRelativeX())
		mouseRelX	= mouse->GetRelativeX();
	if(mouse->GetRelativeY())
		mouseRelY	= mouse->GetRelativeY();

	outs <<L"'C': toggle between Debug camera mode and Player mode\n" <<
			"'B': render bounds, 'N': normals, 'W': wireframe,\n" <<
			"'H': toogle render to texture, 'K': toggle stats\n" <<
			"-----------------------------------------------------------\n"
			"Fps: " << fps << L"\n" <<
			"Ms per frame: " << mspf<< L"\n" <<
			"Cpu usage: " << cpu << L"%\n" <<
			"Mouse x: " << mouseX << " [Rel x: " << mouseRelX << "]," <<
			"Mouse y: " << mouseY << " [Rel y: " << mouseRelY << "]\n" <<
			//"Mouse relative z: " << mouse->GetRelativeZ() << "\n" <<
			//"Camera position: \nx=" << cameraPos.x << "\ny=" << cameraPos.y << "\nz=" << cameraPos.z << "\n" <<
			"Total Scene Nodes: " << Scene::SCTSceneManager::getSingleton().GetSceneNodesCount() << "\n" <<
			"Entities Rendered: " << mSceneNodesRendered;

	mpIRender->SetRenderStats(outs.str());
}

// Get, Set functions
SCTReturn SCTEngine::SetFog(const Math::ColorRGBA &color, float start, float end)
{
	if(mpShaderLighting)
		mpShaderLighting->SetFogParameters(color, start, end);
	else
		return NULL_POINTER;

	return OK;
}

void SCTEngine::SetRenderCamera(Scene::SCTCamera *camera)
{
	mpCamera	= camera;

	// No need to call for that every time we change camera
	Scene::SCTSceneManager::getSingleton().SetCamera(camera);

	// Get the inverse projection matrix
	// used for Depth of Field post processing
	mInvProjectionMtx	= mpCamera->GetProjectionMatrix().Inverse();

	// Debug Info
	SCTPrintD("Projection matrix:");
	Math::DumpMatrix4(mpCamera->GetProjectionMatrix());

	SCTPrintD("Inverse projection matrix:");
	Math::DumpMatrix4(mInvProjectionMtx);
}

void SCTEngine::EnableFrustumCulling(bool mode)
{
	mpCamera->UseFrustum(mode);

	Scene::SCTSceneManager::getSingleton().EnableFrustumCulling(mode);
}

}	// End of SCT Namespace