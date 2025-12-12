// source/voice.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef VOICE_H_
#define VOICE_H_

#include <gctypes.h>
#include "sound.h"

// Forward declarations
struct aesndpb_t;

/**
 * This is a class used for voice.
 * @author Crayon
 */
class Voice
{
private:
    aesndpb_t *_Voice;
public:
    Voice();
    Voice(Voice const&) = delete;
    virtual ~Voice();
    Voice& operator=(Voice const&) = delete;

    void SetVolume(u16 Volume);
    void SetVolume(u16 LeftVolume, u16 RightVolume);
    void Play(const Sound& sound, u32 delay = 0, bool looped = false);
    void Mute(bool mute);
};

#endif /* VOICE_H_ */

// EOF
