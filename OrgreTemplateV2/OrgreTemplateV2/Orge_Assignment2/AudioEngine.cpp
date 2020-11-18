#include "AudioEngine.h"

Implementation::Implementation() {
    mpStudioSystem = NULL;
    DoofAudioEngine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
    DoofAudioEngine::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_PROFILE_ENABLE, NULL));

    mpSystem = NULL;
    DoofAudioEngine::ErrorCheck(mpStudioSystem->getCoreSystem(&mpSystem));
}

Implementation::~Implementation()
{
    DoofAudioEngine::ErrorCheck(mpStudioSystem->unloadAll());
    DoofAudioEngine::ErrorCheck(mpStudioSystem->release());
}


void Implementation::Update() {
    std::vector<ChannelMap::iterator> pStoppedChannels;
    for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
    {
        bool bIsPlaying = false;
        it->second->isPlaying(&bIsPlaying);
        if (!bIsPlaying)
        {
            pStoppedChannels.push_back(it);
        }
    }
    for (auto& it : pStoppedChannels)
    {
        mChannels.erase(it);
    }
    DoofAudioEngine::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

void DoofAudioEngine::Init() {
    sgpImplementation = new Implementation();
    std::cout << "Doof Engine Running";
}

void DoofAudioEngine::Update() {
    sgpImplementation->Update();
}

void DoofAudioEngine::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
    auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
    if (tFoundIt != sgpImplementation->mSounds.end())
        return;

    FMOD_MODE eMode = FMOD_DEFAULT;
    eMode |= b3d ? FMOD_3D : FMOD_2D;
    eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

    FMOD::Sound* pSound = nullptr;
    DoofAudioEngine::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
    if (pSound) {
        sgpImplementation->mSounds[strSoundName] = pSound;
    }

}

void DoofAudioEngine::UnLoadSound(const std::string& strSoundName)
{
    auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
    if (tFoundIt == sgpImplementation->mSounds.end())
        return;

    DoofAudioEngine::ErrorCheck(tFoundIt->second->release());
    sgpImplementation->mSounds.erase(tFoundIt);
}

void DoofAudioEngine::Set3dListenerAndOrientation(const Vector3& vPos, float fVolumedB)
{
}

int DoofAudioEngine::PlaySounds(const string& strSoundName, const Vector3& vPos, float fVolumedB)
{
    int nChannelId = sgpImplementation->mnNextChannelId++;
    auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
    if (tFoundIt == sgpImplementation->mSounds.end())
    {
        LoadSound(strSoundName);
        tFoundIt = sgpImplementation->mSounds.find(strSoundName);
        if (tFoundIt == sgpImplementation->mSounds.end())
        {
            return nChannelId;
        }
    }
    FMOD::Channel* pChannel = nullptr;
    DoofAudioEngine::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
    if (pChannel)
    {
        FMOD_MODE currMode;
        tFoundIt->second->getMode(&currMode);
        if (currMode & FMOD_3D) {
            FMOD_VECTOR position = VectorToFmod(vPos);
            DoofAudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
        }
        DoofAudioEngine::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
        DoofAudioEngine::ErrorCheck(pChannel->setPaused(false));
        sgpImplementation->mChannels[nChannelId] = pChannel;
    }
    return nChannelId;
}

void DoofAudioEngine::SetChannel3dPosition(int nChannelId, const Vector3& vPosition)
{
    auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
    if (tFoundIt == sgpImplementation->mChannels.end())
        return;

    FMOD_VECTOR position = VectorToFmod(vPosition);
    DoofAudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void DoofAudioEngine::SetChannelVolume(int nChannelId, float fVolumedB)
{
    auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
    if (tFoundIt == sgpImplementation->mChannels.end())
        return;

    DoofAudioEngine::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}

void DoofAudioEngine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
    auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
    if (tFoundIt != sgpImplementation->mBanks.end())
        return;
    FMOD::Studio::Bank* pBank;
    DoofAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
    if (pBank) {
        sgpImplementation->mBanks[strBankName] = pBank;
    }
}

void DoofAudioEngine::LoadEvent(const std::string& strEventName) {
    auto tFoundit = sgpImplementation->mEvents.find(strEventName);
    if (tFoundit != sgpImplementation->mEvents.end())
        return;
    FMOD::Studio::EventDescription* pEventDescription = NULL;
    DoofAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
    if (pEventDescription) {
        FMOD::Studio::EventInstance* pEventInstance = NULL;
        DoofAudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
        if (pEventInstance) {
            sgpImplementation->mEvents[strEventName] = pEventInstance;
        }
    }
}

void DoofAudioEngine::StopEvent(const string& strEventName, bool bImmediate) {
    auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
    if (tFoundIt == sgpImplementation->mEvents.end())
        return;

    FMOD_STUDIO_STOP_MODE eMode;
    eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
    DoofAudioEngine::ErrorCheck(tFoundIt->second->stop(eMode));
}

bool DoofAudioEngine::IsEventPlaying(const string& strEventName) const {
    auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
    if (tFoundIt == sgpImplementation->mEvents.end())
        return false;

    FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
    if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
        return true;
    }
    return false;
}

void DoofAudioEngine::GetEventParameter(const string& strEventName, const string& strParameterName, float* parameter) {
    auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
    if (tFoundIt == sgpImplementation->mEvents.end())
        return;

    //FMOD::Studio::EventDescription* pParameter = NULL;
    DoofAudioEngine::ErrorCheck(tFoundIt->second->getParameterByName(strParameterName.c_str(), parameter));
    //DoofAudioEngine::ErrorCheck(pParameter->setVal(parameter));
}

void DoofAudioEngine::SetEventParameter(const string& strEventName, const string& strParameterName, float fValue) {
    auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
    if (tFoundIt == sgpImplementation->mEvents.end())
        return;

    //FMOD::Studio::ParameterInstance* pParameter = NULL;
    DoofAudioEngine::ErrorCheck(tFoundIt->second->setParameterByName(strParameterName.c_str(), fValue));
    //DoofAudioEngine::ErrorCheck(pParameter->setValue(fValue));
}

FMOD_VECTOR DoofAudioEngine::VectorToFmod(const Vector3& vPosition) {
    FMOD_VECTOR fVec;
    fVec.x = vPosition.x;
    fVec.y = vPosition.y;
    fVec.z = vPosition.z;
    return fVec;
}

float  DoofAudioEngine::dbToVolume(float dB)
{
    return powf(10.0f, 0.05f * dB);
}

float  DoofAudioEngine::VolumeTodB(float volume)
{
    return 20.0f * log10f(volume);
}

int DoofAudioEngine::ErrorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        cout << "FMOD ERROR " << result << endl;
        return 1;
    }
    // cout << "FMOD all good" << endl;
    return 0;
}

void DoofAudioEngine::Shutdown() {
    delete sgpImplementation;
    std::cout << "Doof Engine Shutting Down";
}

