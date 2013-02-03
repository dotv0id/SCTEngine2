#include <SCTCore.h>

#include <Resource\SCTResourceManager.h>
#include <Resource\SCTMaterial.h>
#include <Resource\Direct3D10\SCTTextureD3D10.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Resource\Direct3D10\SCTSkyBoxD3D10.h>

#include <Render\Direct3D10\SCTRenderInterfaceD3D10.h>
//#include <Tools\SCTObjLoader.h>

// ASSIMP
#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags


namespace SCT
{
namespace Resource
{

SCTResourceManager::SCTResourceManager()
{
}

SCTResourceManager::~SCTResourceManager()
{
	Shutdown();
}

SCTReturn SCTResourceManager::Initialize()
{
	return OK;
}

SCTReturn SCTResourceManager::Shutdown()
{
	// Release all loaded resources
	ReleaseAllResources();

	return OK;
}

SCTReturn SCTResourceManager::GetResource(const char *filename, SCTTexture **rTexture, bool environmentMap)
{
	SCTTextureD3D10	*texture	= NULL;

	// Check if this texture is loaded
	if(!CheckIfLoaded(filename))
	{
		texture	= new SCTTextureD3D10(Render::SCTRenderInterfaceD3D10::getSingleton().mpDevice);
		SCTReturn ret = texture->Initialize(Core::SCTStringHelper::getSingleton().stringToWString(filename).c_str(), environmentMap);

		SCTResource resTexture;
		resTexture.pResourcePtr	= (void*)texture;
		resTexture.type			= Resource::eTexture;

		SCTPrintD("Loading texture '%s' ptr: [%p]", filename, texture);

		AddResource(filename, resTexture);
	}
	else
	{
		std::map<std::string, SCTResource>::iterator it;

		it	= mResourceMap.find(filename);

		if(it != mResourceMap.end())
		{
			texture	= (SCTTextureD3D10*)it->second.pResourcePtr;
		}
		else
		{
			return RESOURCE_NOT_FOUND;
		}
	}		

	// Redirect the pointer
	*rTexture	= texture;

	return OK;
}

SCTReturn SCTResourceManager::GetResource(const char *filename, SCTSkyBox **rSkyBox, float radius)
{
	SCTSkyBox	*skybox	= NULL;

	// Generate the name of the skybox
	std::string name	= "skybox_";
	name.append(filename);

	// Check if this texture is loaded
	if(!CheckIfLoaded(name.c_str()))
	{
		// Get the skybox texture
		SCTTexture *cubeMapTexture = NULL;

		GetResource(filename, &cubeMapTexture, true);
		skybox	= new SCTSkyBoxD3D10(Render::SCTRenderInterfaceD3D10::getSingleton().mpDevice);
		SCTReturn ret = skybox->Initialize(cubeMapTexture, radius);

		SCTResource resSkybox;
		resSkybox.pResourcePtr	= (void*)skybox;
		resSkybox.type			= Resource::eEnvironmentMap;

		SCTPrintD("Loading skybox '%s' ptr: [%p]", filename, skybox);

		AddResource(name.c_str(), resSkybox);
	}
	else
	{
		std::map<std::string, SCTResource>::iterator it;

		it	= mResourceMap.find(name.c_str());

		if(it != mResourceMap.end())
		{
			skybox	= (SCTSkyBox*)it->second.pResourcePtr;
		}
		else
		{
			return RESOURCE_NOT_FOUND;
		}
	}		

	// Redirect the pointer
	*rSkyBox	= skybox;

	return OK;
}

SCTReturn SCTResourceManager::GetResource(const char *filename, SCTMaterial **rMaterial)
{
	SCTMaterial	*material	= NULL;

	// Check if this texture is loaded
	if(!CheckIfLoaded(filename))
	{
		material	= new SCTMaterial;

		// Check if we need to create a default material
		if(strcmp(filename, "defaultMaterial") == 0)
		{
			material->mMaterialProperties.mAmbientColor			= Math::ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f);
			material->mMaterialProperties.mDiffuseColor			= Math::ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f);
			material->mMaterialProperties.mSpecularColor		= Math::ColorRGBA(0, 0, 0, 1.0f);
			material->mMaterialProperties.mShininess			= 0;
			material->mMaterialProperties.mShininessStrength	= 0;
		}

		SCTResource resMaterial;
		resMaterial.pResourcePtr	= (void*)material;
		resMaterial.type			= Resource::eMaterial;

		SCTPrintD("Loading material '%s' ptr: [%p]", filename, material);

		AddResource(filename, resMaterial);

		// Redirect the pointer
		*rMaterial	= material;

		return NEW_RESOURCE_ALLOCATED;
	}
	else
	{
		std::map<std::string, SCTResource>::iterator it;

		it	= mResourceMap.find(filename);

		if(it != mResourceMap.end())
		{
			material	= (SCTMaterial*)it->second.pResourcePtr;

			// Redirect the pointer
			*rMaterial	= material;

			return OK;
		}
		else
		{
			return RESOURCE_NOT_FOUND;
		}
	}		

	return FAIL;
}

SCTReturn SCTResourceManager::GetResource(const char *filename, SCTMeshD3D10 **rMesh, bool load)
{
	SCTMeshD3D10	*mesh	= NULL;

	// Check if this mesh is loaded
	if(!CheckIfLoaded(filename))
	{
		mesh	= new SCTMeshD3D10(Render::SCTRenderInterfaceD3D10::getSingleton().mpDevice);
		
		// Add the default material to slot 0
		Resource::SCTMaterial *defaultMaterial	= NULL;

		// Use assimp to load the model
		if(load)
		{
			Load3DModelFromFile(filename, mesh);
			mesh->Initialize();
		}
		else
		{
			// Return a mesh with one submesh
			Resource::SCTSubmesh *submesh	= new Resource::SCTSubmeshD3D10(Render::SCTRenderInterfaceD3D10::getSingleton().mpDevice);

			// Use the default material for this submesh
			GetResource("defaultMaterial", &defaultMaterial);
			submesh->SetMaterial(defaultMaterial);
			mesh->AddSubmesh(submesh);
		}

		SCTResource resMesh;
		resMesh.pResourcePtr	= (void*)mesh;
		resMesh.type			= Resource::eMesh;

		SCTPrintD("Loading mesh '%s' ptr: [%p]", filename, mesh);

		AddResource(filename, resMesh);
	}
	else
	{
		std::map<std::string, SCTResource>::iterator it;

		it	= mResourceMap.find(filename);

		if(it != mResourceMap.end())
		{
			mesh	= (SCTMeshD3D10*)it->second.pResourcePtr;
		}
		else
		{
			return RESOURCE_NOT_FOUND;
		}
	}		

	// Redirect the pointer
	*rMesh	= mesh;

	return OK;
}

void SCTResourceManager::AddResource(const char *filename, SCTResource &resource)
{
	mResourceMap.insert(std::pair<std::string, SCTResource>(filename, resource));

	/*
	for(std::map<std::string, SCTResource*>::iterator it = mResourceMap.begin(); it != mResourceMap.end(); it++)
	{
		std::cout	<<"Key = " << it->first << ", SCTResource ptr = " << it->second->pResourcePtr 
					<< ", of type: " << it->second->type << "\n";
	}
	*/
}

void SCTResourceManager::ReleaseResource(SCTResource &resource)
{
	// Find resource in map, remove it from map and release it
	// ...
}

void SCTResourceManager::ReleaseAllResources()
{
	std::map<std::string, SCTResource>::iterator it;

	for(it = mResourceMap.begin(); it != mResourceMap.end(); it++)
	{
		std::cout << "Key=" << it->first << ", [ptr: " << it->second.pResourcePtr << ", type: " << it->second.type << "]\n";

		switch(it->second.type)
		{
		case eMesh:
			{
				delete static_cast<SCTMesh*>(it->second.pResourcePtr);
			} break;
		case eMaterial:
			{
				delete static_cast<SCTMaterial*>(it->second.pResourcePtr);
			} break;
		case eTexture:
			{
				delete static_cast<SCTTexture*>(it->second.pResourcePtr);
			} break;
		case eEnvironmentMap:
			{
				delete static_cast<SCTSkyBoxD3D10*>(it->second.pResourcePtr);
			} break;
		}
	}

	mResourceMap.clear();
}

bool SCTResourceManager::CheckIfLoaded(const char *filename)
{
	std::map<std::string, SCTResource>::iterator it;

	it	= mResourceMap.find(filename);

	if(it != mResourceMap.end())
		return true;

	return false;
}

SCTReturn SCTResourceManager::Load3DModelFromFile(const char *filename, SCTMeshD3D10 *rMesh)
{
	Assimp::Importer importer;

	std::string f(filename);

	if(!(f.length() > 0))
		return FAIL;

	const aiScene* scene = importer.ReadFile(filename,	aiProcess_CalcTangentSpace | 
														aiProcess_Triangulate |
														aiProcess_JoinIdenticalVertices |
														aiProcess_SortByPType);


	// If the import failed, report it
	if(!scene)
	{
	 // Assimp::DoTheErrorLogging( importer.GetErrorString());
	  return FAIL;
	}

	SCTPrintD("**************************************************");
	SCTPrintD("[Assimp]: '%s', number of submeshes: %d", filename, scene->mNumMeshes);

	// Read all the Submeshes that make this mesh
	for(SUInt32 i = 0; i < scene->mNumMeshes; i++)
	{
		// Allocate new submesh
		SCTSubmesh *submesh	= new SCTSubmeshD3D10(Render::SCTRenderInterfaceD3D10::getSingleton().mpDevice);

		// Read the data to the Submesh
		// Vertex Buffer
		SUInt32 numVertices = (SUInt32)scene->mMeshes[i]->mNumVertices;
		//Render::SCTVertexPosNormTex *vertices = new Render::SCTVertexPosNormTex[numVertices];
		Render::SCTVertexPosTangBiNormTex *vertices = new Render::SCTVertexPosTangBiNormTex[numVertices];

		// For bound box calculations
		Math::Vector3 bbmin(0,0,0), bbmax(0,0,0);

    std::ofstream fileOut;
    fileOut.open("kyubo.zmesh");


		for(SUInt32 j = 0; j < numVertices; j++)
		{
			// Vertices
			vertices[j].position.x	= scene->mMeshes[i]->mVertices[j].x;
			vertices[j].position.y	= scene->mMeshes[i]->mVertices[j].y;
			vertices[j].position.z	= scene->mMeshes[i]->mVertices[j].z;

      fileOut << "cubeVerticesRaw["<<j<<"].position  = Math::Vector3(" << vertices[j].position.x << ", " << vertices[j].position.y << ", " << vertices[j].position.z << ");\n";

			// Tangents
			vertices[j].tangent.x	= scene->mMeshes[i]->mTangents[j].x;
			vertices[j].tangent.y	= scene->mMeshes[i]->mTangents[j].y;
			vertices[j].tangent.z	= scene->mMeshes[i]->mTangents[j].z;

			// Binormals
			vertices[j].binormal.x	= scene->mMeshes[i]->mBitangents[j].x;
			vertices[j].binormal.y	= scene->mMeshes[i]->mBitangents[j].y;
			vertices[j].binormal.z	= scene->mMeshes[i]->mBitangents[j].z;

			// Normals
			vertices[j].normal.x	= scene->mMeshes[i]->mNormals[j].x;
			vertices[j].normal.y	= scene->mMeshes[i]->mNormals[j].y;
			vertices[j].normal.z	= scene->mMeshes[i]->mNormals[j].z;

      fileOut << "cubeVerticesRaw["<<j<<"].normal    = Math::Vector3("<< vertices[j].normal.x << "," << vertices[j].normal.y << "," << vertices[j].normal.z <<");\n";

			// Texture coordinates
			vertices[j].texture.x	= scene->mMeshes[i]->mTextureCoords[0][j].x;
			vertices[j].texture.y	= scene->mMeshes[i]->mTextureCoords[0][j].y;


			// Calculate the bound box of the submesh
			if(vertices[j].position.x < bbmin.x) bbmin.x = vertices[j].position.x;
			if(vertices[j].position.x > bbmax.x) bbmax.x = vertices[j].position.x;
						
			if(vertices[j].position.y < bbmin.y) bbmin.y = vertices[j].position.y;
			if(vertices[j].position.y > bbmax.y) bbmax.y = vertices[j].position.y;
						
			if(vertices[j].position.z < bbmin.z) bbmin.z = vertices[j].position.z;
			if(vertices[j].position.z > bbmax.z) bbmax.z = vertices[j].position.z;
		}

		// Index Buffer
		SUInt32 numIndices = (SUInt32)scene->mMeshes[i]->mNumFaces * 3;
		SUInt32	*indices	= new SUInt32[numIndices];

		SUInt32 index	= 0;
		for(SUInt32 j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			indices[index]		= scene->mMeshes[i]->mFaces[j].mIndices[0];
			indices[index + 1]	= scene->mMeshes[i]->mFaces[j].mIndices[1];
			indices[index + 2]	= scene->mMeshes[i]->mFaces[j].mIndices[2];

      fileOut << indices[index] << ", " << indices[index + 1] << ", " << indices[index + 2] << "\n";
			index += 3;
		}		

		// --- Do a BoundBox calculation ---
		Math::BoundingBox aabb(bbmin, bbmax);

		submesh->SetVertexBuffer(vertices, numVertices, Render::ePosTangBiNormTex);
		submesh->SetIndexBuffer(indices, numIndices);
		submesh->SetMaterialIndex(scene->mMeshes[i]->mMaterialIndex);		// Deprecated
		submesh->SetAABB(aabb);

		
		SCTPrintD("Num vertices: %d, Num indices: %d", numVertices, numIndices);
		
		// Get the material used by this submesh
		SCTMaterial *material	= NULL;
		aiMaterial	*mat		= scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

		aiString	name;
		mat->Get(AI_MATKEY_NAME, name);

		// if not default material, pass it to the mesh class
		if(strcmp(name.data, "DefaultMaterial"))
		{
			SCTPrintD("Material index: %d, name: '%s'", scene->mMeshes[i]->mMaterialIndex, name.data);

			// Create new material or get from the manager
			SCTReturn ret = GetResource(name.data, &material);

			if(ret = NEW_RESOURCE_ALLOCATED)	// New material has been created, fill it up
			{
				SCTPrintD("NEW MATERIAL ALLOCATED: %s", name.data);

				aiColor3D	ambient(0,0,0);
				aiColor3D	diffuse(0,0,0);
				aiColor3D	specular(0,0,0);
				float		shininess,
							shininessStrength;

				//mat->Get(AI_MATKEY_NAME, name);
				mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
				mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
				mat->Get(AI_MATKEY_SHININESS, shininess);
				mat->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);


				// Fill the SCTMaterial up
				material->mName										= name.data;
				material->mMaterialProperties.mAmbientColor			= Math::ColorRGBA(ambient.r, ambient.g, ambient.b, 1.0f);
				material->mMaterialProperties.mDiffuseColor			= Math::ColorRGBA(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				material->mMaterialProperties.mSpecularColor		= Math::ColorRGBA(specular.r, specular.g, specular.b, 1.0f);
				material->mMaterialProperties.mShininess			= shininess;
				material->mMaterialProperties.mShininessStrength	= shininessStrength;

				// Debug info
				SCTPrintD("Ambient color (RGB): %f, %f, %f", ambient.r, ambient.g, ambient.g);
				SCTPrintD("Diffuse color (RGB): %f, %f, %f", diffuse.r, diffuse.g, diffuse.g);
				SCTPrintD("Specular color (RGB): %f, %f, %f", specular.r, specular.g, specular.g);
				SCTPrintD("Shininess: %f", material->mMaterialProperties.mShininess);
				SCTPrintD("Shininess strength: %f", material->mMaterialProperties.mShininessStrength);

				// Loat the textures this material uses
				aiString	nameTexAmbient,
							nameTexDiffuse,
							nameTexSpecular,
							nameTexBump,
							nameTexAlpha;

				mat->GetTexture(aiTextureType_AMBIENT, i-1, &nameTexAmbient);
				mat->GetTexture(aiTextureType_DIFFUSE, i-1, &nameTexDiffuse);
				mat->GetTexture(aiTextureType_SPECULAR, i-1, &nameTexSpecular);
				mat->GetTexture(aiTextureType_HEIGHT, i-1, &nameTexBump);
				mat->GetTexture(aiTextureType_OPACITY, i-1, &nameTexAlpha);

				/*
				std::cout<< "{{1: " <<mat->GetTextureCount(aiTextureType::aiTextureType_AMBIENT) <<"}}\n";
				std::cout<< "{{2: " <<mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) <<"}}\n";
				std::cout<< "{{3: " <<mat->GetTextureCount(aiTextureType::aiTextureType_DISPLACEMENT) <<"}}\n";
				std::cout<< "{{4: " <<mat->GetTextureCount(aiTextureType::aiTextureType_EMISSIVE) <<"}}\n";
				std::cout<< "{{5: " <<mat->GetTextureCount(aiTextureType::aiTextureType_HEIGHT) <<"}}\n";
				std::cout<< "{{6: " <<mat->GetTextureCount(aiTextureType::aiTextureType_LIGHTMAP) <<"}}\n";
				std::cout<< "{{7: " <<mat->GetTextureCount(aiTextureType::aiTextureType_NONE) <<"}}\n";
				std::cout<< "{{8: " <<mat->GetTextureCount(aiTextureType::aiTextureType_NORMALS) <<"}}\n";
				std::cout<< "{{9: " <<mat->GetTextureCount(aiTextureType::aiTextureType_OPACITY) <<"}}\n";
				std::cout<< "{{10: " <<mat->GetTextureCount(aiTextureType::aiTextureType_REFLECTION) <<"}}\n";
				std::cout<< "{{11: " <<mat->GetTextureCount(aiTextureType::aiTextureType_SHININESS) <<"}}\n";
				std::cout<< "{{12: " <<mat->GetTextureCount(aiTextureType::aiTextureType_SPECULAR) <<"}}\n";
				std::cout<< "{{13: " <<mat->GetTextureCount(aiTextureType::aiTextureType_UNKNOWN) <<"}}\n";
				*/

				// Debug
				SCTPrintD("Texture maps:");
				SCTPrintD("Ambient map: '%s'", nameTexAmbient.data);
				SCTPrintD("Diffuse map: '%s'", nameTexDiffuse.data);
				SCTPrintD("Specular map: '%s'", nameTexSpecular.data);
				SCTPrintD("Bump map: '%s'", nameTexBump.data);
				SCTPrintD("Alpha map: '%s'", nameTexAlpha.data);

				// If this submeshe's material uses alpha blending, lower the priority
				if(strcmp(nameTexDiffuse.data, nameTexAlpha.data) == 0)
				{
					// Material uses alpha channel
					material->mbEnableAlpha = true;

					// Should be of lower rendering priority
					submesh->SetRenderPriority(0);
				}

				SCTTexture *texAmbient		= NULL;
				SCTTexture *texDiffuse		= NULL;
				SCTTexture *texSpecular		= NULL;
				SCTTexture *texBump			= NULL;

				std::string path	= "";

				// Ambient texture map
				if(nameTexAmbient.length > 0)
				{
					path = "Media\\";
					path.append(nameTexAmbient.data);
					
					ret = GetResource(path.c_str(), &texAmbient);
					path.clear();
				}

				// Diffuse texture map
				if(nameTexDiffuse.length > 0)
				{
					path = "Media\\";
					path.append(nameTexDiffuse.data);

					ret = GetResource(path.c_str(), &texDiffuse);
					path.clear();
				}

				// Specular texture map
				if(nameTexSpecular.length > 0)
				{
					path = "Media\\";
					path.append(nameTexSpecular.data);

					ret = GetResource(path.c_str(), &texSpecular);
					path.clear();
				}

				// Specular texture map
				if(nameTexBump.length > 0)
				{
					path = "Media\\";
					path.append(nameTexBump.data);

					ret = GetResource(path.c_str(), &texBump);
					path.clear();
				}

				material->mAmbientMap	= texAmbient;
				material->mDiffuseMap	= texDiffuse;
				material->mSpecularMap	= texSpecular;
				material->mBumpMap		= texBump;

			} //  End of Get material from resource manager
		}	// End of Default material check
		//\ EOF Material

		// Pass material to the submesh
		submesh->SetMaterial(material);

		// Add the submesh to the mesh
		rMesh->AddSubmesh(submesh);

		SCTPrintD("**************************************************");
	}

	/*
	// # Dump vertices
	std::cout<<"# Dumping vertices [N.o. vertices: " << numVertices << "]\n";
	for(int i = 0; i < numVertices; i++)
	{
		std::cout << "Vertex " << i << "\n";
		std::cout << vertices[i].position.x << "\t" << vertices[i].position.y << "\t" << vertices[i].position.z << "\n";
		std::cout << vertices[i].normal.x	<< "\t" << vertices[i].normal.y	  << "\t" << vertices[i].normal.z << "\n";
		std::cout << vertices[i].texture.x	<< "\t" << vertices[i].texture.y << "\n";
		std::cout << "-----\n";
	}
	*/

	/*
	// # Dump Indices
	std::cout << "# Dumping indices [N.o. indices: " << numIndices << "]\n";
	for(int i = 0; i <  numIndices; i++)
	{
		std::cout << indices[i] << "\t" << indices[i+1] << "\t" << indices[i+2] << "\n";
		i += 2;
	}
	*/

	//importer.FreeScene();

	return OK;
}

}	// End of Resource Namespace
}	// End of SCT Namespace