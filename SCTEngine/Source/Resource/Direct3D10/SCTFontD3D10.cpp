#include <SCTCore.h>
#include <SCTMath.h>

#include <Resource\Direct3D10\SCTFontD3D10.h>
#include <Resource\Direct3D10\SCTTextureD3D10.h>
#include <Render\SCTVertexDefs.h>


namespace SCT
{
namespace Resource
{

SCTFontD3D10::SCTFontD3D10(ID3D10Device *device)
{
	// Get the Direct3D device
	mpDevice	= device;
}
		
SCTFontD3D10::~SCTFontD3D10()
{
}

SCTReturn SCTFontD3D10::Initialize(char *fontFilename, WCHAR *textureFilename)
{
	// Load font data -- remember to delete this later since it gets alocated inside the filemanager
	mpFontType = Core::SCTFileManager::getSingleton().LoadFontData(fontFilename);

	mpTexture	= new Resource::SCTTextureD3D10(mpDevice);
	mpTexture->Initialize(textureFilename);
	
	return OK;
}

SCTReturn SCTFontD3D10::Update()
{
	return OK;
}

SCTReturn SCTFontD3D10::Shutdown()
{
	delete [] mpFontType;
	mpFontType	= NULL;

	delete mpTexture;
	mpTexture	= NULL;

	return OK;
}

void SCTFontD3D10::BuildVertexArray(char *sentence, void *vertices, float x, float y)
{
	Render::SCTVertexPosTexture* vertexPtr;
	int numLetters, index, i, letter;
		
	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (Render::SCTVertexPosTexture*)vertices;
	
	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);
	
	// Initialize the index to the vertex array.
	index = 0;
	
	// Draw each letter onto a quad.
	for(i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;
		
		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			x = x + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = Math::Vector3(x, y, 0.0f);  // Top left.
			vertexPtr[index].texture = Math::Vector2(mpFontType[letter].left, 0.0f);
			index++;
		
			vertexPtr[index].position = Math::Vector3((x + mpFontType[letter].size), (y - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = Math::Vector2(mpFontType[letter].right, 1.0f);
			index++;
		
			vertexPtr[index].position = Math::Vector3(x, (y - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = Math::Vector2(mpFontType[letter].left, 1.0f);
			index++;
		
			// Second triangle in quad.
			vertexPtr[index].position = Math::Vector3(x, y, 0.0f);  // Top left.
			vertexPtr[index].texture = Math::Vector2(mpFontType[letter].left, 0.0f);
			index++;
		
			vertexPtr[index].position = Math::Vector3(x + mpFontType[letter].size, y, 0.0f);  // Top right.
			vertexPtr[index].texture = Math::Vector2(mpFontType[letter].right, 0.0f);
			index++;
		
			vertexPtr[index].position = Math::Vector3((x + mpFontType[letter].size), (y - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = Math::Vector2(mpFontType[letter].right, 1.0f);
			index++;
		
			// Update the x location for drawing by the size of the letter and one pixel.
			x = x + mpFontType[letter].size + 1.0f;
		}
	}
}

void SCTFontD3D10::LoadFontData(char *fontFilename)
{
}

}	// End of Namespace Resource
}	// End of Namespace SCT