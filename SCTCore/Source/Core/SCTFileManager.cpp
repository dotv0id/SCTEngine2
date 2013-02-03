// -------------------- +
//		Includes
// -------------------- +

#include <Core\SCTFileManager.h>


namespace SCT
{
namespace Core
{

SCTFileManager::SCTFileManager()
{
}

SCTFileManager::~SCTFileManager()
{
}


SCTReturn SCTFileManager::Initialize()
{
	return OK;
}

SCTReturn SCTFileManager::Shutdown()
{
	return OK;
}

// Reads config from file.
// Returns configstream & true
bool SCTFileManager::ReadConfigFile(const std::string &cnfgFilename, std::string &outConfigStream)
{
	std::stringstream	lFileInput;
	std::string			lBuffer;
	
	std::ifstream		lCnfgFile;
	
	lCnfgFile.open(cnfgFilename.c_str());

	// Open config file for input
	if(lCnfgFile.is_open())
	{
		while(lCnfgFile.good())
		{
			lBuffer.clear();

			std::getline(lCnfgFile, lBuffer);
			lFileInput << lBuffer << "\n";
		}		
	}else
		return FAIL;

	lCnfgFile.close();

	outConfigStream = lFileInput.str();

	return OK;
}

// writes config stream to file
// returns true
bool SCTFileManager::WriteConfigFile(const std::string &cnfgFilename, const std::string &inpConfigStream)
{
	std::ofstream lCfngFile;

	lCfngFile.open(cnfgFilename.c_str(), std::ios::out | std::ios::trunc);	// write to file deleting old content

	if(lCfngFile.is_open())
	{
		lCfngFile << inpConfigStream;
	}else
		return FAIL;

	lCfngFile.close();

	return OK;
}

SCTFontType* SCTFileManager::LoadFontData(const char *filename)
{
	std::ifstream	fin;
	int				i, numElements;
	char			temp;

	numElements		= 95;	
	
	// Create the font spacing buffer.
	SCTFontType *font = new SCTFontType[numElements];		// --> PUT THIS IN THE RESOURCE MANAGER TO RELEASE IT 
															// AT SHUTDOWN
	if(!font)
	{
		return NULL;
	}
	
	// Read in the font size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}
	
	// Read in the 95 used ascii characters for text.
	for(i=0; i < numElements; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
	
		fin >> font[i].left;
		fin >> font[i].right;
		fin >> font[i].size;
	}
	
	// Close the file.
	fin.close();

	return font;
}

}	// End of Core	namespace
}	// End of SCT	namespace