//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_FWLOG_HPP_INCLUDED
#define FRONTIER_FWLOG_HPP_INCLUDED
#include <FRONTIER/System/Log.hpp>
#define FRONTIER_FWLOG
namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief Log of the Window module
	/// 
	/// @ingroup Window
	/// 
	/// Every time an error occures in
	/// a class that is part of the Window module
	/// the bitshift (<<) operator will be called on 
	/// this instance
	/// 
	/////////////////////////////////////////////////////////////
    extern fm::Log fw_log;
}
#endif // FRONTIER_FWLOG_HPP_INCLUDED
