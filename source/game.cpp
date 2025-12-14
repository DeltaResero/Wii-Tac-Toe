// source/game.cpp
// SPDX-License-Identifier: MIT
//
// Wii-Tac-Toe
//
// Copyright (C) 2025 Crayon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the MIT License. A copy of the license is
// located in the LICENSE file included with this distribution.

#include <array>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <charconv>
#include <limits>
#include <format>
#include <wiiuse/wpad.h>
#include <ogc/conf.h>
#include <ogc/lwp_watchdog.h>
#include "grrlib.h"
#include "grrlib_class.h"
#include "tools.h"
#include "grid.h"
#include "audio.h"
#include "button.h"
#include "cursor.h"
#include "player.h"
#include "language.h"
#include "types.h"
#include "game.h"

// Graphics
#include "splash.h"
#include "splash_arm.h"
#include "backg.h"
#include "backg_hover.h"
#include "hover.h"

// Font
#include "../fonts/Swis721_Ex_BT.h"

/**
 * Array to hold the position of each zone.
 */
static constexpr Point Table[3][3] = {
    {Point(180, 28), Point(180, 131), Point(180, 233)},
    {Point(322, 28), Point(322, 131), Point(322, 233)},
    {Point(464, 28), Point(464, 131), Point(464, 233)}};

/**
 * Maximum digits + null terminator.
 */
static constexpr size_t MaxScoreLength = std::numeric_limits<u16>::digits10 + 2;

/**
 * Constructor for the Game class.
 * @param[in] GameScreenWidth Screen width.
 * @param[in] GameScreenHeight Screen height.
 */
Game::Game(u16 GameScreenWidth, u16 GameScreenHeight) :
    FPS(0),
    ShowFPS(false),
    FrameCount(0),
    LastFrameTime(0),
    ScreenWidth(GameScreenWidth),
    ScreenHeight(GameScreenHeight),
    GameMode(gameMode::VsHuman1),
    SymbolAlpha(5),
    AlphaDirection(false),
    AIThinkLoop(0),
    Copied(false)
{
    std::srand(std::time(nullptr));  // Initialize random seed

    GameGrid = std::make_unique<Grid>();
    Lang = std::make_unique<Language>();

    DefaultFont = GRRLIB_LoadTTF(Swis721_Ex_BT, Swis721_Ex_BT_size);

    // Initialize GridSigns using the Table positions
    for(u8 x = 0; x < 3; ++x)
    {
        for(u8 y = 0; y < 3; ++y)
        {
            GridSign[x][y].SetLocation(Table[x][y]);
        }
    }

    // Hide hands initially
    for(auto &hand : Hand)
    {
        hand.SetVisible(false);
    }

    // Initialize Exit and Menu buttons
    ExitButton[0] = std::make_unique<Button>(buttonType::Home);
    ExitButton[0]->SetFont(DefaultFont);
    ExitButton[0]->SetLeft(EXIT_BUTTON_HOME_LEFT);
    ExitButton[0]->SetTop(EXIT_BUTTON_HOME_TOP);
    ExitButton[0]->SetTextHeight(EXIT_BUTTON_HOME_TEXT_HEIGHT);
    ExitButton[0]->SetCaption(Lang->String("Close"));

    ExitButton[1] = std::make_unique<Button>(buttonType::HomeMenu);
    ExitButton[1]->SetFont(DefaultFont);
    ExitButton[1]->SetLeft((ScreenWidth / 2.0f) + EXIT_BUTTON_MENU_OFFSET);
    ExitButton[1]->SetTop(EXIT_BUTTON_MENU_TOP);
    ExitButton[1]->SetCaption(Lang->String("Reset"));

    ExitButton[2] = std::make_unique<Button>(buttonType::HomeMenu);
    ExitButton[2]->SetFont(DefaultFont);
    ExitButton[2]->SetLeft((ScreenWidth / 2.0f) - ExitButton[1]->GetWidth() - EXIT_BUTTON_MENU_OFFSET);
    ExitButton[2]->SetTop(EXIT_BUTTON_MENU_TOP);
    ExitButton[2]->SetCaption(Lang->String("Return to Loader"));

    MenuButton[0] = std::make_unique<Button>();
    MenuButton[0]->SetFont(DefaultFont);
    MenuButton[0]->SetLeft((ScreenWidth / 2.0f) - (MenuButton[0]->GetWidth() / 2.0f));
    MenuButton[0]->SetTop(MENU_BUTTON_TOP_FIRST);
    MenuButton[0]->SetCaption(Lang->String("2 Players (1 Wiimote)"));

    MenuButton[1] = std::make_unique<Button>();
    MenuButton[1]->SetFont(DefaultFont);
    MenuButton[1]->SetLeft((ScreenWidth / 2.0f) - (MenuButton[1]->GetWidth() / 2.0f));
    MenuButton[1]->SetTop(MENU_BUTTON_TOP_SECOND);
    MenuButton[1]->SetCaption(Lang->String("1 Player (Vs AI)"));

    MenuButton[2] = std::make_unique<Button>();
    MenuButton[2]->SetFont(DefaultFont);
    MenuButton[2]->SetLeft((ScreenWidth / 2.0f) - (MenuButton[2]->GetWidth() / 2.0f));
    MenuButton[2]->SetTop(MENU_BUTTON_TOP_THIRD);
    MenuButton[2]->SetCaption(Lang->String("2 Players (2 Wiimotes)"));

    // Initialize players
    WTTPlayer[0].SetSign('X');
    WTTPlayer[0].SetName(Lang->String("PLAYER 1"));
    WTTPlayer[1].SetSign('O');
    WTTPlayer[1].SetName(Lang->String("PLAYER 2"));

    // Load textures
    GameImg = Texture::CreateFromPNG(backg);
    GameHoverImg = Texture::CreateFromPNG(backg_hover);
    SplashImg = Texture::CreateFromPNG(splash);
    SplashArmImg = Texture::CreateFromPNG(splash_arm);
    HoverImg = Texture::CreateFromPNG(hover);
    CopiedImg = std::make_unique<Texture>(ScreenWidth, ScreenHeight);
    GameText = std::make_unique<Texture>(ScreenWidth, ScreenHeight);

    // Player name with a shadow offset of -2, 2 (includes game background)
    // GameText should only be modified when player names changed.
    GameImg->Draw(0, 0);
    PrintWrapText(PLAYER_NAME_LEFT, PLAYER1_NAME_TOP, PLAYER_NAME_WIDTH, WTTPlayer[0].GetName(), PLAYER_NAME_FONT_SIZE, NAME_TEXT_COLOR, PLAYER1_NAME_COLOR, PLAYER_NAME_SHADOW_X, PLAYER_NAME_SHADOW_Y);
    PrintWrapText(PLAYER_NAME_LEFT, PLAYER2_NAME_TOP, PLAYER_NAME_WIDTH, WTTPlayer[1].GetName(), PLAYER_NAME_FONT_SIZE, NAME_TEXT_COLOR, PLAYER2_NAME_COLOR, PLAYER_NAME_SHADOW_X, PLAYER_NAME_SHADOW_Y);
    PrintWrapText(PLAYER_NAME_LEFT, TIE_NAME_TOP, PLAYER_NAME_WIDTH, Lang->String("TIE GAME"), PLAYER_NAME_FONT_SIZE, NAME_TEXT_COLOR, TIE_NAME_COLOR, PLAYER_NAME_SHADOW_X, PLAYER_NAME_SHADOW_Y);
    GameText->CopyScreen(0, 0, true);

    // Build Start Screen background
    SplashImg->Draw(0, 0);
    GRRLIB_PrintfTTF(CREDITS_LEFT, CREDITS_PROGRAMMER_TOP, DefaultFont,
        std::format(std::runtime_format(Lang->String("Programmer: {}")), "Crayon").c_str(),
        CREDITS_FONT_SIZE, CREDITS_TEXT_COLOR);
    GRRLIB_PrintfTTF(CREDITS_LEFT, CREDITS_GRAPHICS_TOP, DefaultFont,
        std::format(std::runtime_format(Lang->String("Graphics: {}")), "Mr_Nick666").c_str(),
        CREDITS_FONT_SIZE, CREDITS_TEXT_COLOR);
    text = Lang->String("Press The A Button");
    GRRLIB_PrintfTTF((ScreenWidth / 2) - (GRRLIB_WidthTTF(DefaultFont, text.c_str(), START_TEXT_FONT_SIZE) / 2),
                    START_TEXT_TOP, DefaultFont, text.c_str(), START_TEXT_FONT_SIZE, START_TEXT_COLOR);
    SplashImg->CopyScreen(0, 0, true);

    // Set handle for arm rotation
    SplashArmImg->SetHandle(8, 70);

    // Initialize Audio and Rumble
    GameAudio = std::make_unique<Audio>();
    RUMBLE_Init();

    // Initialize frame timing
    LastFrameTime = ticks_to_millisecs(gettime());

    NewGame();
}

/**
 * Destructor for the Game class.
 */
Game::~Game()
{
    GRRLIB_FreeTTF(DefaultFont);
}

/**
 * Draw the proper screen.
 */
void Game::Paint()
{
    switch(CurrentScreen)
    {
        case gameScreen::Start:
            StartScreen();
            break;
        case gameScreen::Menu:
            MenuScreen(true);
            break;
        case gameScreen::Home:
            ExitScreen();
            break;
        case gameScreen::Game:
            GameScreen(true);
            // AI
            if(!RoundFinished && WTTPlayer[CurrentPlayer].GetType() == playerType::CPU)
            {   // AI
                if(AIThinkLoop > (std::rand() % AI_THINK_VARIANCE + AI_THINK_MIN_FRAMES))
                {
                    GameGrid->SetPlayerAI(WTTPlayer[CurrentPlayer].GetSign());
                    TurnIsOver();
                    AIThinkLoop = 0;
                }
                else
                {
                    ++AIThinkLoop;
                }
            }
            break;
        default:
            GRRLIB_FillScreen(0x000000FF);
    }

    if(CurrentScreen != gameScreen::Start &&
        WPAD_Probe(WPAD_CHAN_0, nullptr) == WPAD_ERR_NO_CONTROLLER)
    {   // Controller is disconnected
        Rectangle(0, 0, ScreenWidth, ScreenHeight, 0x000000 | ALPHA_DISCONNECT_OVERLAY, 1);
    }
    else
    {
        for(auto& hand : Hand)
        {
            hand.Paint();
        }
    }

    if(ShowFPS)
    {
        CalculateFrameRate();
        const auto strFPS = std::format("FPS: {}", FPS);
        GRRLIB_PrintfTTF(FPS_LEFT, FPS_TOP, DefaultFont, strFPS.c_str(), FPS_FONT_SIZE, FPS_SHADOW_COLOR_1);
        GRRLIB_PrintfTTF(FPS_SHADOW_X_1, FPS_SHADOW_Y_1, DefaultFont, strFPS.c_str(), FPS_FONT_SIZE, FPS_TEXT_COLOR);
        GRRLIB_PrintfTTF(FPS_SHADOW_X_2, FPS_SHADOW_Y_2, DefaultFont, strFPS.c_str(), FPS_FONT_SIZE, FPS_SHADOW_COLOR_2);
    }
}

/**
 * Draw the start screen.
 */
void Game::StartScreen()
{
    static f32 ArmRotation = 0;
    static bool ArmDirection = 0;

    SplashImg->Draw(0, 0); // Background

    if(ArmDirection == 0)
    {
        ArmRotation += ARM_ROTATION_STEP;
        if(ArmRotation > ARM_ROTATION_MAX)
        {
            ArmDirection = 1;
        }
    }
    else
    {
        ArmRotation -= ARM_ROTATION_STEP;
        if(ArmRotation < ARM_ROTATION_MIN)
        {
            ArmDirection = 0;
        }
    }
    GRRLIB_ClipDrawing(ARM_CLIP_X, ARM_CLIP_Y, ARM_CLIP_W, ARM_CLIP_H);
    SplashArmImg->Draw(START_ARM_X, START_ARM_Y, ArmRotation); // Arm
    GRRLIB_ClipReset();
}

/**
 * Draw the game screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::GameScreen(bool CopyScreen)
{
    if(Copied == false)
    {   // Copy static element
        GameText->Draw(0, 0); // Background image with some text

        // Function to draw the score with a shadow
        auto DrawScore = [&](int playerIndex, int yPos, u32 color)
        {
            char ScoreText[MaxScoreLength] = {};
            std::to_chars(ScoreText, ScoreText + MaxScoreLength, WTTPlayer[playerIndex].GetScore());
            const auto TextLeft = SCORE_CENTER_X - GRRLIB_WidthTTF(DefaultFont, ScoreText, SCORE_FONT_SIZE) / 2;
            GRRLIB_PrintfTTF(TextLeft, yPos + SCORE_SHADOW_OFFSET, DefaultFont, ScoreText, SCORE_FONT_SIZE, NAME_TEXT_COLOR);
            GRRLIB_PrintfTTF(TextLeft - SCORE_SHADOW_OFFSET, yPos, DefaultFont, ScoreText, SCORE_FONT_SIZE, color);
        };

        DrawScore(0, PLAYER1_SCORE_TOP, PLAYER1_NAME_COLOR); // Player 1
        DrawScore(1, PLAYER2_SCORE_TOP, PLAYER2_NAME_COLOR); // Player 2

        // Draw tie score
        char TieScoreText[MaxScoreLength] = {};
        std::to_chars(TieScoreText, TieScoreText + MaxScoreLength, TieGame);
        const auto TieTextLeft = SCORE_CENTER_X - GRRLIB_WidthTTF(DefaultFont, TieScoreText, SCORE_FONT_SIZE) / 2;
        GRRLIB_PrintfTTF(TieTextLeft, TIE_SCORE_TOP + SCORE_SHADOW_OFFSET, DefaultFont, TieScoreText, SCORE_FONT_SIZE, TIE_NAME_COLOR);
        GRRLIB_PrintfTTF(TieTextLeft - SCORE_SHADOW_OFFSET, TIE_SCORE_TOP, DefaultFont, TieScoreText, SCORE_FONT_SIZE, NAME_TEXT_COLOR);

        // Draw text at the bottom with a shadow offset of 1, 1
        PrintWrapText(BOTTOM_TEXT_LEFT, BOTTOM_TEXT_TOP, BOTTOM_TEXT_WIDTH, text, BOTTOM_TEXT_FONT_SIZE, BOTTOM_TEXT_COLOR, BOTTOM_TEXT_SHADOW_COLOR, BOTTOM_TEXT_SHADOW_X, BOTTOM_TEXT_SHADOW_Y);

        if(CopyScreen)
        {
            CopiedImg->CopyScreen();
            Copied = true;
        }
    }
    if(CopyScreen)
    {
        CopiedImg->Draw(0, 0);
    }

    const u32 HoverColor = (WTTPlayer[CurrentPlayer].GetSign() == 'X') ? 0x0093DDFF : 0xDA251DFF;

    // Draw grid content
    if(RoundFinished)
    {
        SymbolAlpha = (AlphaDirection) ? SymbolAlpha + SYMBOL_ALPHA_STEP : SymbolAlpha - SYMBOL_ALPHA_STEP;
        if(SymbolAlpha > SYMBOL_ALPHA_MAX || SymbolAlpha < SYMBOL_ALPHA_MIN)
        {
            AlphaDirection = !AlphaDirection;
        }
    }

    for(u8 x = 0; x < 3; ++x)
    {
        for(u8 y = 0; y < 3; ++y)
        {
            GridSign[x][y].SetPlayer(GameGrid->GetPlayerAtPos(x, y));
            GridSign[x][y].SetColor(0xFFFFFFFF);
            GridSign[x][y].Paint();
            if(GameGrid->IsWinningPosition(x, y))
            {
                GridSign[x][y].SetColor(HoverColor);
                GridSign[x][y].SetAlpha(SymbolAlpha);
                GridSign[x][y].Paint();
            }
        }
    }

    if(SelectZone())
    {
        // Draw selection box
        if(GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
        {
            HoverImg->Draw(Table[HandX][HandY].x, Table[HandX][HandY].y, 0, 1, 1, HoverColor);
        }
    }
    else
    {
        // 40 = radius, 52 = half of image size
        if(PtInCircle(HOME_CIRCLE_X, HOME_CIRCLE_Y, HOVER_CIRCLE_RADIUS, Hand[0].GetLeft(), Hand[0].GetTop()))
        {
            GameHoverImg->Draw(HOME_CIRCLE_X-HOVER_IMAGE_OFFSET, HOME_CIRCLE_Y-HOVER_IMAGE_OFFSET, 0, 1, 1, 0xFFFFFFFF);
            ButtonOn(0);
            FocusedButton = 0;
        }
        else if(PtInCircle(MENU_CIRCLE_X, MENU_CIRCLE_Y, HOVER_CIRCLE_RADIUS, Hand[0].GetLeft(), Hand[0].GetTop()))
        {
            GameHoverImg->Draw(MENU_CIRCLE_X-HOVER_IMAGE_OFFSET, MENU_CIRCLE_Y-HOVER_IMAGE_OFFSET, 0, 1, 1, 0xFFFFFFFF);
            ButtonOn(1);
            FocusedButton = 1;
        }
        else
        {
            FocusedButton = -1;
        }
    }
}

/**
 * Draw the HOME screen.
 */
void Game::ExitScreen()
{
    GameAudio->PauseMusic(true);
    if(Copied == false)
    {   // Copy static element
        switch(LastScreen)
        {
            case gameScreen::Game:
                GameScreen(false);
                break;
            case gameScreen::Menu:
                MenuScreen(false);
                break;
            default:
                break;
        }

        Rectangle(0, 0, ScreenWidth, ScreenHeight, HOME_OVERLAY_COLOR, 1); // Draw a black rectangle over it
        Rectangle(0, HOME_SEPARATOR_TOP, ScreenWidth, HOME_SEPARATOR_HEIGHT, HOME_SEPARATOR_COLOR, 1);
        Rectangle(0, HOME_SEPARATOR_BOTTOM, ScreenWidth, HOME_SEPARATOR_HEIGHT, HOME_SEPARATOR_COLOR, 1);
        Rectangle(0, HOME_BOTTOM_BAR_TOP, ScreenWidth, HOME_BOTTOM_BAR_HEIGHT, HOME_BAR_COLOR, 1);

        CopiedImg->CopyScreen();
        Copied = true;
    }
    else
    {
        CopiedImg->Draw(0, 0);
    }

    if(GRRLIB_PtInRect(0, 0, ScreenWidth, HOME_TOP_BAR_HEIGHT, Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        Rectangle(0, 0, ScreenWidth, HOME_TOP_BAR_HEIGHT, HOME_HIGHLIGHT_COLOR, 1);
    }
    else
    {
        Rectangle(0, 0, ScreenWidth, HOME_TOP_BAR_HEIGHT, HOME_BAR_COLOR, 1);
    }

    GRRLIB_PrintfTTF(HOME_TITLE_LEFT, HOME_TITLE_TOP, DefaultFont, Lang->String("HOME Menu").c_str(), HOME_TITLE_FONT_SIZE, 0xFFFFFFFF);

    ExitButton[0]->SetFocused(false);
    ExitButton[1]->SetFocused(false);
    ExitButton[2]->SetFocused(false);
    if(GRRLIB_PtInRect(0, 0, ScreenWidth, HOME_TOP_BAR_HEIGHT, Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton[0]->SetFocused(true);
        ButtonOn(0);
        FocusedButton = 0;
    }
    else if(ExitButton[1]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton[1]->SetFocused(true);
        ButtonOn(1);
        FocusedButton = 1;
    }
    else if(ExitButton[2]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
    {
        ExitButton[2]->SetFocused(true);
        ButtonOn(2);
        FocusedButton = 2;
    }
    else
    {
        FocusedButton = -1;
    }

    ExitButton[0]->Paint();
    ExitButton[1]->Paint();
    ExitButton[2]->Paint();
}

/**
 * Draw the menu screen.
 * @param[in] CopyScreen If true the screen is copied to a buffer.
 */
void Game::MenuScreen(bool CopyScreen)
{
    if(!Copied)
    {   // Copy static element
        FillScreen(0x000000FF); // Clear screen
        for(int y = 0; y <= ScreenHeight; y += MENU_STRIPE_SPACING)
        {
            Rectangle(0, y, ScreenWidth, MENU_STRIPE_THICKNESS, MENU_STRIPE_COLOR, 1);
        }

        Rectangle(0, 0, ScreenWidth, MENU_TOP_BAR_HEIGHT, MENU_BAR_COLOR, 1);
        Rectangle(0, MENU_SEPARATOR_TOP, ScreenWidth, MENU_STRIPE_THICKNESS, MENU_SEPARATOR_COLOR, 1);
        Rectangle(0, MENU_SEPARATOR_BOTTOM, ScreenWidth, MENU_STRIPE_THICKNESS, MENU_SEPARATOR_COLOR, 1);
        Rectangle(0, HOME_BOTTOM_BAR_TOP, ScreenWidth, HOME_BOTTOM_BAR_HEIGHT, MENU_BAR_COLOR, 1);

        GRRLIB_PrintfTTF(MENU_VERSION_LEFT, MENU_VERSION_TOP, DefaultFont,
            std::format(std::runtime_format(Lang->String("Ver. {}")), "1.1.0").c_str(),
            MENU_VERSION_FONT_SIZE, 0xFFFFFFFF);

        if(CopyScreen)
        {
            CopiedImg->CopyScreen();
            Copied = true;
        }
    }
    else
    {
        CopiedImg->Draw(0, 0);
    }

    for(int i = 0; i < 3; ++i)
    {
        MenuButton[i]->SetFocused(false);
    }

    for(int i = 0; i < 3; ++i)
    {
        if(MenuButton[i]->IsInside(Hand[0].GetLeft(), Hand[0].GetTop()))
        {
            MenuButton[i]->SetFocused(true);
            ButtonOn(i);
            FocusedButton = i;
            break;
        }
    }

    if(FocusedButton == -1)
    {
        FocusedButton = -1;
    }

    for(int i = 0; i < 3; ++i)
    {
        MenuButton[i]->Paint();
    }
}

/**
 * Controls all inputs.
 * @return True to exit to loader, false otherwise.
 */
bool Game::ControllerManager()
{
    RUMBLE_Verify();
    WPADData *WPadData[4] = {
        WPAD_Data(WPAD_CHAN_0),
        WPAD_Data(WPAD_CHAN_1),
        WPAD_Data(WPAD_CHAN_2),
        WPAD_Data(WPAD_CHAN_3)
    };

    const u32 Buttons[4] = {
        WPadData[0]->btns_d,
        WPadData[1]->btns_d,
        WPadData[2]->btns_d,
        WPadData[3]->btns_d
    };

    for(int i = 0; i < 4; ++i)
    {
        if(WPadData[i]->ir.valid)
        {
            Hand[i].SetLeft((WPadData[i]->ir.x / ScreenWidth * (ScreenWidth + Hand[i].GetWidth() * 2)) - Hand[i].GetWidth());
            Hand[i].SetTop((WPadData[i]->ir.y / ScreenHeight * (ScreenHeight + Hand[i].GetHeight() * 2)) - Hand[i].GetHeight());
            Hand[i].SetAngle(WPadData[i]->orient.roll);
            Hand[i].SetVisible(true);
        }
        else
        {
            Hand[i].SetVisible(false);
        }
    }

    if(Buttons[0] || Buttons[1])
    {
        switch(CurrentScreen)
        {
            case gameScreen::Start:
                if(Buttons[0] & WPAD_BUTTON_A)
                {
                    ChangeScreen(gameScreen::Menu);
                }
                break;
            case gameScreen::Menu:
                if(Buttons[0] & WPAD_BUTTON_A)
                {
                    switch(FocusedButton)
                    {
                        case 0:
                            WTTPlayer[1].SetType(playerType::Human);
                            GameMode = gameMode::VsHuman1;
                            ChangeScreen(gameScreen::Game);
                            break;
                        case 1:
                            WTTPlayer[1].SetType(playerType::CPU);
                            GameMode = gameMode::VsAI;
                            ChangeScreen(gameScreen::Game);
                            break;
                        case 2:
                            WTTPlayer[1].SetType(playerType::Human);
                            GameMode = gameMode::VsHuman2;
                            ChangeScreen(gameScreen::Game);
                            break;
                    }
                }
                else if(Buttons[0] & WPAD_BUTTON_B)
                {
                    ChangeScreen(gameScreen::Start);
                }
                else if(Buttons[0] & WPAD_BUTTON_HOME || Buttons[1] & WPAD_BUTTON_HOME)
                {
                    ChangeScreen(gameScreen::Home);
                }
                break;
            case gameScreen::Home:
                if(Buttons[0] & WPAD_BUTTON_HOME || Buttons[1] & WPAD_BUTTON_HOME)
                {
                    GameAudio->PauseMusic(false);
                    ChangeScreen(LastScreen);
                }
                else if(Buttons[0] & WPAD_BUTTON_A)
                {
                    switch(FocusedButton)
                    {
                        case 0:
                            GameAudio->PauseMusic(false);
                            ChangeScreen(LastScreen);
                            break;
                        case 1:
                            NewGame();
                            break;
                        case 2:
                            ExitScreen();
                            Hand[3].Paint();
                            Hand[2].Paint();
                            Hand[1].Paint();
                            Hand[0].Paint();
                            CopiedImg->CopyScreen();
                            WPAD_Rumble(WPAD_CHAN_ALL, 0); // Rumble off, just in case
                            Draw_FadeOut(CopiedImg.get(), 1, 1, 3);
                            return true; // Exit to loader
                    }
                }
                break;
            default:
                if(Buttons[0] & WPAD_BUTTON_HOME || Buttons[1] & WPAD_BUTTON_HOME)
                {
                    ChangeScreen(gameScreen::Home);
                }

                if(Buttons[0] & WPAD_BUTTON_A)
                {
                    if(FocusedButton > -1)
                    {
                        ChangeScreen(gameScreen::Home);
                    }
                    else if(RoundFinished)
                    {
                        Clear();
                    }
                    else if(GameMode == gameMode::VsHuman2 && CurrentPlayer == 0)
                    {
                        if(GameGrid->SetPlayer(WTTPlayer[0].GetSign(), HandX, HandY))
                        {
                            TurnIsOver();
                        }
                        else
                        {   // Position is invalid
                            RUMBLE_Wiimote(WPAD_CHAN_0, RUMBLE_INVALID_MOVE);
                        }
                    }
                    else if(GameGrid->SetPlayer(WTTPlayer[CurrentPlayer].GetSign(), HandX, HandY))
                    {
                        TurnIsOver();
                    }
                    else
                    {
                        RUMBLE_Wiimote(WPAD_CHAN_0, RUMBLE_INVALID_MOVE);
                    }
                }

                if(Buttons[1] & WPAD_BUTTON_A && GameMode == gameMode::VsHuman2 && !RoundFinished)
                {
                    if(GameGrid->SetPlayer(WTTPlayer[1].GetSign(), HandX, HandY))
                    {
                        TurnIsOver();
                    }
                    else
                    {   // Position is invalid
                        RUMBLE_Wiimote(WPAD_CHAN_1, RUMBLE_INVALID_MOVE);
                    }
                }
                break;
        }
    }

    if((WPadData[0]->btns_h & WPAD_BUTTON_1 && WPadData[0]->btns_h & WPAD_BUTTON_2) ||
       (WPadData[1]->btns_h & WPAD_BUTTON_1 && WPadData[1]->btns_h & WPAD_BUTTON_2) ||
       (WPadData[2]->btns_h & WPAD_BUTTON_1 && WPadData[2]->btns_h & WPAD_BUTTON_2) ||
       (WPadData[3]->btns_h & WPAD_BUTTON_1 && WPadData[3]->btns_h & WPAD_BUTTON_2))
    {
        WPAD_Rumble(WPAD_CHAN_ALL, 1); // Rumble on
        WIILIGHT_TurnOn();

        const auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
        const auto path = std::format("sd:/Screenshot {:%F %H%M%S}.png", now);

        text = (ScreenShot(path)) ? "A screenshot was taken!!!" : "Screenshot did not work!!!";

        WIILIGHT_TurnOff();
        WPAD_Rumble(WPAD_CHAN_ALL, 0); // Rumble off
        Copied = false;
    }

    if(Buttons[0] & WPAD_BUTTON_PLUS || Buttons[1] & WPAD_BUTTON_PLUS ||
       Buttons[2] & WPAD_BUTTON_PLUS || Buttons[3] & WPAD_BUTTON_PLUS)
    {
        ShowFPS = !ShowFPS;
    }

    return false;
}

/**
 * Start a new round.
 */
void Game::Clear()
{
    GameGrid->Clear();
    CurrentPlayer = PlayerToStart;
    PlayerToStart = !PlayerToStart; // Next other player will start
    text = std::format(std::runtime_format(Lang->GetTurnOverMessage()), WTTPlayer[CurrentPlayer].GetName());
    RoundFinished = false;
    Copied = false;
    ChangeCursor();
}

/**
 * Things to do when a turn is over.
 */
void Game::TurnIsOver()
{
    u8 GameWinner = GameGrid->GetWinner();
    if(GameWinner != ' ')
    {   // A winner is declare
        GameWinner = (GameWinner == WTTPlayer[0].GetSign()) ? 0 : 1;
        WTTPlayer[GameWinner].IncScore();
        text = std::format(std::runtime_format(Lang->GetWinningMessage()),
            WTTPlayer[GameWinner].GetName(), WTTPlayer[!GameWinner].GetName());
        RoundFinished = true;
        SymbolAlpha = SYMBOL_ALPHA_MIN;
        AlphaDirection = false;
    }
    else if(GameGrid->IsFilled() == true)
    {   // Tie game
        ++TieGame;
        text = Lang->GetTieMessage();
        RoundFinished = true;
    }
    else
    {
        CurrentPlayer = !CurrentPlayer; // Change player's turn
        text = std::format(std::runtime_format(Lang->GetTurnOverMessage()), WTTPlayer[CurrentPlayer].GetName());
    }

    Copied = false;
    ChangeCursor();
}

/**
 * Start a new game, initialize variables.
 */
void Game::NewGame()
{
    GameAudio->LoadMusic();

    PlayerToStart = rand() & 1; // 0 or 1

    ChangeScreen(gameScreen::Start, false);

    WTTPlayer[0].ResetScore();
    WTTPlayer[1].ResetScore();
    TieGame = 0;

    AIThinkLoop = 0;

    Clear();
}

/**
 * Print the text with multiple lines if needed.
 * @param[in] x Specifies the x-coordinate of the upper-left corner of the text.
 * @param[in] y Specifies the y-coordinate of the upper-left corner of the text.
 * @param[in] maxLineWidth Maximum width of the string.
 * @param[in] input Text to draw.
 * @param[in] fontSize Size of the text.
 * @param[in] TextColor Text color in RGBA format. The alpha channel is used to change the opacity of the text.
 * @param[in] ShadowColor Shadow color in RGBA format. The alpha channel is used to change the opacity of the text.
 * @param[in] OffsetX Shadow offset for the x-coordinate.
 * @param[in] OffsetY Shadow offset for the y-coordinate.
 */
void Game::PrintWrapText(u16 x, u16 y, u16 maxLineWidth,
    std::string_view input, u32 fontSize, u32 TextColor,
    u32 ShadowColor, s8 OffsetX, s8 OffsetY)
{
    std::string tmp(input);
    tmp += " "; // Make local copy
    auto startIndex = tmp.begin();
    auto lastSpace = tmp.begin();
    int ypos = y;
    int z = 0;
    const int stepSize = static_cast<int>(fontSize * LINE_HEIGHT_MULTIPLIER);

    for(auto i = tmp.begin(); i != tmp.end(); ++i)
    {
        if(*i == ' ')
        {
            const std::string tmp2(startIndex, i);
            z = GRRLIB_WidthTTF(DefaultFont, tmp2.c_str(), fontSize);

            if(z >= maxLineWidth)
            {
                *lastSpace = 0;
                const int textLeft = x + (maxLineWidth / 2) -
                    (GRRLIB_WidthTTF(DefaultFont, &(*startIndex), fontSize) / 2);
                GRRLIB_PrintfTTF(textLeft + OffsetX, ypos + OffsetY, DefaultFont, &(*startIndex),
                    fontSize, ShadowColor);
                GRRLIB_PrintfTTF(textLeft, ypos, DefaultFont, &(*startIndex),
                    fontSize, TextColor);
                startIndex = lastSpace + 1;
                ypos += stepSize;
                z = 0;
            }
            lastSpace = i;
        }
    }

    if(z <= maxLineWidth)
    {
        const int textLeft = x + (maxLineWidth / 2) -
            (GRRLIB_WidthTTF(DefaultFont, &(*startIndex), fontSize) / 2);
        GRRLIB_PrintfTTF(textLeft + OffsetX, ypos + OffsetY, DefaultFont, &(*startIndex),
            fontSize, ShadowColor);
        GRRLIB_PrintfTTF(textLeft, ypos, DefaultFont, &(*startIndex),
            fontSize, TextColor);
    }
}

/**
 * Change the screen.
 * @param[in] NewScreen New screen to show.
 * @param[in] PlaySound Set to true to play a sound. Default value is true.
 */
void Game::ChangeScreen(gameScreen NewScreen, bool PlaySound)
{
    if(PlaySound)
    {
        GameAudio->PlaySoundScreenChange(100);
    }

    FocusedButton = -1;
    LastScreen = CurrentScreen;
    CurrentScreen = NewScreen;

    Copied = false;
    ChangeCursor();
}

/**
 * Rumble if on button.
 * @param[in] NewFocusedButton New button to select.
 */
void Game::ButtonOn(s8 NewFocusedButton)
{
    if(FocusedButton != NewFocusedButton)
    {
        GameAudio->PlaySoundButton(80);
        RUMBLE_Wiimote(WPAD_CHAN_0, RUMBLE_BUTTON_HOVER);
    }
}

/**
 * Check if a zone is selected.
 * @return true if in a zone, false otherwise.
 */
bool Game::SelectZone()
{
    u8 HandID = (GameMode == gameMode::VsHuman2 && CurrentPlayer == 1) ? 1 : 0;

    if(!RoundFinished && AIThinkLoop == 0)
    {
        for(s8 x = 0; x < 3; ++x)
        {
            for(s8 y = 0; y < 3; ++y)
            {
                const auto &cell = Table[x][y];
                if (Hand[HandID].GetLeft() > cell.x &&
                    Hand[HandID].GetLeft() < (cell.x + 136) &&
                    Hand[HandID].GetTop() > cell.y &&
                    Hand[HandID].GetTop() < (cell.y + 100))
                {
                    if(HandX != x || HandY != y)
                    {
                        HandX = x;
                        HandY = y;
                        if(GameGrid->GetPlayerAtPos(HandX, HandY) == ' ')
                        {   // Zone is empty
                            GameAudio->PlaySoundButton(90);
                            RUMBLE_Wiimote(HandID, RUMBLE_ZONE_SELECT);
                        }
                    }
                    return true;
                }
            }
        }
    }
    HandX = -1;
    HandY = -1;
    return false;
}

/**
 * Change the cursor.
 */
void Game::ChangeCursor()
{
    constexpr u8 fullAlpha = ALPHA_FULL;
    constexpr u8 lowAlpha = ALPHA_LOW;

    if(CurrentScreen == gameScreen::Home)
    {
        Hand[0].SetPlayer(cursorType::P1);
        Hand[0].SetAlpha(fullAlpha);
        Hand[1].SetPlayer(cursorType::P2);
        Hand[1].SetAlpha(lowAlpha);
        Hand[2].SetPlayer(cursorType::P3);
        Hand[2].SetAlpha(lowAlpha);
        Hand[3].SetPlayer(cursorType::P4);
        Hand[3].SetAlpha(lowAlpha);
    }
    else if(CurrentScreen == gameScreen::Game)
    {
        if(GameMode == gameMode::VsHuman1)
        {
            Hand[0].SetPlayer((WTTPlayer[CurrentPlayer].GetSign() == 'O') ? cursorType::O : cursorType::X);
            Hand[0].SetAlpha(fullAlpha);
        }
        else if(GameMode == gameMode::VsHuman2)
        {
            Hand[0].SetPlayer(cursorType::X);
            Hand[1].SetPlayer(cursorType::O);
            if (RoundFinished)
            {
                Hand[0].SetAlpha(fullAlpha);
                Hand[1].SetAlpha(fullAlpha);
            }
            else
            {
                Hand[CurrentPlayer].SetAlpha(fullAlpha);
                Hand[!CurrentPlayer].SetAlpha(lowAlpha);
            }
        }
        else
        {   // gameMode::VsAI
            Hand[0].SetPlayer(cursorType::X);
            Hand[0].SetAlpha((CurrentPlayer == 0 || RoundFinished) ? fullAlpha : lowAlpha);
            Hand[1].SetAlpha((CurrentPlayer == 1 && !RoundFinished) ? fullAlpha : lowAlpha);
        }
    }
    else
    {   // START_SCREEN or MENU_SCREEN
        Hand[0].SetPlayer(cursorType::X);
        Hand[0].SetAlpha(fullAlpha);
        Hand[1].SetPlayer(cursorType::O);
        Hand[1].SetAlpha(lowAlpha);
    }
}

/**
 * This function calculates the number of frames we render each second.
 */
void Game::CalculateFrameRate()
{
    ++FrameCount;
    const u32 currentTime = ticks_to_millisecs(gettime());
    if (currentTime - LastFrameTime > 1000)
    {
        LastFrameTime = currentTime;
        FPS = FrameCount;
        FrameCount = 0;
    }
}

// EOF
