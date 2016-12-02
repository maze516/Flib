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
#ifndef FRONTIER_REF_INL_INCLUDED
#define FRONTIER_REF_INL_INCLUDED

namespace fm
{
    /////////////////////////////////////////////////////////////
    template<class T>
    Ref<T>::Ref(T *objPtr) : m_ptr(objPtr)
    {

    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Ref<T>::Ref(T &objRef) : m_ptr(&objRef)
    {

    }

    /////////////////////////////////////////////////////////////
    template<class T>
    T *Ref<T>::operator->()
    {
        return m_ptr;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    const T *Ref<T>::operator->() const
    {
        return m_ptr;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    T &Ref<T>::operator*()
    {
        return *m_ptr;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    const T &Ref<T>::operator*() const
    {
        return *m_ptr;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Ref<T> &Ref<T>::operator=(T *pointer)
    {
        m_ptr = pointer;
        return *this;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Ref<T>::operator T*()
    {
        return m_ptr;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Ref<T>::operator const T*() const
    {
        return m_ptr;
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Ref<T>::operator bool() const
    {
        return m_ptr != nullptr;
    }
}

#endif // FRONTIER_REF_INL_INCLUDED
