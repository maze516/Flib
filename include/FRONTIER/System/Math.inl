////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_MATH_INL_INCLUDED
#define FRONTIER_MATH_INL_INCLUDED

namespace fm
{
	template<class T>
	inline T ipow(T t,unsigned int exponent,T one)
	{
		if (exponent == 0) return one;
		
		T r = ipow(t,exponent/2,one);
		
		if (exponent % 2 == 0) {
			return r*r;
		}
		
		return t*r*r;
	}
}

#endif // FRONTIER_MATH_INL_INCLUDED