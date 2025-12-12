// source/audio.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef AudioH
#define AudioH
//---------------------------------------------------------------------------
#include <optional>
#include "voice.h"

/**
 * This is a class used for the game audio.
 * @author Crayon
 */
class Audio
{
public:
    Audio();
    Audio(Audio const&) = delete;
    ~Audio();
    Audio& operator=(Audio const&) = delete;

    void PauseMusic(bool Paused);
    void LoadMusic(s16 Volume = 255);
    void PlaySoundScreenChange(u16 Volume);
    void PlaySoundButton(u16 Volume);
private:
    bool Paused;
    std::optional<Voice> ScreenVoice;
    std::optional<Voice> ButtonVoice;
};
//---------------------------------------------------------------------------
#endif

// EOF
