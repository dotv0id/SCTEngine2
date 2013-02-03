#include <Resource\SCTMaterial.h>

namespace SCT
{
namespace Resource
{

SCTMaterial::SCTMaterial()
:	mAmbientMap (NULL),
	mDiffuseMap (NULL),
	mSpecularMap (NULL),
	mBumpMap (NULL),
	mAlphaMap (NULL),
	mbEnableAlpha (false)
{
	/*
	mAmbientColor (Math::ColorRGBA(0,0,0,0)),
	mDiffuseColor (Math::ColorRGBA(0,0,0,0)),
	mSpecularColor (Math::ColorRGBA(0,0,0,0)),
	mShininess (0),
	mShininessStrength (0),
	*/
}

SCTMaterial::~SCTMaterial()
{
}

}	// End of Namespace Resource 
}	// End of Namespace SCT 