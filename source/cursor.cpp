// source/cursor.cpp
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#include "cursor.h"

#include <utility>

// Graphics
#include "hands.h"

/**
 * Constructor for the Cursor class.
 */
Cursor::Cursor() : Object(),
    Cursors(std::unique_ptr<Texture>(Texture::CreateFromPNG(hands)))
{
    Width = 96;
    Height = 96;

    // Load textures
    GRRLIB_InitTileSet(Cursors->AsGRRLIB(), Width, Height, 0);

    // Set hotspot
    Cursors->SetOffset(48, 45);
    Cursors->SetHandle(Cursors->GetOffsetX(), Cursors->GetOffsetY());

    // Default values
    SetPlayer(cursorType::X);
}

/**
 * Draw the cursor to screen.
 */
void Cursor::Paint()
{
    if(Visible)
    {
        // Draw the shadow
        Cursors->DrawTile(Left + 3, Top + 3, Angle, 1, 1, 0x00000000 | ((A(Color) == 0xFF) ? 0x44 : 0x11), Frame);
        // Draw the cursor
        Cursors->DrawTile(Left, Top, Angle, 1, 1, Color, Frame);
    }
}

/**
 * Set cursor.
 * @param[in] NewCType New cursor type to use.
 */
void Cursor::SetPlayer(cursorType NewCType)
{
    Frame = std::to_underlying(NewCType);
}

// EOF
