// source/symbol.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef SymbolH
#define SymbolH
//---------------------------------------------------------------------------

#include <memory>
#include "object.h"
#include "types.h"
#include "grrlib_class.h"

/**
 * This class is used to draw X or O on the screen.
 * @author Crayon
 */
class Symbol : public Object
{
public:
    Symbol();
    Symbol(Symbol const&) = delete;
    ~Symbol() = default;
    Symbol& operator=(Symbol const&) = delete;
    void Paint() override;
    void SetPlayer(u8 APlayer);
    void SetLocation(Point APoint);
private:
    int Frame;
    std::unique_ptr<Texture> Img;
};
//---------------------------------------------------------------------------
#endif

// EOF
