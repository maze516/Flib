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
#ifndef FRONTIER_SOCKET_HPP_INCLUDED
#define FRONTIER_SOCKET_HPP_INCLUDED

#include <FRONTIER/Network/BasicSocket.hpp>
#include <FRONTIER/System/macros/API.h>

namespace fn
{
	class FRONTIER_API Socket
    {
    protected:
        BasicSocket m_impl;
		fm::Result create(bool ipv6,bool tcp);
        
    public:
        void close();
		void shutdown(bool read = true,bool write = true);

        bool isValid() const;
        operator bool() const;
        
		void setBlocking(bool blocking = true);
		bool isBlocking() const;

        IpAddress getRemoteAddress() const;
		IpAddress getLocalAddress()  const;
        
		bool isReady() const;
		void setReady(bool ready = true);

        SocketID getID() const;
        
		BasicSocket &getImpl();
		const BasicSocket &getImpl() const;
    };
}

#endif // FRONTIER_SOCKET_HPP_INCLUDED
