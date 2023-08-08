//-----------------------------------------------------------------------------
// File:			W_SoundManager.h
// Original Author:	Hesham Hamshary
//
// Class to Manage Sound with Irrklang
//-----------------------------------------------------------------------------
#ifndef W_SOUND_MANAGER_H
#define W_SOUND_MANAGER_H

#include "W_Types.h"
#include "W_Program.h"
#include <string>
#include <map>
#include "../irrklang/include/irrKlang.h"

using namespace irrklang;

namespace wolf
{
    class SoundManager
    {
    public:
        //-------------------------------------------------------------------------
        // PUBLIC TYPES
        //-------------------------------------------------------------------------

        typedef std::map<std::string, ISound *> SoundMap;

        //-------------------------------------------------------------------------
        // PUBLIC INTERFACE
        //-------------------------------------------------------------------------

        static void CreateSoundSystem();
        static void SetListenerPosition(const glm::vec3 &position, const glm::vec3 &direction);
        static void Play2D(const std::string &sName, const std::string &sPath, bool bLooped = false, bool bForced = false);
        static void Play3D(const std::string &sName, const std::string &sPath, const glm::vec3 &vPosition, float minDistance, bool bLooped = false);
        static void PauseSound(const std::string &sName);
        static void StopSound(const std::string &sName);
        static void StopAllSounds();
        static void PauseAllSounds();
        static void DestroySoundSystem();
        static ISound *GetSound(std::string sName);

    private:
        //-------------------------------------------------------------------------
        // PRIVATE MEMBERS
        //-------------------------------------------------------------------------
        static ISoundEngine *m_pSoundEngine;

        static SoundMap m_mSoundMap;
    };

}

#endif
