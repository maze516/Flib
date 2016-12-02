////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_IS_CONTAINER_HPP_INCLUDED
#define FRONTIER_IS_CONTAINER_HPP_INCLUDED
#include <FRONTIER/System/type_traits/YesNo.hpp>
#define FRONTIER_IS_CONTAINER

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a container type (has const_iterator, begin and end)
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class Is_container
	{
		template<class Type,Type t>
		class SFINAE
		{
			
		};
		
		template<class U> 
		static fm::priv::Yes TestBeg(SFINAE<typename U::const_iterator (U::*)() const,&U::begin>*);
		
		template<class U> 
		static fm::priv::No  TestBeg(...);
		
		template<class U> 
		static fm::priv::Yes TestEnd(SFINAE<typename U::const_iterator (U::*)() const,&U::end>*);
		
		template<class U> 
		static fm::priv::No  TestEnd(...);
		/// @endcond
		
	public:
		enum {
			value /** @cond DOXYGEN_HIDE */ = ((sizeof(TestBeg<T>(0)) == sizeof(fm::priv::Yes)) && (sizeof(TestEnd<T>(0)) == sizeof(fm::priv::Yes))) /** @endcond */ ///< True iff T::const_iterator is a type
		};
	};
}

#endif // FRONTIER_IS_CONTAINER_HPP_INCLUDED