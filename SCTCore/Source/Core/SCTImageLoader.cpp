// -------------------- +
//		Includes
// -------------------- +

#include <Core\SCTImageLoader.h>
#include <Core\SCTStringHelper.h>
#include <Core\SCTLogManager.h>

#include <IL\il.h>
#include <IL\ilu.h>


namespace SCT
{
namespace Core
{

SCTImageLoader::SCTImageLoader()
{
}

SCTImageLoader::~SCTImageLoader()
{
}

SCTReturn SCTImageLoader::Initialize()
{
	// initialize DevIL image loader library
	ilInit();
	iluInit();

	return OK;
}

SCTReturn SCTImageLoader::Shutdown()
{
	return OK;
}

SCTReturn SCTImageLoader::LoadImageFromFile(const wchar_t * filename, std::vector<ColorRGB> &rawImageData, bool resize, SUInt32 width, SUInt32 height)
{
	ILuint iImage	= (ILuint) -1;
	ilGenImages(1, &iImage);
	ilBindImage(iImage);
	
	// Image has been loader
	if(ilLoadImage(filename) == IL_TRUE)
	{
		// Convert image
		if(!ilConvertImage(IL_RGB, IL_FLOAT))
			SCTLogManager::getSingleton().PrintDebug("[SCTImageLoader] Could not convert image to RGB, float values\n");
		
		// Resize image, if neccessary
		if(resize)
			if(!iluScale(width, height, 1))
				SCTLogManager::getSingleton().PrintDebug("[SCTImageLoader] Could not scale image...");

		std::wstring file	= filename;

		// Get the raw data from the image
		SInt32 imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
		SInt32 imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);

		SCTLogManager::getSingleton().PrintDebug(	"[SCTImageLoader] Image '%s' has been loaded...", 
													SCTStringHelper::getSingleton().wStringToString(file));

		std::cout<< "Image width: "	<< imgWidth << ", height: " << imgHeight << "\n" ;
		std::cout<< "Image target width: " << width << ", height: " << height << "\n" ;

		// Copy the image's raw data
		ILfloat *bytes	 = new ILfloat[imgWidth * imgHeight * 3];
		ilCopyPixels(0, 0, 0, imgWidth, imgHeight, 1, IL_RGB, IL_FLOAT, bytes);

		for(int i = 0; i < imgHeight; i++)
		{
			for(int j = 0; j < imgWidth; j++)
			{
				ColorRGB color;

				color.r	= float(bytes[(i * imgWidth + j) * 3 + 0]);
				color.g	= float(bytes[(i * imgWidth + j) * 3 + 1]);
				color.b = float(bytes[(i * imgWidth + j) * 3 + 2]);

				rawImageData.push_back(color);
			}
		}

		delete []bytes;
		bytes = NULL;
	}

	ilDeleteImage(iImage);

	ILenum Error;
	while ((Error = ilGetError()) != IL_NO_ERROR)
	{
		const wchar_t *error	 = iluErrorString(Error);
		std::cout<<"SCTImageLoader " << SCTStringHelper::getSingleton().wStringToString(error);
	}

	return OK;
}


}	// End of Core Namespace
}	// End of SCT Namespace