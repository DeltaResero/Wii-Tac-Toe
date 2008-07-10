//---------------------------------------------------------------------------
#ifndef GameH
#define GameH
//---------------------------------------------------------------------------

#include "grid.h"
#include "button.h"
#include "cursor.h"
#include "player.h"

/**
 * This is the main class of this project. This is where the magic happens.
 * @author Crayon
 */
class Game
{
public:
	Game();
	~Game();
	void Paint();
	bool ControllerManager();
private:
	void StartSreen();
	void MenuScreen();
	void GameScreen();
	void ExitScreen();
	void Clear();
	void TurnIsOver();
	void NewGame();
	void PrintText(u16, u16, const char*, u8 [], u32, u32, int, int, f32);
	void ChangeScreen(u8 NewScreen);
	void ButtonOn(signed char);
	void SelectZone();

	Cursor *Hand;
	signed char HandX, HandY;

	bool CurrentPlayer;
	bool PlayerToStart;
	Player *WTTPlayer;
	u8 CurrentScreen;
	signed char SelectedButton;
	char text[50];

	int button_text_height;
	u8 scrore_height;
	u8 scrore_width;
	
	u16 TieGame;
	bool RoundFinished;

	Button *ExitButton;
	Button *MenuButton;
	Grid *GameGrid;
	
	u8 *GameImg; // PNG Array
	u8 *SplashImg;
	u8 *HoverImg;
	u8 *CopiedImg;
	u8 *TextFont;
};

class Point
{
private:
    u16 x, y;
public:
	Point(u16 x, u16 y)
	{
		this->x = x;
		this->y = y;
	}
	u16 GetX()
	{
		return x;
	}
	u16 GetY()
	{
		return y;
	}
	void setLocation(u16 x, u16 y)
	{
		this->x = x;
		this->y = y;
	}
};
//---------------------------------------------------------------------------
#endif
