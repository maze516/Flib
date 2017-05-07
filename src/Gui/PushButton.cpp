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
#include <FRONTIER/Gui/PushButton.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>

namespace fgui
{
	void PushButton::applyState(InnerState state)
	{
		if (state == Normal)
			m_activeSprite = &m_spriteNorm;
			
		if (state == Hover)
			m_activeSprite = &m_spriteHover;
			
		if (state == Press)
			m_activeSprite = &m_spritePress;
		
		setSize(getSize());
		setPosition(getPosition());
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont) : GuiButton(cont),
											   m_activeSprite(&m_spriteNorm)
	{
		m_spriteNorm  = cont.getSprite("Button_Bckg_Norm" );
		m_spriteHover = cont.getSprite("Button_Bckg_Hover");
		m_spritePress = cont.getSprite("Button_Bckg_Press");
		
		setSize(getSize());
		setPosition(getPosition());
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont,const fm::String &text) : GuiButton(cont,text),
																	  m_activeSprite(&m_spriteNorm)
	{
		m_spriteNorm  = cont.getSprite("Button_Bckg_Norm" );
		m_spriteHover = cont.getSprite("Button_Bckg_Hover");
		m_spritePress = cont.getSprite("Button_Bckg_Press");
		
		setSize(getSize());
		setPosition(getPosition());
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::onDraw(fg::ShaderManager &shader)
	{
		m_activeSprite->onDraw(shader);
		
		GuiButton::onDraw(shader);
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setSize(fm::vec2s size)
	{
		fm::vec2s textSize = getGuiText().getSize();
		
		textSize += m_activeSprite->getFrameSize() * 2;
		
		if (size.w < textSize.w) size.w = textSize.w;
		if (size.h < textSize.h) size.h = textSize.h;
		
		GuiButton::setSize(size);
		
		m_activeSprite->setSize(getSize());
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setPosition(fm::vec2i pos)
	{
		GuiButton::setPosition(pos);
		
		m_activeSprite->setPosition(getPosition());
	}
}
