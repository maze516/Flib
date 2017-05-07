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
#ifndef FRONTIER_PUSHBUTTON_HPP_INCLUDED
#define FRONTIER_PUSHBUTTON_HPP_INCLUDED

#include <FRONTIER/Graphics/FramedSprite.hpp>
#include <FRONTIER/Gui/GuiButton.hpp>

#define FRONTIER_PUSHBUTTON

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Pushable button with a callback and sprite as background
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class PushButton : public GuiButton
	{
		fg::FramedSprite *m_activeSprite; ///< The active state's sprite
		fg::FramedSprite m_spriteHover;   ///< The sprite to display when the button is hovered
		fg::FramedSprite m_spritePress;   ///< The sprite to display when pressed
		fg::FramedSprite m_spriteNorm;    ///< The sprite to display by default
		
	protected:
		/////////////////////////////////////////////////////////////
		/// @brief Apply a state to the button
		/// 
		/// @param state The state to apply
		/// 
		/////////////////////////////////////////////////////////////
		virtual void applyState(InnerState state) override;
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param cont The owner context
		/// 
		/////////////////////////////////////////////////////////////
		PushButton(GuiContext &cont);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the button with given text
		/// 
		/// @param cont The owner context
		/// @param text The text
		/// 
		/////////////////////////////////////////////////////////////
		PushButton(GuiContext &cont,const fm::String &text);
		
		/////////////////////////////////////////////////////////////
		/// @brief draw the button
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the button
		/// 
		/// @param size The size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the button
		/// 
		/// @param pos The position
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos) override;
	};
}

#endif // FRONTIER_PUSHBUTTON_HPP_INCLUDED
