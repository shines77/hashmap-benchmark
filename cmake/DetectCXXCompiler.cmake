# ----------------------------------------------------------------------------
# Detect Microsoft compiler:
# ----------------------------------------------------------------------------
if (CMAKE_CL_64)
  set(MSVC64 1)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_GNUCC 1)
  set(CMAKE_COMPILER_IS_CLANGCC 1)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_COMPILER_IS_GNUCC 1)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_GNUCXX 1)
  set(CMAKE_COMPILER_IS_CLANGCXX 1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_COMPILER_IS_GNUCXX 1)
endif()

if ("${CMAKE_CXX_COMPILER};${CMAKE_C_COMPILER}" MATCHES "ccache")
  set(CMAKE_COMPILER_IS_CCACHE 1)
endif()

# ----------------------------------------------------------------------------
# Detect Intel ICC compiler -- for -fPIC in 3rdparty ( UNIX ONLY ):
#  see  include/opencv/cxtypes.h file for related   ICC & CV_ICC defines.
# NOTE: The system needs to determine if the '-fPIC' option needs to be added
#  for the 3rdparty static libs being compiled.  The CMakeLists.txt files
#  in 3rdparty use the CV_ICC definition being set here to determine if
#  the -fPIC flag should be used.
# ----------------------------------------------------------------------------
if (UNIX)
  if (__ICL)
    set(CV_ICC   __ICL)
  elseif (__ICC)
    set(CV_ICC   __ICC)
  elseif (__ECL)
    set(CV_ICC   __ECL)
  elseif (__ECC)
    set(CV_ICC   __ECC)
  elseif (__INTEL_COMPILER)
    set(CV_ICC   __INTEL_COMPILER)
  elseif (CMAKE_C_COMPILER MATCHES "icc")
    set(CV_ICC   icc_matches_c_compiler)
  endif()
endif()

if (MSVC AND CMAKE_C_COMPILER MATCHES "icc|icl")
  set(CV_ICC   __INTEL_COMPILER_FOR_WINDOWS)
endif()

# ----------------------------------------------------------------------------
# Detect GNU version:
# ----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_CLANGCXX)
  set(CMAKE_GCC_REGEX_VERSION "4.2.1")
  set(CMAKE_JSTD_GCC_VERSION_MAJOR 4)
  set(CMAKE_JSTD_GCC_VERSION_MINOR 2)
  set(CMAKE_JSTD_GCC_VERSION 42)
  set(CMAKE_JSTD_GCC_VERSION_NUM 402)

  execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -v
                  ERROR_VARIABLE CMAKE_JSTD_CLANG_VERSION_FULL
                  ERROR_STRIP_TRAILING_WHITESPACE)

  string(REGEX MATCH "version.*$" CMAKE_JSTD_CLANG_VERSION_FULL "${CMAKE_JSTD_CLANG_VERSION_FULL}")
  string(REGEX MATCH "[0-9]+\\.[0-9]+" CMAKE_CLANG_REGEX_VERSION "${CMAKE_JSTD_CLANG_VERSION_FULL}")

elseif (CMAKE_COMPILER_IS_GNUCXX)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
                OUTPUT_VARIABLE CMAKE_JSTD_GCC_VERSION_FULL
                OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -v
                ERROR_VARIABLE CMAKE_JSTD_GCC_INFO_FULL
                OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Typical output in CMAKE_JSTD_GCC_VERSION_FULL: "c+//0 (whatever) 4.2.3 (...)"
  # Look for the version number, major.minor.build
  string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CMAKE_GCC_REGEX_VERSION "${CMAKE_JSTD_GCC_VERSION_FULL}")
  if (NOT CMAKE_GCC_REGEX_VERSION) # major.minor
    string(REGEX MATCH "[0-9]+\\.[0-9]+" CMAKE_GCC_REGEX_VERSION "${CMAKE_JSTD_GCC_VERSION_FULL}")
  endif()

  # dumpversion prints only major version since gcc 7.0
  if ((NOT CMAKE_GCC_REGEX_VERSION) AND (${CMAKE_JSTD_GCC_VERSION_FULL} GREATER 6))
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -dumpfullversion
                  OUTPUT_VARIABLE CMAKE_JSTD_GCC_VERSION_FULL
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CMAKE_GCC_REGEX_VERSION "${CMAKE_JSTD_GCC_VERSION_FULL}")
  endif()

  if (CMAKE_GCC_REGEX_VERSION)
    # Split the parts:
    string(REGEX MATCHALL "[0-9]+" CMAKE_JSTD_GCC_VERSIONS "${CMAKE_GCC_REGEX_VERSION}")

    list(GET CMAKE_JSTD_GCC_VERSIONS 0 CMAKE_JSTD_GCC_VERSION_MAJOR)
    list(GET CMAKE_JSTD_GCC_VERSIONS 1 CMAKE_JSTD_GCC_VERSION_MINOR)
  else() # compiler returned just the major version number
    string(REGEX MATCH "[0-9]+" CMAKE_GCC_REGEX_VERSION "${CMAKE_JSTD_GCC_VERSION_FULL}")
    if (NOT CMAKE_GCC_REGEX_VERSION) # compiler did not return anything reasonable
      set(CMAKE_GCC_REGEX_VERSION "0")
      message(WARNING "GCC version not detected!")
    endif()
    set(CMAKE_JSTD_GCC_VERSION_MAJOR ${CMAKE_GCC_REGEX_VERSION})
    set(CMAKE_JSTD_GCC_VERSION_MINOR 0)
  endif()

  set(CMAKE_JSTD_GCC_VERSION ${CMAKE_JSTD_GCC_VERSION_MAJOR}${CMAKE_JSTD_GCC_VERSION_MINOR})
  math(EXPR CMAKE_JSTD_GCC_VERSION_NUM "${CMAKE_JSTD_GCC_VERSION_MAJOR}*100 + ${CMAKE_JSTD_GCC_VERSION_MINOR}")
  message(STATUS "Detected version of GNU GCC: ${CMAKE_JSTD_GCC_VERSION} (${CMAKE_JSTD_GCC_VERSION_NUM})")

  if (WIN32)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
              OUTPUT_VARIABLE JSTD_GCC_TARGET_MACHINE
              OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (JSTD_GCC_TARGET_MACHINE MATCHES "amd64|x86_64|AMD64")
      set(MINGW64 1)
    endif()
  endif()
endif()

if (MSVC64 OR MINGW64)
  set(X86_64 1)
elseif (MINGW OR (MSVC AND NOT CMAKE_CROSSCOMPILING))
  set(X86 1)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*")
  set(X86_64 1)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i686.*|i386.*|x86.*|amd64.*|AMD64.*")
  set(X86 1)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")
  set(ARM 1)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*)")
  set(AARCH64 1)
endif()

# Workaround for 32-bit operating systems on 64-bit x86_64 processor
if (X86_64 AND CMAKE_SIZEOF_VOID_P EQUAL 4 AND NOT FORCE_X86_64)
  message(STATUS "sizeof(void) = 4 on x86 / x86_64 processor. Assume 32-bit compilation mode (X86=1)")
  unset(X86_64)
  set(X86 1)
endif()

# Similar code exists in JstdConfig.cmake
if (NOT DEFINED JSTD_STATIC)
  # look for global setting
  if (NOT DEFINED BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
    set(JSTD_STATIC OFF)
  else()
    set(JSTD_STATIC ON)
  endif()
endif()

if (MSVC)
  if (CMAKE_CL_64)
    set(JSTD_ARCH x64)
  elseif ((CMAKE_GENERATOR MATCHES "ARM") OR ("${arch_hint}" STREQUAL "ARM") OR (CMAKE_VS_EFFECTIVE_PLATFORMS MATCHES "ARM|arm"))
    # see Modules/CmakeGenericSystem.cmake
    set(JSTD_ARCH ARM)
  else()
    set(JSTD_ARCH x86)
  endif()
  if (MSVC_VERSION EQUAL 1400)
    set(JSTD_RUNTIME vc8)
  elseif (MSVC_VERSION EQUAL 1500)
    set(JSTD_RUNTIME vc9)
  elseif (MSVC_VERSION EQUAL 1600)
    set(JSTD_RUNTIME vc10)
  elseif (MSVC_VERSION EQUAL 1700)
    set(JSTD_RUNTIME vc11)
  elseif (MSVC_VERSION EQUAL 1800)
    set(JSTD_RUNTIME vc12)
  elseif (MSVC_VERSION EQUAL 1900)
    set(JSTD_RUNTIME vc14)
  elseif (MSVC_VERSION EQUAL 1910 OR MSVC_VERSION EQUAL 1911)
    set(JSTD_RUNTIME vc15)
  endif()
elseif (MINGW)
  set(JSTD_RUNTIME mingw)

  if (MINGW64)
    set(JSTD_ARCH x64)
  else()
    set(JSTD_ARCH x86)
  endif()
endif()

# Fix handling of duplicated files in the same static library:
# https://public.kitware.com/Bug/view.php?id=14874
if (CMAKE_VERSION VERSION_LESS "3.1")
  foreach(var CMAKE_C_ARCHIVE_APPEND CMAKE_CXX_ARCHIVE_APPEND)
    if (${var} MATCHES "^<CMAKE_AR> r")
      string(REPLACE "<CMAKE_AR> r" "<CMAKE_AR> q" ${var} "${${var}}")
    endif()
  endforeach()
endif()

if (ENABLE_CXX11)
  # cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
  set(CMAKE_CXX_EXTENSIONS OFF) # use -std=c++11 instead of -std=gnu++11
  if (CMAKE_CXX11_COMPILE_FEATURES)
    set(HAVE_CXX11 ON)
  endif()
endif()

if (NOT HAVE_CXX11)
  jstd_check_compiler_flag(CXX "" HAVE_CXX11 "${PROJECT_SOURCE_DIR}/cmake/checks/cxx11.cpp")
  if (NOT HAVE_CXX11 AND ENABLE_CXX11)
    jstd_check_compiler_flag(CXX "-std=c++11" HAVE_STD_CXX11 "${PROJECT_SOURCE_DIR}/cmake/checks/cxx11.cpp")
    if (HAVE_STD_CXX11)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
      set(HAVE_CXX11 ON)
    endif()
  endif()
endif()
