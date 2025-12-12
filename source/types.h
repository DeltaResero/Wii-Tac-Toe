// source/types.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef TypesH
#define TypesH
//---------------------------------------------------------------------------

#include <gctypes.h>

/**
 * Class used to defines a pixel location onscreen.
 * @author Crayon
 */
class Point
{
private:
    u16 x;
    u16 y;
public:
    /**
     * Constructor for the Point class.
     * Initializes a point at the origin (0, 0) of the coordinate space.
     */
    constexpr Point() : x(0), y(0)
    {
    }
    /**
     * Constructor for the Point class.
     * @param[in] x Specifies the x-coordinate.
     * @param[in] y Specifies the y-coordinate.
     */
    constexpr Point(u16 x, u16 y) : x(x), y(y)
    {
    }
    /**
     * Get the x position.
     */
    [[nodiscard]] constexpr u16 GetX() const
    {
        return x;
    }
    /**
     * Get the y position.
     */
    [[nodiscard]] constexpr u16 GetY() const
    {
        return y;
    }
    /**
     * Set the new position.
     * @param[in] x Specifies the x-coordinate.
     * @param[in] y Specifies the y-coordinate.
     */
    constexpr void SetLocation(u16 x, u16 y)
    {
        this->x = x;
        this->y = y;
    }
    /**
     * Compare two Point object.
     * @param[in] pt The Point to evaluate.
     * @return If the Point meet the comparison condition, the operators return a true value. Otherwise, false is returned.
     */
    constexpr bool operator ==(const Point& pt) const
    {
        return (x == pt.x) && (y == pt.y);
    }
    /**
     * Compare two Point object.
     * @param[in] pt The Point to evaluate.
     * @return If the Point meet the comparison condition, the operators return a true value. Otherwise, false is returned.
     */
    constexpr bool operator !=(const Point& pt) const
    {
        return !(pt == *this);
    }
};
//---------------------------------------------------------------------------
#endif

// EOF
