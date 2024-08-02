cmake_minimum_required(VERSION 3.16)
project(SoundTouch VERSION 2.3.3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)

set(COMPILE_OPTIONS)

if(MSVC)
  list(APPEND COMPILE_OPTIONS /O2 /fp:fast)
else()
  list(APPEND COMPILE_OPTIONS -Wall -Wextra -Wzero-as-null-pointer-constant -Wno-unknown-pragmas)
  if(EMSCRIPTEN)
    list(APPEND COMPILE_OPTIONS -O3)
  else()
    list(APPEND COMPILE_OPTIONS -Ofast)
  endif()
endif()

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

#####################
# SoundTouch library

add_library(SoundTouch STATIC
  source/SoundTouch/AAFilter.cpp
  source/SoundTouch/BPMDetect.cpp
  source/SoundTouch/cpu_detect_x86.cpp
  source/SoundTouch/FIFOSampleBuffer.cpp
  source/SoundTouch/FIRFilter.cpp
  source/SoundTouch/InterpolateCubic.cpp
  source/SoundTouch/InterpolateLinear.cpp
  source/SoundTouch/InterpolateShannon.cpp
  source/SoundTouch/mmx_optimized.cpp
  source/SoundTouch/PeakFinder.cpp
  source/SoundTouch/RateTransposer.cpp
  source/SoundTouch/SoundTouch.cpp
  source/SoundTouch/sse_optimized.cpp
  source/SoundTouch/TDStretch.cpp
)
target_include_directories(SoundTouch PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(SoundTouch PRIVATE ${COMPILE_DEFINITIONS})
target_compile_options(SoundTouch PRIVATE ${COMPILE_OPTIONS})
set_target_properties(SoundTouch PROPERTIES OUTPUT_NAME "soundtouch-static")

option(INTEGER_SAMPLES "Use integers instead of floats for samples" OFF)
if(INTEGER_SAMPLES)
  target_compile_definitions(SoundTouch PRIVATE SOUNDTOUCH_INTEGER_SAMPLES)
  set(SOUNDTOUCH_INTEGER_SAMPLES TRUE)
else()
  target_compile_definitions(SoundTouch PRIVATE SOUNDTOUCH_FLOAT_SAMPLES)
  set(SOUNDTOUCH_FLOAT_SAMPLES TRUE)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv7.*)$")
  target_compile_options(SoundTouch PRIVATE -mfpu=neon)
endif()

install(
  FILES
    include/BPMDetect.h
    include/FIFOSampleBuffer.h
    include/FIFOSamplePipe.h
    include/STTypes.h
    include/SoundTouch.h
  DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/soundtouch"
)


########################
# SoundTouchDll library

option(SOUNDTOUCH_DLL "Build SoundTouchDLL C wrapper library" ON)
if(SOUNDTOUCH_DLL)
  add_library(SoundTouchDLL SHARED
    source/SoundTouchDLL/SoundTouchDLL.cpp
    source/SoundTouchDLL/SoundTouchDLL.rc
  )
  set_target_properties(SoundTouch PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
  target_compile_options(SoundTouchDLL PRIVATE ${COMPILE_OPTIONS})
  set_target_properties(SoundTouchDLL PROPERTIES CXX_VISIBILITY_PRESET hidden)
  target_compile_definitions(SoundTouchDLL PRIVATE DLL_EXPORTS)
  target_include_directories(SoundTouchDLL PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
  target_link_libraries(SoundTouchDLL PRIVATE SoundTouch)
  set_target_properties(SoundTouchDLL PROPERTIES OUTPUT_NAME "soundtouch")
  set_target_properties(SoundTouchDLL PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})
  if(NOT WIN32)
    set_target_properties(SoundTouchDLL PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
  endif()
  install(FILES source/SoundTouchDLL/SoundTouchDLL.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/soundtouch")
  install(TARGETS SoundTouchDLL EXPORT SoundTouchTargets)
endif()

########################

# CMake config
include(CMakePackageConfigHelpers)
set(SOUNDTOUCH_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/SoundTouch")
install(
  EXPORT SoundTouchTargets
  FILE SoundTouchTargets.cmake
  NAMESPACE SoundTouch::
  DESTINATION "${SOUNDTOUCH_INSTALL_CMAKEDIR}"
)
configure_package_config_file(SoundTouchConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/SoundTouchConfig.cmake"
  INSTALL_DESTINATION "${SOUNDTOUCH_INSTALL_CMAKEDIR}"
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SoundTouchConfigVersion.cmake"
  VERSION "${CMAKE_PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SoundTouchConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SoundTouchConfigVersion.cmake"
  DESTINATION "${SOUNDTOUCH_INSTALL_CMAKEDIR}"
)
