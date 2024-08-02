//////////////////////////////////////////////////////////////////////////////
///
/// SoundTouch DLL wrapper - wraps SoundTouch routines into a Dynamic Load
/// Library interface.
///
/// Author        : Copyright (c) Olli Parviainen
/// Author e-mail : oparviai 'at' iki.fi
/// SoundTouch WWW: http://www.surina.net/soundtouch
///
////////////////////////////////////////////////////////////////////////////////
//
// License :
//
//  SoundTouch audio processing library
//  Copyright (c) Olli Parviainen
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
////////////////////////////////////////////////////////////////////////////////


#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>

    // DLL main in Windows compilation
    BOOL APIENTRY DllMain( HANDLE hModule,
                           DWORD  ul_reason_for_call,
                           LPVOID lpReserved
                         )
    {
        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        }
        return TRUE;
    }
#endif

#include <limits.h>
#include <string.h>
#include "SoundTouchDLL.h"
#include "SoundTouch.h"
#include "BPMDetect.h"

using namespace soundtouch;

#ifdef SOUNDTOUCH_INTEGER_SAMPLES
    #error "error - compile the dll version with float samples"
#endif // SOUNDTOUCH_INTEGER_SAMPLES

//////////////

SOUNDTOUCHDLL_API ST_HANDLE __cdecl soundtouch_createInstance()
{
    return new SoundTouch();
}


SOUNDTOUCHDLL_API void __cdecl soundtouch_destroyInstance(ST_HANDLE h)
{
    delete static_cast<SoundTouch*>(h);
}


/// Get SoundTouch library version string
SOUNDTOUCHDLL_API const char *__cdecl soundtouch_getVersionString()
{
    return SoundTouch::getVersionString();
}


/// Get SoundTouch library version Id
SOUNDTOUCHDLL_API uint __cdecl soundtouch_getVersionId()
{
    return SoundTouch::getVersionId();
}

/// Sets new rate control value. Normal rate = 1.0, smaller values
/// represent slower rate, larger faster rates.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setRate(ST_HANDLE h, float newRate)
{
    static_cast<SoundTouch*>(h)->setRate(newRate);
}


/// Sets new tempo control value. Normal tempo = 1.0, smaller values
/// represent slower tempo, larger faster tempo.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setTempo(ST_HANDLE h, float newTempo)
{
    static_cast<SoundTouch*>(h)->setTempo(newTempo);
}

/// Sets new rate control value as a difference in percents compared
/// to the original rate (-50 .. +100 %)
SOUNDTOUCHDLL_API void __cdecl soundtouch_setRateChange(ST_HANDLE h, float newRate)
{
    static_cast<SoundTouch*>(h)->setRateChange(newRate);
}

/// Sets new tempo control value as a difference in percents compared
/// to the original tempo (-50 .. +100 %)
SOUNDTOUCHDLL_API void __cdecl soundtouch_setTempoChange(ST_HANDLE h, float newTempo)
{
    static_cast<SoundTouch*>(h)->setTempoChange(newTempo);
}

/// Sets new pitch control value. Original pitch = 1.0, smaller values
/// represent lower pitches, larger values higher pitch.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setPitch(ST_HANDLE h, float newPitch)
{
    static_cast<SoundTouch*>(h)->setPitch(newPitch);
}

/// Sets pitch change in octaves compared to the original pitch
/// (-1.00 .. +1.00)
SOUNDTOUCHDLL_API void __cdecl soundtouch_setPitchOctaves(ST_HANDLE h, float newPitch)
{
    static_cast<SoundTouch*>(h)->setPitchOctaves(newPitch);
}

/// Sets pitch change in semi-tones compared to the original pitch
/// (-12 .. +12)
SOUNDTOUCHDLL_API void __cdecl soundtouch_setPitchSemiTones(ST_HANDLE h, float newPitch)
{
    static_cast<SoundTouch*>(h)->setPitchSemiTones(newPitch);
}


/// Sets the number of channels, 1 = mono, 2 = stereo
SOUNDTOUCHDLL_API void __cdecl soundtouch_setChannels(ST_HANDLE h, uint numChannels)
{
    static_cast<SoundTouch*>(h)->setChannels(numChannels);
}

/// Sets sample rate.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setSampleRate(ST_HANDLE h, uint srate)
{
    static_cast<SoundTouch*>(h)->setSampleRate(srate);
}

/// Flushes the last samples from the processing pipeline to the output.
/// Clears also the internal processing buffers.
//
/// Note: This function is meant for extracting the last samples of a sound
/// stream. This function may introduce additional blank samples in the end
/// of the sound stream, and thus it's not recommended to call this function
/// in the middle of a sound stream.
SOUNDTOUCHDLL_API void __cdecl soundtouch_flush(ST_HANDLE h)
{
    static_cast<SoundTouch*>(h)->flush();
}

/// Adds 'numSamples' pcs of samples from the 'samples' memory position into
/// the input of the object. Notice that sample rate _has_to_ be set before
/// calling this function, otherwise throws a runtime_error exception.
SOUNDTOUCHDLL_API void __cdecl soundtouch_putSamples(ST_HANDLE h,
        const SAMPLETYPE *samples,      ///< Pointer to sample buffer.
        unsigned int numSamples         ///< Number of samples in buffer. Notice
                                        ///< that in case of stereo-sound a single sample
                                        ///< contains data for both channels.
        )
{
    static_cast<SoundTouch*>(h)->putSamples(samples, numSamples);
}

/// Clears all the samples in the object's output and internal processing
/// buffers.
SOUNDTOUCHDLL_API void __cdecl soundtouch_clear(ST_HANDLE h)
{
  return static_cast<SoundTouch*>(h)->clear();
}

/// Changes a setting controlling the processing system behaviour. See the
/// 'SETTING_...' defines for available setting ID's.
///
/// \return 'nonzero' if the setting was successfully changed
SOUNDTOUCHDLL_API int __cdecl soundtouch_setSetting(ST_HANDLE h,
        int settingId,   ///< Setting ID number. see SETTING_... defines.
        int value        ///< New setting value.
        )
{
    return static_cast<SoundTouch*>(h)->setSetting(settingId, value);
}

/// Reads a setting controlling the processing system behaviour. See the
/// 'SETTING_...' defines for available setting ID's.
///
/// \return the setting value.
SOUNDTOUCHDLL_API int __cdecl soundtouch_getSetting(ST_HANDLE h,
        int settingId    ///< Setting ID number, see SETTING_... defines.
        )
{
    return static_cast<SoundTouch*>(h)->getSetting(settingId);
}


/// Returns number of samples currently unprocessed.
SOUNDTOUCHDLL_API uint __cdecl soundtouch_numUnprocessedSamples(ST_HANDLE h)
{
    return static_cast<SoundTouch*>(h)->numUnprocessedSamples();
}


/// Receive ready samples from the processing pipeline.
///
/// if called with outBuffer=nullptr, just reduces amount of ready samples within the pipeline.
SOUNDTOUCHDLL_API uint __cdecl soundtouch_receiveSamples(ST_HANDLE h,
        SAMPLETYPE *outBuffer,      ///< Buffer where to copy output samples.
        unsigned int maxSamples     ///< How many samples to receive at max.
        )
{
    if (outBuffer)
    {
        return static_cast<SoundTouch*>(h)->receiveSamples(outBuffer, maxSamples);
    }
    else
    {
        return static_cast<SoundTouch*>(h)->receiveSamples(maxSamples);
    }
}


/// Returns number of samples currently available.
SOUNDTOUCHDLL_API uint __cdecl soundtouch_numSamples(ST_HANDLE h)
{
    return static_cast<SoundTouch*>(h)->numSamples();
}


/// Returns nonzero if there aren't any samples available for outputting.
SOUNDTOUCHDLL_API int __cdecl soundtouch_isEmpty(ST_HANDLE h)
{
    return static_cast<SoundTouch*>(h)->isEmpty();
}
