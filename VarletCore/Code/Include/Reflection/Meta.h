#pragma once

namespace Varlet::Core
{
	enum TypeMetaFlags : uint8_t
	{
		/*
		* Type marked as abstract cannot be created in the editor or by reflection methods
		*/
		Abstract = BIT<1>,
		Interface = Abstract,

		//-----------------------------------------------------------------------
		//---------- Implicit Flags (Dont mark types with these flags) ----------
		//-----------------------------------------------------------------------

		/* Template type */
		GeneratedType = BIT<2>,

		Enum = BIT<3>
	};

	enum FieldMetaFlags : uint8_t
	{
		EditorAccessible = BIT<3>,

		//-----------------------------------------------------------------------
		//---------- Implicit Flags (Dont mark types with these flags) ----------
		//-----------------------------------------------------------------------

		Pointer	= BIT<1>,
		Static	= BIT<2>,
	};
}
