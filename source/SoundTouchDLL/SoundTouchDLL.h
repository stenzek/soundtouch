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

#ifndef _SoundTouchDLL_h_
#define _SoundTouchDLL_h_

#if defined(_WIN32) || defined(WIN32)
    // Windows
    #ifndef __cplusplus
        #error "Expected g++"
    #endif

    #ifdef DLL_EXPORTS
        #define SOUNDTOUCHDLL_API extern "C" __declspec(dllexport)
    #else
        #define SOUNDTOUCHDLL_API extern "C" __declspec(dllimport)
    #endif

#else
    // GNU version

    #if defined(DLL_EXPORTS) || defined(SoundTouchDLL_EXPORTS)
        // GCC declaration for exporting functions
        #define SOUNDTOUCHDLL_API extern "C" __attribute__((__visibility__("default")))
    #else
        // import function
        #define SOUNDTOUCHDLL_API extern "C"
    #endif

    // Linux-replacements for Windows declarations:
    #define __cdecl
    typedef unsigned int DWORD;
    #define FALSE    0
    #define TRUE    1

#endif

typedef void * ST_HANDLE;

/// Create a new instance of SoundTouch processor.
SOUNDTOUCHDLL_API ST_HANDLE __cdecl soundtouch_createInstance();

/// Destroys a SoundTouch processor instance.
SOUNDTOUCHDLL_API void __cdecl soundtouch_destroyInstance(ST_HANDLE h);

/// Get SoundTouch library version string
SOUNDTOUCHDLL_API const char *__cdecl soundtouch_getVersionString();

/// Get SoundTouch library version Id
SOUNDTOUCHDLL_API unsigned int __cdecl soundtouch_getVersionId();

/// Sets new rate control value. Normal rate = 1.0, smaller values
/// represent slower rate, larger faster rates.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setRate(ST_HANDLE h, float newRate);

/// Sets new tempo control value. Normal tempo = 1.0, smaller values
/// represent slower tempo, larger faster tempo.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setTempo(ST_HANDLE h, float newTempo);

/// Sets new rate control value as a difference in percents compared
/// to the original rate (-50 .. +100 %);
SOUNDTOUCHDLL_API void __cdecl soundtouch_setRateChange(ST_HANDLE h, float newRate);

/// Sets new tempo control value as a difference in percents compared
/// to the original tempo (-50 .. +100 %);
SOUNDTOUCHDLL_API void __cdecl soundtouch_setTempoChange(ST_HANDLE h, float newTempo);

/// Sets new pitch control value. Original pitch = 1.0, smaller values
/// represent lower pitches, larger values higher pitch.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setPitch(ST_HANDLE h, float newPitch);

/// Sets pitch change in octaves compared to the original pitch
/// (-1.00 .. +1.00);
SOUNDTOUCHDLL_API void __cdecl soundtouch_setPitchOctaves(ST_HANDLE h, float newPitch);

/// Sets pitch change in semi-tones compared to the original pitch
/// (-12 .. +12);
SOUNDTOUCHDLL_API void __cdecl soundtouch_setPitchSemiTones(ST_HANDLE h, float newPitch);


/// Sets the number of channels, 1 = mono, 2 = stereo, n = multichannel
SOUNDTOUCHDLL_API void __cdecl soundtouch_setChannels(ST_HANDLE h, unsigned int numChannels);

/// Sets sample rate.
SOUNDTOUCHDLL_API void __cdecl soundtouch_setSampleRate(ST_HANDLE h, unsigned int srate);

/// Flushes the last samples from the processing pipeline to the output.
/// Clears also the internal processing buffers.
//
/// Note: This function is meant for extracting the last samples of a sound
/// stream. This function may introduce additional blank samples in the end
/// of the sound stream, and thus it's not recommended to call this function
/// in the middle of a sound stream.
SOUNDTOUCHDLL_API void __cdecl soundtouch_flush(ST_HANDLE h);

/// Adds 'numSamples' pcs of samples from the 'samples' memory position into
/// the input of the object. Notice that sample rate _has_to_ be set before
/// calling this function, otherwise throws a runtime_error exception.
SOUNDTOUCHDLL_API void __cdecl soundtouch_putSamples(ST_HANDLE h,
        const float *samples,       ///< Pointer to sample buffer.
        unsigned int numSamples     ///< Number of sample frames in buffer. Notice
                                    ///< that in case of multi-channel sound a single
                                    ///< sample frame contains data for all channels.
);

/// Clears all the samples in the object's output and internal processing
/// buffers.
SOUNDTOUCHDLL_API void __cdecl soundtouch_clear(ST_HANDLE h);

/// Changes a setting controlling the processing system behaviour. See the
/// 'SETTING_...' defines for available setting ID's.
///
/// \return 'nonzero' if the setting was successfully changed, otherwise zero
SOUNDTOUCHDLL_API int __cdecl soundtouch_setSetting(ST_HANDLE h,
        int settingId,   ///< Setting ID number. see SETTING_... defines.
        int value        ///< New setting value.
);

/// Reads a setting controlling the processing system behaviour. See the
/// 'SETTING_...' defines for available setting ID's.
///
/// \return the setting value.
SOUNDTOUCHDLL_API int __cdecl soundtouch_getSetting(ST_HANDLE h,
        int settingId    ///< Setting ID number, see SETTING_... defines.
);


/// Returns number of samples currently unprocessed.
SOUNDTOUCHDLL_API unsigned int __cdecl soundtouch_numUnprocessedSamples(ST_HANDLE h);

/// Adjusts book-keeping so that given number of samples are removed from beginning of the
/// sample buffer without copying them anywhere.
///
/// Used to reduce the number of samples in the buffer when accessing the sample buffer directly
/// with 'ptrBegin' function.
SOUNDTOUCHDLL_API unsigned int __cdecl soundtouch_receiveSamples(ST_HANDLE h,
        float *outBuffer,           ///< Buffer where to copy output samples.
        unsigned int maxSamples     ///< How many samples to receive at max.
);

/// Returns number of samples currently available.
SOUNDTOUCHDLL_API unsigned int __cdecl soundtouch_numSamples(ST_HANDLE h);

/// Returns nonzero if there aren't any samples available for outputting.
SOUNDTOUCHDLL_API int __cdecl soundtouch_isEmpty(ST_HANDLE h);

#endif  // _SoundTouchDLL_h_

