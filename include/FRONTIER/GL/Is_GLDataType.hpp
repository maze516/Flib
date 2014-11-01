////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_IS_GLDATATYPE_INCLUDED
#define FRONTIER_IS_GLDATATYPE_INCLUDED
#include <FRONTIER/GL/GL_TYPES.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a OpenGL Data Type 
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class is_GLDataType
	{
	public:
		enum {
			value   /** @cond DOXYGEN_HIDE */ = false /** @endcond */ ,///< True if T is a OpenGL Data Type 
			enumVal /** @cond DOXYGEN_HIDE */ = 0x0 /* GL_NONE */ /** @endcond */ ///< The corresponding OpenGL value
		};
	};

	/// @cond DOXYGEN_HIDE
	template<>
	class is_GLDataType<GLbyte>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1400 // GL_BYTE
		};
	};

	template<>
	class is_GLDataType<GLubyte>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1401 // GL_UNSIGNED_BYTE
		};
	};

	template<>
	class is_GLDataType<GLshort>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1402 // GL_SHORT
		};
	};

	template<>
	class is_GLDataType<GLushort>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1403 // GL_UNSIGNED_SHORT
		};
	};

	template<>
	class is_GLDataType<GLint>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1404 // GL_INT
		};
	};

	template<>
	class is_GLDataType<GLuint>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1405 // GL_UNSIGNED_INT
		};
	};

	template<>
	class is_GLDataType<GLfloat>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x1406 // GL_FLOAT
		};
	};

	template<>
	class is_GLDataType<GLdouble>
	{
	public:
		enum {
			value   = true,
			enumVal = 0x140a // GL_DOUBLE
		};
	};
	/// @endcond
}

#endif // FRONTIER_IS_GLDATATYPE_INCLUDED