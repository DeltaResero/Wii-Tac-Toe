// source/button.cpp
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#include <string>
#include "button.h"

// Graphics
#include "button_on.h"
#include "button_off.h"
#include "home_button.h"
#include "button_home.h"
#include "button_select.h"

/**
 * Constructor for the Button class.
 * @param[in] NewType Button type.
 */
Button::Button(buttonType NewType) : Object(),
    Type(NewType)
{
    switch(Type)
    {
        case buttonType::HomeMenu:
            // For HomeMenu buttons, the "On" texture is the same as the "Off" texture,
            // which is then tinted by the Paint() function.
            ButtonImgOn = Texture::CreateFromPNG(button_home);
            ButtonImgOff = Texture::CreateFromPNG(button_home);
            break;
        case buttonType::Home:
            ButtonImgOff = Texture::CreateFromPNG(home_button);
            break;
        default:
            ButtonImgOn = Texture::CreateFromPNG(button_on);
            ButtonImgOff = Texture::CreateFromPNG(button_off);
            ButtonSelected = Texture::CreateFromPNG(button_select);
    }

    Width = ButtonImgOff->GetWidth();
    Height = ButtonImgOff->GetHeight();
}

/**
 * Draw the button to screen.
 */
void Button::Paint()
{
    if(Type == buttonType::HomeMenu)
    {   // Draw shadow
        ButtonImgOff->Draw(Left + 4.0f, Top + 5.0f, 0, 1.0f, 1.0f, 0x00000055);
    }
    ButtonImgOff->Draw(Left, Top, 0, 1.0f, 1.0f, 0xFFFFFFFF);
    GRRLIB_PrintfTTF(TextLeft, TextTop, Font, Caption.c_str(), TextHeight, TextColor);

    if(Focused && ButtonImgOn)
    {
        u32 HoverColor = (Type == buttonType::HomeMenu) ? 0x0000FF33 : 0xFFFFFFFF;
        ButtonImgOn->Draw(Left, Top, 0, 1.0f, 1.0f, HoverColor);
    }

    if(Selected && ButtonSelected)
    {   // Select button
        ButtonSelected->Draw(Left - 8.0f, Top - 6.0f);
    }
}

/**
 * Set text height of the button.
 * @param[in] NewHeight height of the button.
 */
void Button::SetTextHeight(unsigned int NewHeight)
{
    TextHeight = NewHeight;
}

/**
 * Set caption on the button.
 * @param[in] NewCaption Text to put on the button.
 */
void Button::SetCaption(const std::string &NewCaption)
{
    Caption = NewCaption;
    TextWidth = GRRLIB_WidthTTF(Font, Caption.c_str(), TextHeight);
    TextTop = Top + (Height / 2) - (TextHeight / 2);
    TextLeft = Left + (Width / 2) - (TextWidth / 2);
    if(Type == buttonType::Home)
    {
        TextLeft += 20;
    }
}

/**
 * Set the font to use for the text on the button.
 * @param[in] AFont Font to use for the text on the button.
 */
void Button::SetFont(GRRLIB_ttfFont *AFont)
{
    Font = AFont;
}

/**
 * Set the button state, focused or not.
 * @param[in] IsFocused Set to true to set focus to the button, false otherwise.
 */
void Button::SetFocused(bool IsFocused)
{
    Focused = IsFocused;
}

/**
 * Set the button state, selected or not.
 * @param[in] IsSelected Set to true to select the button, false otherwise.
 */
void Button::SetSelected(bool IsSelected)
{
    Selected = IsSelected;
}

/**
 * Set the text color on the button.
 * @param[in] NewColor Color of the text when the button is not focused.
 */
void Button::SetTextColor(u32 NewColor)
{
    TextColor = NewColor;
}

// EOF
