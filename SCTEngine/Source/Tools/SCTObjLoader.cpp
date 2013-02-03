#include <Tools\SCTObjLoader.h>
#include <Render\SCTVertexDefs.h>





namespace SCT
{
namespace Tools
{
	
SCTObjLoader::SCTObjLoader()
{
}

SCTObjLoader::~SCTObjLoader()
{
}

SCTReturn SCTObjLoader::LoadModelFromOBJ(const char *filename, Render::SCTVertexPosNormTex **rVertexBuffer, SUInt32 **rIndexBuffer, SUInt32 &rVertexCount, SUInt32 &vIndexCount)
{
	return OK;
}

SUInt32 SCTObjLoader::AddVertex(UINT hash, Render::SCTVertexPosNormTex *vertex)
{
    return 0;
}	    
	    
void SCTObjLoader::ClearLoaderBuffers()
{
	// Clear the mmvVertexCache
	ClearCache();

	mvVertices.clear();
	mvIndices.clear();
	mvAttributes.clear();
	//mvMaterials.clear();
}	    
	    
void SCTObjLoader::ClearCache()
{
	// Iterate through all the elements in the cache and subsequent linked lists
	std::vector<SCTCacheEntry*>::iterator it;

	for(it = mvVertexCache.begin(); it != mvVertexCache.end(); it++)
    {
        SCTCacheEntry* pEntry = *it;

        while( pEntry != NULL )
        {
            SCTCacheEntry* pNext = pEntry->pNext;
            delete pEntry;
            pEntry = pNext;
        }
    }

	mvVertexCache.clear();
}

}	// End of SCT Namespace
}	// End of Tools Namespace