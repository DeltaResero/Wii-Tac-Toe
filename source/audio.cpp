#include <aesndlib.h>
#include <grrmod.h>
#include "voice.h"
#include "sound.h"
#include "audio.h"

// Audio files using modern C++23 #embed
constexpr u8 button_rollover_raw[] = {
    #embed "../audio/button_rollover.raw"
};
constexpr u8 screen_change_raw[] = {
    #embed "../audio/screen_change.raw"
};
constexpr char tic_tac_it[] = {
    #embed "../audio/tic_tac.it"
};

static constexpr Sound ChangeSound(VOICE_MONO16, std::span{screen_change_raw, sizeof(screen_change_raw)}, 44100.0f);
static constexpr Sound RollOverSound(VOICE_MONO16, std::span{button_rollover_raw, sizeof(button_rollover_raw)}, 44100.0f);

/**
 * Constructor for the Audio class.
 */
Audio::Audio() :
    Paused(false)
{
    AESND_Init();
    AESND_Pause(false);

    GRRMOD_Init(true);
    GRRMOD_SetMOD(tic_tac_it, sizeof(tic_tac_it)); // Using your .it file

    ScreenVoice = new Voice();
    ButtonVoice = new Voice();
}

/**
 * Destructor for the Audio class.
 */
Audio::~Audio()
{
    GRRMOD_Unload();
    GRRMOD_End();
    AESND_Pause(true);

    delete ScreenVoice;
    delete ButtonVoice;
}

/**
 * Pause/unpause the music volume.
 * @param[in] Paused On or off.
 */
void Audio::PauseMusic(bool Paused)
{
    if(this->Paused != Paused)
    {
        this->Paused = Paused;
        GRRMOD_Pause();
    }
}

/**
 * Load the music.
 * @param[in] Volume The music volume.
 */
void Audio::LoadMusic(s16 Volume)
{
    GRRMOD_Stop();
    GRRMOD_Start();
    GRRMOD_SetVolume(Volume, Volume); // Maximum volume is 255
    Paused = false;
}

/**
 * Play the screen change sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundScreenChange(u16 Volume)
{
    ScreenVoice->SetVolume(Volume);
    ScreenVoice->Play(ChangeSound);
}

/**
 * Play the button sound FX.
 * @param[in] Volume The sound volume.
 */
void Audio::PlaySoundButton(u16 Volume)
{
    ButtonVoice->SetVolume(Volume);
    ButtonVoice->Play(RollOverSound);
}
