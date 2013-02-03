#include <Scene\SCTSceneManager.h>
#include <Resource\SCTMesh.h>
#include <Scene\SCTCamera.h>


namespace SCT
{
namespace Scene
{

SCTSceneManager::SCTSceneManager()
:	mRootNode (NULL),
	mNodeCount (0),
	mpCamera (NULL),
	mbFrustumCulling (false)
{
	mRootNode	= new SCTSceneNode("Root_Node");
}

SCTSceneManager::~SCTSceneManager()
{
	Shutdown();
}

SCTReturn SCTSceneManager::Initilize()
{
	return OK;
}

SCTReturn SCTSceneManager::Update(float elapsedTime)		  
{
	mNodeCount = 0;

	// Update the camera
	mpCamera->Update();

	// Get camera Frustum & calculate the new one
	Math::Frustum &frustum = mpCamera->GetFrustum();


	// Parse the Scene tree and update the scene nodes
	std::vector<Scene::SCTSceneNode*> vNodeStack;
	vNodeStack.push_back(mRootNode);

	// Traverse the nodes - Depth first [Dragonbreath]
	while(!vNodeStack.empty())
	{
		// Get the back
		Scene::SCTSceneNode *current	= vNodeStack.back();
		vNodeStack.pop_back();

		// Update the node
		current->Update(elapsedTime);
		
		if( (current != mRootNode) && mbFrustumCulling )
			current->mbIsVisible = false;

		// --- Frustum Culling ---
		Resource::SCTMesh *mesh = NULL;
		current->GetData(&mesh);

		if(mesh && mpCamera->IsFrustumEnabled() && mbFrustumCulling)
		{
			for(SUInt32 itSubmesh = 0; itSubmesh < mesh->GetNumberOfSubmeshes(); itSubmesh++)
			{
				Resource::SCTSubmesh *submesh = mesh->GetSubmesh(itSubmesh);

				if( frustum.IsVisibleAABB(submesh->GetAABB()) )
					current->mbIsVisible = true;
			}
		}
		//\ Eof Frustum culling

		// Push all this node's children and put them in the stack
		std::list<Scene::SCTSceneNode*>::iterator it;			
		for(it = current->mChildren.begin(); it != current->mChildren.end(); it++)
		{
			vNodeStack.push_back(*it);
		}

		mNodeCount++;
	}

	return OK;
}

SCTReturn SCTSceneManager::Shutdown()
{
	if(mRootNode)
	{
		delete mRootNode;
		mRootNode	= NULL;
	}
	return OK;
}

SCTSceneNode* SCTSceneManager::GetSceneNode(const char *name)
{
	// Parse the Scene tree until you find the node
	std::vector<Scene::SCTSceneNode*> vNodeStack;
	vNodeStack.push_back(mRootNode);

	// Traverse the nodes - Depth first [Dragonbreath]
	while(!vNodeStack.empty())
	{
		// Get the back
		Scene::SCTSceneNode *current = vNodeStack.back();
		vNodeStack.pop_back();

		// Check the name
		if(!strcmp(current->mName.c_str(), name))
			return current;

		// Push all this node's children and put them in the stack
		std::list<Scene::SCTSceneNode*>::iterator it;			
		for(it = current->mChildren.begin(); it != current->mChildren.end(); it++)
		{
			vNodeStack.push_back(*it);
		}
	}

	return NULL;
}

SUInt32 SCTSceneManager::GetSceneNodes(const char *name, std::vector<SCTSceneNode*> &nodeList)
{
	// Parse the Scene tree until you find the node
	std::vector<Scene::SCTSceneNode*> vNodeStack;
	vNodeStack.push_back(mRootNode);

	// Traverse the nodes - Depth first [Dragonbreath]
	while(!vNodeStack.empty())
	{
		// Get the back
		Scene::SCTSceneNode *current = vNodeStack.back();
		vNodeStack.pop_back();

		// Check the name
		if(current->mName.find(name) != -1)
		{
			nodeList.push_back(current);
		}

		// Push all this node's children and put them in the stack
		std::list<Scene::SCTSceneNode*>::iterator it;			
		for(it = current->mChildren.begin(); it != current->mChildren.end(); it++)
		{
			vNodeStack.push_back(*it);
		}
	}

	return SUInt32(nodeList.size());
}


// distance: list of distances that dictates where the intersection(s) with this nodes geometry occured
// returns 0 if no intersection, else returns the number of intersections detected
// if stopAtFirst is enabled, the check will stop at the first intersection it detects
SUInt32 SCTSceneManager::CheckIntersection(SCTSceneNode *node, const Math::Ray &ray, std::vector<float> &distance, bool stopAtFirst)
{
	if(!node)
		return false;

	// Translate Ray to object space
	Math::Ray		icRay			= ray;
	Math::Vector3	nodeWorldPos	= node->GetLocalToWorldMatrix().GetTranslation();
	
	//icRay.mOrigin	+= nodeWorldPos;

	// Get mesh
	Resource::SCTMesh		*mesh		= NULL;
	Resource::SCTSubmesh	*tSubmesh;

	// Object geometry to test the ray against
	Render::SCTVertexPosTangBiNormTex	*geometry;
	SUInt32								*indices;
	SUInt32								vertexCount, indexCount;

	node->GetData(&mesh);
	
	if(!mesh)
		return false;
	
	// For every submesh in this mesh
	for(SUInt32 submeshIndex = 0; submeshIndex < mesh->GetNumberOfSubmeshes(); submeshIndex++)
	{
		tSubmesh	= NULL;
		tSubmesh	= mesh->GetSubmesh(submeshIndex);
		
		// Check if the submesh is valid
		if(!tSubmesh)
			return false;

		geometry	= NULL;
		indices		= NULL;
		vertexCount	= 0;
		indexCount	= 0;

		// Get vertex and index buffer from this submesh
		tSubmesh->GetVertexBuffer(&geometry, vertexCount);
		tSubmesh->GetIndexBuffer(&indices, indexCount);

	
		// Do the intersection testing for every triangle of the mesh
		if(geometry && indices)
		{
			SUInt32 numFaces	= indexCount / 3;
			float t	= 0;
	
			for(SUInt32 i = 0; i < numFaces; i++)
			{
				Math::Vector3 v0 = geometry[indices[3 * i + 0]].position;
				Math::Vector3 v1 = geometry[indices[3 * i + 1]].position;
				Math::Vector3 v2 = geometry[indices[3 * i + 2]].position;
	
				if(icRay.CheckIntersection(v0, v1, v2, t))
				{
					distance.push_back(t);
					if(stopAtFirst)
						return distance.size();
				}
			}
		}
	}

	return distance.size();
}

}	// End of Namespace Scene
}	// End of Namespace SCT