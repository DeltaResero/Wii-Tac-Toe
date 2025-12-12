// source/language.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include <string>
#include <string_view>
#include <vector>
#include <gctypes.h>
#include <random>  // Include for std::mt19937

// Forward declarations
struct _mxml_node_s;

/**
 * This is a class to manage different languages.
 * @author Crayon
 */
class Language
{
public:
    Language();
    Language(Language const&) = delete;
    ~Language();
    Language& operator=(Language const&) = delete;
    std::string String(std::string_view From);
    std::string GetWinningMessage(s32 Index = -1);
    std::string GetTieMessage(s32 Index = -1);
    std::string GetTurnOverMessage(s32 Index = -1);
private:
    _mxml_node_s *First_Node;

    std::vector<std::string> WinningMessage;
    std::vector<std::string> TieMessage;
    std::vector<std::string> TurnOverMessage;

    std::mt19937 rng;  // Random number generator

    void SetLanguage(s32 Conf_Lang);
};
//---------------------------------------------------------------------------
#endif

// EOF
