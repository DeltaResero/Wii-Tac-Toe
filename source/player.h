// source/player.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef PlayerH
#define PlayerH
//---------------------------------------------------------------------------

#include <gctypes.h>
#include <string>

/**
 * Types of player.
 */
enum class playerType : u8 {
    Human, /**< Identify a human player. */
    CPU    /**< Identify a computer player. */
};

/**
 * This class is used to managed players.
 * @author Crayon
 */
class Player
{
public:
    /**
     * Constructor for the Player class.
     */ 
    Player() = default;
    Player(Player const&) = delete;
    ~Player() = default;
    Player& operator=(Player const&) = delete;

    void SetName(std::string_view AName);
    [[nodiscard]] std::string GetName() const;

    void SetSign(u8 ASign);
    [[nodiscard]] u8 GetSign() const;

    [[nodiscard]] u16 GetScore() const;

    void IncScore();
    void ResetScore();

    void SetType(playerType AType);
    [[nodiscard]] playerType GetType() const;

private:
    u16 Score{0};
    std::string Name{};
    u8 Sign{0};
    playerType Type{playerType::Human};
};
//---------------------------------------------------------------------------
#endif

// EOF
