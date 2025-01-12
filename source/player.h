/**
 * @file player.h
 * Contains the Player class.
 */

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
    Player();
    Player(Player const&) = delete;
    ~Player() = default;
    Player& operator=(Player const&) = delete;

    void SetName(const std::string &AName);
    [[nodiscard]] std::string GetName() const;

    void SetSign(u8 ASign);
    [[nodiscard]] u8 GetSign() const;

    [[nodiscard]] unsigned int GetScore() const;

    void IncScore();
    void ResetScore();

    void SetType(playerType AType);
    [[nodiscard]] playerType GetType() const;

private:
    unsigned int Score;
    std::string Name;
    u8 Sign;
    playerType Type;
};
//---------------------------------------------------------------------------
#endif
