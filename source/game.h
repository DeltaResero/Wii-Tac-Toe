// source/game.h
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include <array>
#include <string>
#include <memory>
#include "cursor.h"
#include "player.h"
#include "button.h"
#include "symbol.h"

// Forward declarations
class Grid;
class Language;
class Audio;
struct GRRLIB_Font;

/**
 * This is the main class of this project. This is where the magic happens.
 * @author Crayon
 */
class Game
{
public:
    Game(u16 GameScreenWidth, u16 GameScreenHeight);
    Game(Game const&) = delete;
    ~Game();
    Game& operator=(Game const&) = delete;
    void Paint();
    bool ControllerManager();
private:
    /**
     * Types of game.
     */
    enum class gameMode : u8 {
        VsAI,       /**< Play vs AI. */
        VsHuman1,   /**< Play vs a human with 1 Wii Remote. */
        VsHuman2    /**< Play vs a human with 2 Wii Remotes. */
    };

    /**
     * Types of screen available in the game.
     */
    enum class gameScreen : u8 {
        Start,  /**< Start screen. */
        Game,   /**< Game screen. */
        Home,   /**< Home screen. */
        Menu    /**< Menu screen. */
    };

    // Layout constants
    static constexpr f32 EXIT_BUTTON_HOME_LEFT = 430.0f;
    static constexpr f32 EXIT_BUTTON_HOME_TOP = 20.0f;
    static constexpr u32 EXIT_BUTTON_HOME_TEXT_HEIGHT = 20;

    static constexpr f32 EXIT_BUTTON_MENU_OFFSET = 20.0f;
    static constexpr f32 EXIT_BUTTON_MENU_TOP = 165.0f;

    static constexpr f32 MENU_BUTTON_TOP_FIRST = 92.0f;
    static constexpr f32 MENU_BUTTON_TOP_SECOND = 292.0f;
    static constexpr f32 MENU_BUTTON_TOP_THIRD = 192.0f;

    // Player name rendering offsets
    static constexpr f32 PLAYER_NAME_LEFT = 44.0f;
    static constexpr f32 PLAYER1_NAME_TOP = 48.0f;
    static constexpr f32 PLAYER2_NAME_TOP = 143.0f;
    static constexpr f32 TIE_NAME_TOP = 248.0f;
    static constexpr f32 PLAYER_NAME_WIDTH = 125.0f;
    static constexpr u32 PLAYER_NAME_FONT_SIZE = 15;
    static constexpr s8 PLAYER_NAME_SHADOW_X = -2;
    static constexpr s8 PLAYER_NAME_SHADOW_Y = 2;

    // Player name colors
    static constexpr u32 PLAYER1_NAME_COLOR = 0x6BB6DEFF;
    static constexpr u32 PLAYER2_NAME_COLOR = 0xE6313AFF;
    static constexpr u32 TIE_NAME_COLOR = 0x109642FF;
    static constexpr u32 NAME_TEXT_COLOR = 0xFFFFFFFF;

    // Score display positions
    static constexpr f32 SCORE_CENTER_X = 104.0f;
    static constexpr f32 PLAYER1_SCORE_TOP = 75.0f;
    static constexpr f32 PLAYER2_SCORE_TOP = 175.0f;
    static constexpr f32 TIE_SCORE_TOP = 280.0f;
    static constexpr u32 SCORE_FONT_SIZE = 35;
    static constexpr f32 SCORE_SHADOW_OFFSET = 2.0f;

    // Bottom text display
    static constexpr f32 BOTTOM_TEXT_LEFT = 130.0f;
    static constexpr f32 BOTTOM_TEXT_TOP = 420.0f;
    static constexpr f32 BOTTOM_TEXT_WIDTH = 390.0f;
    static constexpr u32 BOTTOM_TEXT_FONT_SIZE = 15;
    static constexpr u32 BOTTOM_TEXT_COLOR = 0x8C8A8CFF;
    static constexpr u32 BOTTOM_TEXT_SHADOW_COLOR = 0x111111FF;
    static constexpr s8 BOTTOM_TEXT_SHADOW_X = 1;
    static constexpr s8 BOTTOM_TEXT_SHADOW_Y = 1;

    // Game hover circles (for home/menu button areas)
    static constexpr f32 HOME_CIRCLE_X = 65.0f;
    static constexpr f32 HOME_CIRCLE_Y = 409.0f;
    static constexpr f32 MENU_CIRCLE_X = 571.0f;
    static constexpr f32 MENU_CIRCLE_Y = 409.5f;
    static constexpr f32 HOVER_CIRCLE_RADIUS = 40.0f;
    static constexpr f32 HOVER_IMAGE_OFFSET = 52.0f;

    // Home screen layout
    static constexpr f32 HOME_TOP_BAR_HEIGHT = 78.0f;
    static constexpr f32 HOME_SEPARATOR_TOP = 78.0f;
    static constexpr f32 HOME_SEPARATOR_BOTTOM = 383.0f;
    static constexpr f32 HOME_SEPARATOR_HEIGHT = 2.0f;
    static constexpr f32 HOME_BOTTOM_BAR_TOP = 385.0f;
    static constexpr f32 HOME_BOTTOM_BAR_HEIGHT = 95.0f;
    static constexpr f32 HOME_TITLE_LEFT = 30.0f;
    static constexpr f32 HOME_TITLE_TOP = 20.0f;
    static constexpr u32 HOME_TITLE_FONT_SIZE = 30;
    static constexpr u32 HOME_OVERLAY_COLOR = 0x000000CC;
    static constexpr u32 HOME_SEPARATOR_COLOR = 0x848284FF;
    static constexpr u32 HOME_BAR_COLOR = 0x000000FF;
    static constexpr u32 HOME_HIGHLIGHT_COLOR = 0x30B6EBFF;

    // Menu screen layout
    static constexpr f32 MENU_TOP_BAR_HEIGHT = 63.0f;
    static constexpr f32 MENU_SEPARATOR_TOP = 63.0f;
    static constexpr f32 MENU_SEPARATOR_BOTTOM = 383.0f;
    static constexpr f32 MENU_VERSION_LEFT = 500.0f;
    static constexpr f32 MENU_VERSION_TOP = 40.0f;
    static constexpr u32 MENU_VERSION_FONT_SIZE = 12;
    static constexpr u32 MENU_STRIPE_SPACING = 8;
    static constexpr u32 MENU_STRIPE_THICKNESS = 2;
    static constexpr u32 MENU_STRIPE_COLOR = 0xB0B0B030;
    static constexpr u32 MENU_BAR_COLOR = 0x000000FF;
    static constexpr u32 MENU_SEPARATOR_COLOR = 0xFFFFFFFF;

    // Start screen
    static constexpr f32 START_ARM_X = 146.0f;
    static constexpr f32 START_ARM_Y = 62.0f;
    static constexpr f32 ARM_ROTATION_MAX = 40.0f;
    static constexpr f32 ARM_ROTATION_MIN = -15.0f;
    static constexpr f32 ARM_ROTATION_STEP = 0.5f;
    static constexpr f32 ARM_CLIP_X = 158.0f;
    static constexpr f32 ARM_CLIP_Y = 40.0f;
    static constexpr f32 ARM_CLIP_W = 100.0f;
    static constexpr f32 ARM_CLIP_H = 145.0f;
    static constexpr f32 START_TEXT_TOP = 400.0f;
    static constexpr u32 START_TEXT_FONT_SIZE = 20;
    static constexpr u32 START_TEXT_COLOR = 0x000000FF;

    // Programmer credits
    static constexpr f32 CREDITS_LEFT = 50.0f;
    static constexpr f32 CREDITS_PROGRAMMER_TOP = 310.0f;
    static constexpr f32 CREDITS_GRAPHICS_TOP = 330.0f;
    static constexpr u32 CREDITS_FONT_SIZE = 11;
    static constexpr u32 CREDITS_TEXT_COLOR = 0xFFFFFFFF;

    // FPS display
    static constexpr f32 FPS_LEFT = 15.0f;
    static constexpr f32 FPS_TOP = 445.0f;
    static constexpr f32 FPS_SHADOW_X_1 = 14.0f;
    static constexpr f32 FPS_SHADOW_Y_1 = 444.0f;
    static constexpr f32 FPS_SHADOW_X_2 = 16.0f;
    static constexpr f32 FPS_SHADOW_Y_2 = 446.0f;
    static constexpr u32 FPS_FONT_SIZE = 17;
    static constexpr u32 FPS_SHADOW_COLOR_1 = 0x000000FF;
    static constexpr u32 FPS_TEXT_COLOR = 0xFFFFFFFF;
    static constexpr u32 FPS_SHADOW_COLOR_2 = 0x808080FF;

    // Text wrapping
    static constexpr f32 LINE_HEIGHT_MULTIPLIER = 1.2f;

    void StartScreen();
    void MenuScreen(bool CopyScreen);
    void GameScreen(bool CopyScreen);
    void ExitScreen();
    void Clear();
    void TurnIsOver();
    void NewGame();
    void PrintWrapText(u16 x, u16 y, u16 maxLineWidth, const std::string &input,
        u32 fontSize, u32 TextColor, u32 ShadowColor, s8 OffsetX, s8 OffsetY);
    void ChangeScreen(gameScreen NewScreen, bool PlaySound = true);
    void ButtonOn(s8 NewFocusedButton);
    bool SelectZone();
    void ChangeCursor();
    void CalculateFrameRate();

    std::array<Cursor, 4> Hand;
    s8 HandX;
    s8 HandY;

    bool CurrentPlayer;
    bool PlayerToStart;
    std::array<Player, 2> WTTPlayer;
    gameScreen CurrentScreen;
    gameScreen LastScreen;
    s8 FocusedButton;
    std::string text;

    u16 TieGame;
    bool RoundFinished;

    /* Initialize in the same order as in the constructor */
    u8 FPS;
    bool ShowFPS;
    u8 FrameCount{0};
    u32 LastFrameTime{0};

    u16 ScreenWidth;
    u16 ScreenHeight;

    gameMode GameMode;

    u8 SymbolAlpha;
    bool AlphaDirection;

    u8 AIThinkLoop;
    bool Copied;

    // AI timing constants
    static constexpr u8 AI_THINK_MIN_FRAMES = 20;
    static constexpr u8 AI_THINK_VARIANCE = 10;

    // Animation constants
    static constexpr u8 SYMBOL_ALPHA_MIN = 5;
    static constexpr u8 SYMBOL_ALPHA_MAX = 128;
    static constexpr u8 SYMBOL_ALPHA_STEP = 2;

    // Alpha channel constants
    static constexpr u8 ALPHA_FULL = 0xFF;
    static constexpr u8 ALPHA_LOW = 0x55;

    // Controller disconnect overlay alpha
    static constexpr u8 ALPHA_DISCONNECT_OVERLAY = 0xB2;

    // Rumble durations (milliseconds)
    static constexpr int RUMBLE_INVALID_MOVE = 200;
    static constexpr int RUMBLE_BUTTON_HOVER = 50;
    static constexpr int RUMBLE_ZONE_SELECT = 30;

    std::array<std::unique_ptr<Button>, 3> ExitButton;
    std::array<std::unique_ptr<Button>, 3> MenuButton;
    std::unique_ptr<Grid> GameGrid;
    std::unique_ptr<Language> Lang;
    std::array<std::array<Symbol, 3>, 3> GridSign;
    std::unique_ptr<Audio> GameAudio;

    std::unique_ptr<Texture> GameImg; /**< Background texture for the game. */
    std::unique_ptr<Texture> GameHoverImg; /**< Hover texture for the game buttons. */
    std::unique_ptr<Texture> SplashImg; /**< Splash screen texture. */
    std::unique_ptr<Texture> SplashArmImg; /**< Arm texture for splash screen. */
    std::unique_ptr<Texture> HoverImg; /**< Texture to put over a symbol when selected. */
    std::unique_ptr<Texture> CopiedImg; /**< Texture to store a temporary copy of the screen. */
    std::unique_ptr<Texture> GameText; /**< Game text that does not change including background. */

    GRRLIB_Font *DefaultFont;
};
//---------------------------------------------------------------------------
#endif

// EOF
