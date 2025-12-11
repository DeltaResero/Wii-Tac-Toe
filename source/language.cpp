#include <mxml.h>
#include <ogc/conf.h>
#include <random>  // Add for random number generator
#include "language.h"

// Languages
constexpr char english_xml[] = {
    #embed "../languages/english.xml"
};
constexpr char french_xml[] = {
    #embed "../languages/french.xml"
};
constexpr char german_xml[] = {
    #embed "../languages/german.xml"
};
constexpr char dutch_xml[] = {
    #embed "../languages/dutch.xml"
};
constexpr char spanish_xml[] = {
    #embed "../languages/spanish.xml"
};
constexpr char italian_xml[] = {
    #embed "../languages/italian.xml"
};
constexpr char japanese_xml[] = {
    #embed "../languages/japanese.xml"
};

/**
 * Constructor for the Language class.
 */
Language::Language() :
    rng(std::random_device{}())  // Initialize random number generator once
{
    SetLanguage(CONF_GetLanguage());

    mxml_node_t *Message_Node;
    mxml_node_t *Up_Node;

    Up_Node = mxmlFindElement(First_Node, First_Node, "tie_game", nullptr, nullptr, MXML_DESCEND);
    for(Message_Node = mxmlFindElement(Up_Node, Up_Node, "message", nullptr, nullptr, MXML_DESCEND);
        Message_Node != nullptr;
        Message_Node = mxmlFindElement(Message_Node, Up_Node, "message", nullptr, nullptr, MXML_DESCEND))
    {
        TieMessage.push_back(mxmlElementGetAttr(Message_Node, "text"));
    }

    Up_Node = mxmlFindElement(First_Node, First_Node, "winning_game", nullptr, nullptr, MXML_DESCEND);
    for(Message_Node = mxmlFindElement(Up_Node, Up_Node, "message", nullptr, nullptr, MXML_DESCEND);
        Message_Node != nullptr;
        Message_Node = mxmlFindElement(Message_Node, Up_Node, "message", nullptr, nullptr, MXML_DESCEND))
    {
        WinningMessage.push_back(mxmlElementGetAttr(Message_Node, "text"));
    }

    Up_Node = mxmlFindElement(First_Node, First_Node, "turn_over", nullptr, nullptr, MXML_DESCEND);
    for(Message_Node = mxmlFindElement(Up_Node, Up_Node, "message", nullptr, nullptr, MXML_DESCEND);
        Message_Node != nullptr;
        Message_Node = mxmlFindElement(Message_Node, Up_Node, "message", nullptr, nullptr, MXML_DESCEND))
    {
        TurnOverMessage.push_back(mxmlElementGetAttr(Message_Node, "text"));
    }
}

/**
 * Destructor for the Language class.
 */
Language::~Language()
{
    if(First_Node != nullptr)
    {
        mxmlDelete(First_Node);
    }
}

/**
 * Load a text from an XML file in memory.
 * @param[in] From Original string to translate.
 * @return Translated string.
 */
std::string Language::String(std::string_view From)
{
    if(From.empty() == true)
    {
        return "";
    }

    mxml_node_t *Text_Node = mxmlFindElement(First_Node, First_Node, "translation", "from", From.data(), MXML_DESCEND);
    if(Text_Node == nullptr)
    {
        return "";
    }

    return mxmlElementGetAttr(Text_Node, "to");
}

/**
 * Set the proper language.
 * @param[in] Conf_Lang Language ID to set.
 */
void Language::SetLanguage(s32 Conf_Lang)
{
    mxml_node_t *Root_Node;
    switch(Conf_Lang)
    {
        case CONF_LANG_FRENCH:
            Root_Node = mxmlLoadString(nullptr, french_xml, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_GERMAN:
            Root_Node = mxmlLoadString(nullptr, german_xml, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_DUTCH:
            Root_Node = mxmlLoadString(nullptr, dutch_xml, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_SPANISH:
            Root_Node = mxmlLoadString(nullptr, spanish_xml, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_ITALIAN:
            Root_Node = mxmlLoadString(nullptr, italian_xml, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_JAPANESE:
#ifdef DEBUG
            Root_Node = mxmlLoadString(nullptr, japanese_xml, MXML_TEXT_CALLBACK);
            break;
#endif
        case CONF_LANG_KOREAN:
        case CONF_LANG_SIMP_CHINESE:
        case CONF_LANG_TRAD_CHINESE:
        default:    // CONF_LANG_ENGLISH
            Root_Node = mxmlLoadString(nullptr, english_xml, MXML_TEXT_CALLBACK);
            break;
    }
    First_Node = mxmlFindElement(Root_Node, Root_Node, "language", nullptr, nullptr, MXML_DESCEND);
}

/**
 * Get a winning message.
 * @param[in] Index The index of the message to get.
 *            If the value is under 0, a random message will be returned.
 * @return A winning message.
 */
std::string Language::GetWinningMessage(s32 Index)
{
    const s32 WinningCount = WinningMessage.size();
    if(Index < 0)
    {
        std::uniform_int_distribution<s32> dist(0, WinningCount - 1);
        Index = dist(rng);  // Use class-wide RNG
    }
    else if(Index >= WinningCount)
    {
        return "";
    }
    return WinningMessage[Index];
}

/**
 * Get a tie message.
 * @param[in] Index The index of the message to get.
 *            If the value is under 0, a random message will be returned.
 * @return A tie message.
 */
std::string Language::GetTieMessage(s32 Index)
{
    const s32 TieCount = TieMessage.size();
    if(Index < 0)
    {
        std::uniform_int_distribution<s32> dist(0, TieCount - 1);
        Index = dist(rng);  // Use class-wide RNG
    }
    else if(Index >= TieCount)
    {
        return "";
    }
    return TieMessage[Index];
}

/**
 * Get a turn over message.
 * @param[in] Index The index of the message to get.
 *            If the value is under 0, a random message will be returned.
 * @return A turn over message.
 */
std::string Language::GetTurnOverMessage(s32 Index)
{
    const s32 TurnOverCount = TurnOverMessage.size();
    if(Index < 0)
    {
        std::uniform_int_distribution<s32> dist(0, TurnOverCount - 1);
        Index = dist(rng);  // Use class-wide RNG
    }
    else if(Index >= TurnOverCount)
    {
        return "";
    }
    return TurnOverMessage[Index];
}
