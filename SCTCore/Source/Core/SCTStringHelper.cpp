// -------------------- +
//		Includes
// -------------------- +

#include <Core\SCTStringHelper.h>


namespace SCT
{
namespace Core
{

SCTStringHelper::SCTStringHelper()
{
}

SCTStringHelper::~SCTStringHelper()
{
}

SCTReturn SCTStringHelper::Initialize()
{
	return OK;
}

SCTReturn SCTStringHelper::Shutdown()
{
	return OK;
}

std::vector<std::string> SCTStringHelper::tokenizeString(const std::string & str, const std::string & delims)
{
	// Skip delims at beginning, find start of first token
	std::string::size_type lastPos = str.find_first_not_of(delims, 0);
	// Find next delimiter @ end of token
	std::string::size_type pos     = str.find_first_of(delims, lastPos);
 
	// output vector
	std::vector<std::string> tokens;
 
	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delims.  Note the "not_of". this is beginning of token
		lastPos = str.find_first_not_of(delims, pos);
		// Find next delimiter at end of token.
		pos     = str.find_first_of(delims, lastPos);
	}

	return tokens;
}

std::wstring SCTStringHelper::stringToWString(const std::string& s)
{
	std::wstring temp(s.length(),L' ');
	std::copy(s.begin(), s.end(), temp.begin());

	return temp; 
}

std::string SCTStringHelper::wStringToString(const std::wstring& ws)
{
	std::string s(ws.begin(), ws.end());
	s.assign(ws.begin(), ws.end());

	return s;
}

// taken from: http://www.daniweb.com/software-development/cpp/threads/98542
std::string SCTStringHelper::removeWhitespaces(const std::string &inString)
{
	std::string				lTmpString	= inString;
	std::string::size_type	pos			= 0;
	bool					spacesLeft	= true;

	while(spacesLeft)
	{
		pos = lTmpString.find(" ");
		if( pos != std::string::npos )
			lTmpString.erase(pos, 1);
		else
			spacesLeft = false;
	}

	return lTmpString;
}

}	// End of Core	namespace
}	// End of SCT	namespace