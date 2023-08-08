//-----------------------------------------------------------------------------
// File:			W_SoundManager.cpp
// Original Author:	Hesham ElHamshary
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_SoundManager.h"

namespace wolf
{

    ISoundEngine *SoundManager::m_pSoundEngine;
    SoundManager::SoundMap SoundManager::m_mSoundMap;

    //----------------------------------------------------------
    // Creates an IrrKlang Sound engine.
    //----------------------------------------------------------
    void SoundManager::DestroySoundSystem()
    {
        m_mSoundMap.clear();
        if (!m_pSoundEngine)
            return;
        m_pSoundEngine->drop();
        m_pSoundEngine = nullptr;
    }

    //----------------------------------------------------------
    // Creates an IrrKlang Sound engine.
    //----------------------------------------------------------
    void SoundManager::CreateSoundSystem()
    {
        m_pSoundEngine = createIrrKlangDevice();

        if (!m_pSoundEngine)
        {
            printf("ERROR: Could not create sound engine.");
        }
    }

    void SoundManager::SetListenerPosition(const glm::vec3 &position, const glm::vec3 &direction)
    {
        if (!m_pSoundEngine)
            return;
        m_pSoundEngine->setListenerPosition(vec3df(position.x, position.y, position.z), vec3df(direction.x, direction.y, direction.z));
    }

    //----------------------------------------------------------
    // Plays 2D sound and stores it in Sound Map.
    // Replays sound if it already exists and is not finished
    //----------------------------------------------------------
    void SoundManager::Play2D(const std::string &sName, const std::string &sPath, bool bLooped, bool bForced)
    {
        if (!m_pSoundEngine)
            return;
        ISound *pSound = nullptr;
        SoundMap::iterator it = m_mSoundMap.find(sName);
        if (it != m_mSoundMap.end())
        {
            pSound = (ISound *)it->second;
            if (!pSound->isFinished() && bForced)
            {
                pSound->setPlayPosition(0);
                pSound->setIsPaused(false);
                return;
            }
            m_pSoundEngine->play2D(sPath.c_str(), bLooped, false, true);
        }
        else
        {
            ISound *pSound = m_pSoundEngine->play2D(sPath.c_str(), bLooped, false, true);
            m_mSoundMap.insert(std::pair<std::string, ISound *>(sName, pSound));
        }
    }

    //----------------------------------------------------------
    // Playes 3D sound and stores it in Sound Map.
    // Replays sound if it already exists and is not finished
    //----------------------------------------------------------
    void SoundManager::Play3D(const std::string &sName, const std::string &sPath, const glm::vec3 &vPosition, float minDistance, bool bLooped)
    {
        if (!m_pSoundEngine)
            return;
        ISound *pSound = nullptr;
        SoundMap::iterator it = m_mSoundMap.find(sName);
        if (it != m_mSoundMap.end())
        {

            pSound = (ISound *)it->second;
            if (!pSound->isFinished())
            {
                pSound->setPlayPosition(0);
                pSound->setIsPaused(false);
                return;
            }
            m_pSoundEngine->play3D(sPath.c_str(), vec3df(vPosition.x, vPosition.y, vPosition.z), bLooped, false, true);
        }
        else
        {
            ISound *pSound = m_pSoundEngine->play3D(sPath.c_str(), vec3df(vPosition.x, vPosition.y, vPosition.z), bLooped, false, true);
            pSound->setMinDistance(minDistance);
            m_mSoundMap.insert(std::pair<std::string, ISound *>(sName, pSound));
        }
    }

    //----------------------------------------------------------
    // Pause sound if it exists in map
    //----------------------------------------------------------
    void SoundManager::PauseSound(const std::string &sName)
    {
        ISound *pSound = NULL;
        SoundMap::iterator it = m_mSoundMap.find(sName);
        if (it != m_mSoundMap.end())
        {
            pSound = (ISound *)it->second;
            pSound->setIsPaused(true);
        }
        else
        {
            printf("ERROR: Could not pause %s because it does not exist", sName.c_str());
            return;
        }
    }

    //----------------------------------------------------------
    // Stop sound if it exists in map
    //----------------------------------------------------------
    void SoundManager::StopSound(const std::string &sName)
    {
        ISound *pSound = NULL;
        SoundMap::iterator it = m_mSoundMap.find(sName);
        if (it != m_mSoundMap.end())
        {
            pSound = (ISound *)it->second;
            pSound->stop();
        }
        else
        {
            printf("ERROR: Could not stop %s because it does not exist", sName.c_str());
            return;
        }
    }

    //----------------------------------------------------------
    // Stop all sounds
    //----------------------------------------------------------
    void SoundManager::StopAllSounds()
    {
        SoundMap::iterator it;
        for (it = m_mSoundMap.begin(); it != m_mSoundMap.end(); it++)
        {
            ISound *sound = (ISound *)it->second;
            sound->stop();
        }
    }

    //----------------------------------------------------------
    // Pause all sounds
    //----------------------------------------------------------
    void SoundManager::PauseAllSounds()
    {
        SoundMap::iterator it;
        for (it = m_mSoundMap.begin(); it != m_mSoundMap.end(); it++)
        {
            ISound *sound = (ISound *)it->second;
            sound->setIsPaused(true);
        }
    }

    ISound *SoundManager::GetSound(std::string sName)
    {
        ISound *pSound = nullptr;
        SoundMap::iterator it = m_mSoundMap.find(sName);
        if (it != m_mSoundMap.end())
        {
            pSound = (ISound *)it->second;
        }
        return pSound;
    }

}
