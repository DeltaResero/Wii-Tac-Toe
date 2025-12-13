// source/grid.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef GridH
#define GridH
//---------------------------------------------------------------------------

#include <gctypes.h>
#include <random>

/**
 * Tic-Tac-Toe grid.
 * @author Crayon
 */
class Grid
{
public:
    Grid();
    Grid(Grid const&) = delete;
    /**
     * Destructor for the Grid class.
     */
    virtual ~Grid() = default;
    Grid& operator=(Grid const&) = delete;
    bool SetPlayer(u8 Player, u8 X, u8 Y);
    void SetPlayerAI(u8 Player);
    [[nodiscard]] u8 GetPlayerAtPos(u8 X, u8 Y) const;
    [[nodiscard]] u8 GetWinner() const;
    void Clear();
    [[nodiscard]] bool IsFilled();
    [[nodiscard]] bool IsWinningPosition(u8 X, u8 Y) const;
private:
    u8 Board[3][3];
    u8 Winner;
    std::mt19937 Generator;
    std::uniform_int_distribution<u8> Distribution;
    bool WinningBoard[3][3]; /**< A board filled with the winning position. */

    [[nodiscard]] bool IsPlayerWinning(u8 Player);
    [[nodiscard]] bool IsPlayerWinning(u8 Player, const u8 MyBoard[3][3]);
};
//---------------------------------------------------------------------------
#endif

// EOF
