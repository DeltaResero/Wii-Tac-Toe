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
#include <array>
#include <utility>

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
    // Win condition patterns: 8 winning lines, each with 3 positions [x][y]
    static constexpr std::array<std::array<std::pair<u8, u8>, 3>, 8> WinPatterns = {{
        // Rows
        {{{0,0}, {1,0}, {2,0}}},  // Top row
        {{{0,1}, {1,1}, {2,1}}},  // Middle row
        {{{0,2}, {1,2}, {2,2}}},  // Bottom row
        // Columns
        {{{0,0}, {0,1}, {0,2}}},  // Left column
        {{{1,0}, {1,1}, {1,2}}},  // Middle column
        {{{2,0}, {2,1}, {2,2}}},  // Right column
        // Diagonals
        {{{0,0}, {1,1}, {2,2}}},  // Top-left to bottom-right
        {{{2,0}, {1,1}, {0,2}}}   // Top-right to bottom-left
    }};

    std::array<std::array<u8, 3>, 3> Board;
    u8 Winner;
    std::mt19937 Generator;
    std::uniform_int_distribution<u8> Distribution;
    std::array<std::array<bool, 3>, 3> WinningBoard; /**< A board filled with the winning position. */

    [[nodiscard]] bool IsPlayerWinning(u8 Player);
    [[nodiscard]] bool IsPlayerWinning(u8 Player, const std::array<std::array<u8, 3>, 3>& MyBoard);
};
//---------------------------------------------------------------------------
#endif

// EOF
