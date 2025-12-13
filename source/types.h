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
 * Simple struct to define a pixel location onscreen.
 * @author Crayon
 */
struct Point
{
    u16 x; /**< The x-coordinate. */
    u16 y; /**< The y-coordinate. */

    /**
     * Default constructor - initializes at origin (0, 0).
     */
    constexpr Point() : x(0), y(0) {}

    /**
     * Constructor with coordinates.
     * @param[in] x Specifies the x-coordinate.
     * @param[in] y Specifies the y-coordinate.
     */
    constexpr Point(u16 x, u16 y) : x(x), y(y) {}

    /**
     * Equality comparison.
     */
    constexpr bool operator==(const Point& pt) const
    {
        return (x == pt.x) && (y == pt.y);
    }

    /**
     * Inequality comparison.
     */
    constexpr bool operator!=(const Point& pt) const
    {
        return !(*this == pt);
    }
};
//---------------------------------------------------------------------------
#endif

// EOF
