#include "Core\SCTBase.h"

namespace SCT
{

SCTBase::SCTBase()
{
	m_bIsInitialized	= false;
}
		
SCTBase::~SCTBase()
{
	if(m_bIsInitialized)
		Shutdown();
}
	
SCTReturn SCTBase::Initialize()
{
	m_bIsInitialized	= true;

	std::cout << "Base class initialized \n";

	return OK;
}		
		
SCTReturn SCTBase::Shutdown()
{
	m_bIsInitialized	= false;

	std::cout << "Base class Cleaned Up \n";

	return OK;
}

}	// End of SCT namespace