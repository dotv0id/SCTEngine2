#include <Scene\SCTSceneNode.h>


namespace SCT
{
namespace Scene
{
SCTSceneNode::SCTSceneNode()
:	mParent (NULL),
	mpData (NULL),
	mbIsVisible (true),
	mOrientation (Math::Quaternion(0, 0, 0, 1.0f)),
	mScale (Math::Vector3(1.0f, 1.0f, 1.0f)),
	mTranslation (Math::Vector3(0, 0, 0))
{
}

SCTSceneNode::SCTSceneNode(const std::string &name)
:	mParent (NULL),
	mName (name),
	mpData (NULL),
	mbIsVisible (true),
	mOrientation (Math::Quaternion(0, 0, 0, 1.0f)),
	mScale (Math::Vector3(1.0f, 1.0f, 1.0f)),
	mTranslation (Math::Vector3(0, 0, 0))
{
}

SCTSceneNode::~SCTSceneNode()
{
	DeleteAllChildren();
}

SCTReturn SCTSceneNode::Update(float elapsedTime)
{
	Math::Matrix4 rotMatrix;

	mLocalToWorldMatrix.Identity();
	mLocalToWorldMatrix.SetScale(mScale);

	mOrientation.GetRotationMatrix(rotMatrix);

	Math::Vector3 tranlation = mLocalToWorldMatrix.GetTranslation();

	mLocalToWorldMatrix = mLocalToWorldMatrix * rotMatrix;
	mLocalToWorldMatrix.SetTranslation(mTranslation);

	// Rotation
	if(mRotationSpace == eWorld)
	{
		
	}
	else if (mRotationSpace = eParent)
	{
		if(mParent)
			mLocalToWorldMatrix *= mParent->GetLocalToWorldMatrix();

		//mLocalToWorldMatrix.SetTranslation(tranlation);
		//mLocalToWorldMatrix = mLocalToWorldMatrix * rotMatrix;
		//
		//mTranslation += tranlation;
		//
		//mLocalToWorldMatrix.SetTranslation(mTranslation);		
	}


	// Reset
	//mTranslation = Math::Vector3(0,0,0);
	//Math::DumpMatrix4(mLocalToWorldMatrix);

	return OK;
}

void SCTSceneNode::DeleteAllChildren()
{
	std::list<SCTSceneNode*>::iterator it;
	for(it = mChildren.begin(); it != mChildren.end(); it++)
	{
		if(*it)
			delete *it;
	}

	mChildren.clear();
}

}	// End of Namespace Scene
}	// End of Namespace SCT