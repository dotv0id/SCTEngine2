/**
----------------------------------------------------------------------------
	@File:			SCTMaterial.h
   
	System:         SCT Game Engine
	Component Name: SCTMaterial
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Header file for Project X
					This file contains the defined types for Project X
					This is sometimes called the "Abstract" and may be
					followed by a section called "Notes".
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/

#ifndef SCT_MATERIAL_H
#define SCT_MATERIAL_H


#include <SCT.h>
#include <SCTMath.h>
#include <Resource\SCTTexture.h>


namespace SCT
{
	// Foreward class declarations
	namespace Resource
	{
		class SCTResourceManager;
	}

	namespace Render
	{
		class SCTRenderInterfaceD3D10;
	}

	namespace Resource
	{
		/// class SCTMaterial
		//  ...
		class SCTMaterial
		{
		public:
			SCTMaterial();
			~SCTMaterial();

			// Material properties -- so we can send them to the shader
			struct MaterialProperties
			{
				Math::ColorRGBA		mAmbientColor,
									mDiffuseColor,
									mSpecularColor;

				float				mShininess,
									mShininessStrength;
			};

			inline std::string&		GetName()
			{
				return mName;
			}

			inline void				SetTexture(Resource::SCTTexture *texture, SCTTextureType type)
			{
				switch(type)
				{
				case eAmbient:
					{
						mAmbientMap	= texture;
					} break; 
				case eDiffuse:
					{
						mDiffuseMap = texture;
					} break;
				case eSpecular:
					{
						mSpecularMap= texture;
					} break;
				case eAlpha:
					{
						mAlphaMap	= texture;
					} break;
				case eBump:
					{
						mBumpMap	= texture;
					} break;
				};
			}

		private:
			// material name
			std::string				mName;

			// Material properties
			MaterialProperties		mMaterialProperties;
			
			// Textures
			SCTTexture				*mAmbientMap,
									*mDiffuseMap,
									*mSpecularMap,
									*mBumpMap,
									*mAlphaMap;

			// If true, use the AlphaBlendingEnabled technique
			bool					mbEnableAlpha;

		public:
			friend class SCTResourceManager;
			friend class Render::SCTRenderInterfaceD3D10;
		};
	}	// End of Namespace Resource 
}	// End of Namespace SCT 
#endif