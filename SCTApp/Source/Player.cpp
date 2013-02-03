#include "Player.h"
#include <SCTMathDebug.h>
#include <SCTEngine.h>
#include <Resource\SCTMesh.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Scene\SCTSceneManager.h>
#include <Scene\SCTCamera.h>

#include <Input\Win32\SCTInputManagerWin32.h>
#include <Input\Win32\SCTKeyboardWin32.h>
#include <Input\Win32\SCTMouseWin32.h>


using namespace SCT;
using namespace Scene;
using namespace Demo;
using namespace Math;


Player::Player()
:	mpCamera (NULL),
	mCameraHeightOffset (5.0f),
	mpPlayerNode (NULL),
	mpWeaponNode (NULL),
	mPosition (Vector3(-50.0f, 0, 0)),
	mCameraOffset (Vector3(0, 0, 0)),
	mWeaponOffset (Vector3(0, 0, 0)),
	mMoveSpeed (7.0f),
	mSensitivity (0.05f),
	mbAccelerate (false),
	mCrouchLimit (3.0f),
	mbJump (false),
	mCrouchInit (mCameraHeightOffset),
	mJumpInit (mCameraHeightOffset),
	mJumpMax (5.0f),
	mCurrentPitch (0),
	mPitchLimit (float(Math::PI / 4))
{
}

Player::~Player()
{
	Shutdown();
}

SCTReturn Player::Initialize()
{
	// Get the render window resolution to configure the camera
	SCTResolution resolution = SCTEngine::getSingleton().GetResolution();

	// Create the player camera
	mpCamera	= new Scene::SCTCamera;
	mpCamera->SetProjectionMatrix(float(Math::HALFPI/2), float(resolution.width / resolution.height), 1.0f, 2000.0f);
	mpCamera->SetPosition(mPosition);
	mpCamera->UseFrustum(true);

	// Load the weapon Model
	Resource::SCTMeshD3D10 *weaponMesh = NULL;
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\awm.dae", &weaponMesh);

	// Create the Player scene node
	//mpPlayerNode = Scene::SCTSceneManager::getSingleton().GetRoot()->AddChild(NULL, "Player_node");
	//mpPlayerNode->SetScale(Math::Vector3(0.5f, 0.5f, 0.5f));

	//Matrix4 weaponLtW;
	mpWeaponNode = Scene::SCTSceneManager::getSingleton().GetRoot()->AddChild(weaponMesh, "PlayerWeapon");
	mpWeaponNode->SetPosition(Vector3(0, 0, 0));
	mpWeaponNode->SetScale(Vector3(0.015f, 0.015f, 0.015f));
	//mpWeaponNode->SetRotation(float(Math::HALFPI), 0, 0, Scene::SCTSceneNode::eParent);
	
	return OK;
}

SCTReturn Player::ProcessInput(float elapsedTime)
{
	elapsedTime;

	Input::SCTKeyboard	*keyboard	= Input::SCTInputManager::getSingleton().GetKeyboard();
	Input::SCTMouse		*mouse		= Input::SCTInputManager::getSingleton().GetMouse();

	// Move the camera
	if(keyboard->GetKeyState(Input::SCT_W))
		mpCamera->Move(mMoveSpeed * elapsedTime);
	if(keyboard->GetKeyState(Input::SCT_S))
		mpCamera->Move(-mMoveSpeed * elapsedTime);
	
	if(keyboard->GetKeyState(Input::SCT_A))
		mpCamera->MovePerpendicular(-mMoveSpeed * elapsedTime, 0);
	if(keyboard->GetKeyState(Input::SCT_D))
		mpCamera->MovePerpendicular(mMoveSpeed * elapsedTime, 0);

	// Adjust mouse sensitivity
	if(keyboard->IsKeyPressed(Input::SCT_EQUALS))
		mSensitivity	+= 0.1f;
	if(keyboard->IsKeyPressed(Input::SCT_MINUS))
		mSensitivity	-= 0.1f;

	keyboard->IsKeyReleased(Input::SCT_EQUALS);
	keyboard->IsKeyReleased(Input::SCT_MINUS);

	// Increase speed while Left Shift down
	if(keyboard->IsKeyPressed(Input::SCT_LSHIFT))
		mMoveSpeed	+= 5.0f;
	if(keyboard->IsKeyReleased(Input::SCT_LSHIFT))
		mMoveSpeed	-= 5.0f;

	// Look around
	float yawAngle = mouse->GetRelativeX() * elapsedTime * mSensitivity;
	mpCamera->RotateYaw(yawAngle);

	// Check pitch limits
	float pitchAngle = mouse->GetRelativeY() * elapsedTime * mSensitivity;
	mCurrentPitch	+= pitchAngle;

	if(mCurrentPitch < -mPitchLimit)
	{
		mCurrentPitch = -mPitchLimit;
		pitchAngle = 0;
	}
	if(mCurrentPitch > mPitchLimit)
	{
		mCurrentPitch = mPitchLimit;
		pitchAngle = 0;
	}
	
	// One circle
	if(yawAngle > float(Math::PI/2))
		yawAngle = float(Math::PI/2);
	if(yawAngle < -float(Math::PI/2))
		yawAngle = -float(Math::PI/2);

	//std::cout<<mCurrentPitch << ", " << mPitchLimit << "\n";
	mpCamera->RotatePitch(pitchAngle);			

	// Rotate the weapon
	//mpPlayerNode->SetRotation(0, pitchAngle, 0);
	//mpPlayerNode->SetRotation(yawAngle * 2.0f, 0, 0);

	// Center the mouse
	Core::SCTWindows::getSingleton().CenterMouseCursor();

	// Cast ray to find the height of the terrain
	Vector3 position	= mpCamera->GetPosition();
	position.y	= 500.0f;
	
	Math::Ray			terrainRay(position, Vector3(0, -1.0f, 0));
	std::vector<float>	distance;

	// We will test against the terrain
	Scene::SCTSceneNode *terrainNode;
	terrainNode	= Scene::SCTSceneManager::getSingleton().GetSceneNode("Terrain_Patch_00");
	
	if(Scene::SCTSceneManager::getSingleton().CheckIntersection(terrainNode, terrainRay, distance, true))
		position = terrainRay.GetPoint(distance[0]);

	
	// Set new position -- camera offset
	mPosition	= position;
	//mPosition	+= mCameraOffset;

	
	// --- Crouch ---
	static float crouchCurrent = mCrouchInit;
	if(keyboard->GetKeyState(Input::SCT_LCTRL))
	{
		if(crouchCurrent > mCrouchLimit)
			crouchCurrent -= 4.0f * elapsedTime;
	}
	else
	{
		if(crouchCurrent < mCameraHeightOffset)
			crouchCurrent += 4.0f * elapsedTime;
	}

	// Jump
	static float velocity = 0;
	if(keyboard->IsKeyPressed(Input::SCT_SPACE))
	{
		mbJump	= true;
		velocity = 10.0f;
	}

	keyboard->IsKeyReleased(Input::SCT_SPACE);

	// Calculate Jump
	static float jumpCurrent = 0;
	static float acc = 5.0f;
	
	if(mbJump)
	{
		if(jumpCurrent < mJumpMax)
		{
			velocity -= 9.8f * 2.0f * elapsedTime;				// Decrease the velocity
			jumpCurrent += velocity * elapsedTime;
		}

		if(jumpCurrent < 0)
		{
			mbJump		= false;
			velocity	= 0;
		}
	}
	else
	{
		if(jumpCurrent > 0)
		{
			velocity += 9.8f * 2.0f * elapsedTime;
			jumpCurrent -= velocity * elapsedTime;
		}
	}

	//std::cout<<jumpCurrent << ", " << mbJump <<"\n";
	mPosition.y += crouchCurrent + jumpCurrent;
	
	// --- Position the camera
	if(position.y	< 500.0f)
	{
		mpCamera->SetPosition(mPosition);
	}
	else
	{

		//mLastIn	= mPosition;
	}

	//Math::Matrix4 m;// = mpCamera->GetViewMatrix();

	//mPosition.y += 4.0f;
	//m.SetTranslation(mPosition);
	//mpWeaponNode->SetLocalToWorldMatrix(m);
	// --- Position the weapon
	//mpPlayerNode->SetPosition(mPosition);

	return OK;
}

SCTReturn Player::Update()
{
	return OK;
}

SCTReturn Player::Shutdown()
{
	delete mpCamera;
	mpCamera	= NULL;

	return OK;
}
