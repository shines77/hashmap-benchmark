
if(NOT detect_cpu_architectures_INC) # include guard
set(detect_cpu_architectures_INC)

#
# How to detect if 64 bit MSVC with cmake?
# See: https://stackoverflow.com/questions/39258250/how-to-detect-if-64-bit-msvc-with-cmake
#

# This code was originally written by Jake Petroules,
# https://github.com/axr/solar-cmake/blob/master/TargetArch.cmake

# then edited/extended by Daniel Russell with help from
# https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qprocessordetection.h
# https://opensource.apple.com/source/WTFEmbedded/WTFEmbedded-95/wtf/Platform.h.auto.html
# http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.kui0098a/armccref_babjidgc.htm
# https://sourceforge.net/p/predef/wiki/Architectures/
# https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html

# Regarding POWER/PowerPC, just as is noted in the Qt source,
# "There are many more known variants/revisions that we do not handle/detect."
# Set ppc_support to TRUE before including this file or ppc and ppc64
# will be treated as invalid architectures since they are no longer supported by Apple

# TODO: add more chips!!!! Tedious though.    
enable_language(C) # this must be at file-scope not function scope. Thus include-guard.

set(archdetect_c_code "
/* -----------------------------------ARM ---------------------------------- */

#if defined(__ARM64_ARCH_8__) || defined(__aarch64__) || defined(__ARMv8__) || defined(__ARMv8_A__) || defined(_M_ARM64)
    #if defined(__thumb__) || defined(_M_ARMT)
        #error cmake_ARCH armthumbv3
    #elif
        #error cmake_ARCH armv8
    #endif

#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__  ) || defined(__ARM_ARCH_7F__) || defined(__ARM_ARCH_7S__) || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7)
    #if defined(__thumb__) || defined(__thumb2__) || defined(_M_ARMT)
        #error cmake_ARCH armthumbv2
    #elif
        #error cmake_ARCH armv7
    #endif

#elif defined(__ARM_ARCH_6__) ||  defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6M__) || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6)
    #if defined(__thumb__) || defined(_M_ARMT)
        #error cmake_ARCH armthumbv1
    #elif
        #error cmake_ARCH armv6
    #endif

#elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__ ) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || defined(__MARM_ARMV5__) || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5)
    #error cmake_ARCH armv5

#elif defined(__ARM_ARCH_4__) ||  defined(__ARM_ARCH_4T__) || defined(__MARM_ARMV4__)
    #error cmake_ARCH armv4

#elif defined(__TARGET_ARCH_ARM)
    #error cmake_ARCH armv ## __TARGET_ARCH_ARM

#elif defined(__arm__) || defined(_M_ARM)
    #error arm

/* ----------------------------------- x86 ---------------------------------- */

#elif defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(i386)  || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL)
    #error cmake_ARCH x86

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64) || defined(__amd64__) || defined(_M_AMD64)
    #error cmake_ARCH amd64

// AMD wrote the original x86_64 bit spec after cloning x86 for years.
// Then, in 2003, they renamed it to amd64. So basically x64 ~ amd64 ~ x86_64
// But the trend seems to be to tidy this up and go with the most officialish, amd64.
// Think of it this way, AMD was in fact the one pushing the architecture forwards at times.
// Even if Intel invented it. Therefore, I say, let's stick with amd64 and be over this mess already.

/* ------------------------------- Itanium ---------------------------------- */

#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64) || defined(_IA64) || defined(__IA64__) || defined(__itanium__)
    /* 32-bit mode on Itanium */
    #if !defined(__LP64__) && !defined(_M_X64) /* <-- Visual Studio macro (it USED to support Itanium) */
        #error cmake_ARCH ia32
    #else
        #error cmake_ARCH ia64
    #endif

/* ------------------------------ Power PC  --------------------------------- */

#elif defined(__ppc__) || defined(__ppc) || defined(__PPC__)  || defined(__PPC) || defined(__powerpc__) || defined(__powerpc) || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC) || defined(_M_MPPC) || defined(_M_PPC) || defined(__POWERPC__)
    #if defined(__ppc64__) || defined(__PPC64__) || defined(__powerpc64__) || defined(__64BIT__) 
        #error cmake_ARCH ppc64
    #else
        #error cmake_ARCH ppc32
    #endif

/* ---------------------------------- Sparc --------------------------------- */

#elif defined(__sparc) && !defined(__arch64__) || defined(__sparcv8)
    #error cmake_ARCH sparc32

#elif defined(__sparc__) && defined(__arch64__) || defined (__sparcv9)
    #error cmake_ARCH sparc64

/* --------------------------- IBM Mainframes ------------------------------- */

/* 64 bit IBM z/Architecture */
#elif defined(__s390x__) || defined(__s390X__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
    #error cmake_ARCH zArch

/* Older 32 bit IBM System/390, maybe useful for Hercules emulation? */
#elif defined(__s390__)
    #error cmake_ARCH s390

/* --------------------------- Hitachi SuperH ------------------------------- */

#elif defined(__sh1__)
    #error cmake_ARCH superH1
#elif defined(__sh2__)
    #error cmake_ARCH superH2
#elif defined(__sh3__) || defined(__SH3__)
    #error cmake_ARCH superH3
#elif defined(__SH4__)
    #error cmake_ARCH superH4
#elif defined(__SH5__)
    #error cmake_ARCH superH5
#elif defined(__sh__) // not sure which version...unlikely?
    #error cmake_ARCH superH

/* -------------------------- Analog Devices --------------------------------- */

#elif defined(__bfin) || defined(__BFIN__)
    #error cmake_ARCH blackfin
#elif defined(__ADSPTS101__)
    #error cmake_ARCH adps-ts101
#elif defined(__ADSPTS201__)
    #error cmake_ARCH adps-ts201
#elif defined(__ADSPTS202__)
    #error cmake_ARCH adps-ts202
#elif defined(__ADSPTS203__)
    #error cmake_ARCH adps-ts203
#elif defined(__ADSPTS20x__) // this will be true for any of the above but we use as  catchall
    #error cmake_ARCH adps-ts20x

/* ------------------------------- Alpha ------------------------------------- */

#elif defined(__alpha_ev4__)
    #error cmake_ARCH alpha_ev4
#elif defined(__alpha_ev5__)
    #error cmake_ARCH alpha_ev5
#elif defined(__alpha_ev6__)
    #error cmake_ARCH alpha_ev6
#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
    #error cmake_ARCH alpha

/* ------------------------- Texas Instruments ------------------------------ */

#elif defined(_TMS320C6200)
    #error cmake_ARCH c6200
#elif defined(_TMS320C6400)
    #error cmake_ARCH c6400
#elif defined(_TMS320C6400_PLUS)
    #error cmake_ARCH c6400+
#elif defined(_TMS320C6600)
    #error cmake_ARCH c6600
#elif defined(_TMS320C6700)
    #error cmake_ARCH c6700
#elif defined(_TMS320C6700_PLUS)
    #error cmake_ARCH c6700+
#elif defined(_TMS320C6740)
    #error cmake_ARCH c6740
#elif defined(__TMS470__)
    #error cmake_ARCH tms470

/* -------------------------- Hewlett Packard ------------------------------ */

#elif defined(_PA_RISC1_0)
    #error cmake_ARCH parisc1_0

#elif defined(_PA_RISC1_1) || defined(__HPPA11__) || defined(__PA7100__)
    #error cmake_ARCH parisc1_1

#elif defined(_PA_RISC2_0) || defined(__RISC2_0__) || defined(__HPPA20__) || defined(__PA8000__)
    #error cmake_ARCH parisc2_0

#elif defined(__hppa__) || defined(__HPPA__) || defined(__hppa)
    #error cmake_ARCH parisc

/* ---------------------------- Adapteva --------------------------------- */

#elif defined(__epiphany__)
    #error cmake_ARCH ephiphany

/* ------------------------------ MIPS  ---------------------------------- */

#elif defined(_MIPS_ISA_MIPS1) || (__mips == 1)
    #error cmake_ARCH mips1
#elif defined(_MIPS_ISA_MIPS2) || defined(__MIPS_ISA2__) || (__mips == 2)
    #error cmake_ARCH mips2
#elif defined(_MIPS_ISA_MIPS3) || defined(__MIPS_ISA3__) || (__mips == 3)
    #error cmake_ARCH mips3
#elif defined(_MIPS_ISA_MIPS4) || defined(__MIPS_ISA4__) || (__mips == 4)
    #error cmake_ARCH mips4
#elif defined(__MIPS__) || defined(__mips__) || defined(mips) || defined(__mips)
    #error cmake_ARCH mips

/* ---------------------------- Motorola  -------------------------------- */

#elif defined(__mc68000__) || defined(__MC68000__)
    #error cmake_ARCH mc68000
#elif defined(__mc68010__)
    #error cmake_ARCH mc68010
#elif defined(__mc68020__) || defined(__MC68020__)
    #error cmake_ARCH mc68020
#elif defined(__mc68030__) || defined(__MC68030__)
    #error cmake_ARCH mc68030
#elif defined(__mc68040__) || defined(__MC68040__)
    #error cmake_ARCH mc68040
#elif defined(__mc68060__) || defined(__MC68060__)
    #error cmake_ARCH mc68060
#elif defined(__m68k__) || defined(M68000) || defined(__MC68K__)
    #error cmake_ARCH mc68

#endif
")

if(APPLE AND CMAKE_OSX_ARCHITECTURES)
    # On OS X we use CMAKE_OSX_ARCHITECTURES *if* it was set
    # First let's normalize the order of the values

    # Note that it's not possible to compile PowerPC applications if you are using
    # the OS X SDK version 10.6 or later - you'll need 10.4/10.5 for that, so we
    # disable it by default
    # See this page for more information:
    # http://stackoverflow.com/questions/5333490/how-can-we-restore-ppc-ppc64-as-well-as-full-10-4-10-5-sdk-support-to-xcode-4

    # Architecture defaults to x86  or ppc on OS X 10.5 and earlier, depending on the CPU type detected at runtime.
    # On OS X 10.6+ the default is x86_64 if the CPU supports it, x86 otherwise.

    foreach(osx_arch ${CMAKE_OSX_ARCHITECTURES})
        if("${osx_arch}" STREQUAL "ppc" AND ppc_support)
            set(osx_arch_ppc TRUE)
        elseif("${osx_arch}" STREQUAL "i386")
            set(osx_arch_i386 TRUE)
        elseif("${osx_arch}" STREQUAL "x86_64")
            set(osx_arch_x86_64 TRUE)
        elseif("${osx_arch}" STREQUAL "ppc64" AND ppc_support)
            set(osx_arch_ppc64 TRUE)
        else()
            message(FATAL_ERROR "Invalid OS X arch name: ${osx_arch}")
        endif()
    endforeach()

    # Now add all the architectures in our normalized order
    if(osx_arch_ppc)
        list(APPEND ARCH ppc32)
    endif()

    if(osx_arch_i386)
        list(APPEND ARCH x86)
    endif()

    if(osx_arch_x86_64)
        list(APPEND ARCH x64)
    endif()

    if(osx_arch_ppc64)
        list(APPEND ARCH ppc64)
    endif()
else()
    file(WRITE "${CMAKE_BINARY_DIR}/CMakeFiles/detect_cpu_arch.c" "${archdetect_c_code}")

    # Detect the architecture in a rather creative way...
    # This compiles a small C program which is a series of ifdefs that selects a
    # particular #error preprocessor directive whose message string contains the
    # target architecture. The program will always fail to compile (both because
    # file is not a valid C program, and obviously because of the presence of the
    # #error preprocessor directives... but by exploiting the preprocessor in this
    # way, we can detect the correct target architecture even when cross-compiling,
    # since the program itself never needs to be run (only the compiler/preprocessor)
    try_run(
        run_result_unused
        compile_result_unused
        "${CMAKE_BINARY_DIR}/CMakeFiles"
        "${CMAKE_BINARY_DIR}/CMakeFiles/detect_cpu_arch.c"
        COMPILE_OUTPUT_VARIABLE ARCH
        CMAKE_FLAGS CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    )

    # Parse the architecture name from the compiler output
    string(REGEX MATCH "cmake_ARCH ([a-zA-Z0-9_]+)" ARCH "${ARCH}")

    # Get rid of the value marker leaving just the architecture name
    string(REPLACE "cmake_ARCH " "" ARCH "${ARCH}")

    # If we are compiling with an unknown architecture this variable should
    # already be set to "unknown" but in the case that it's empty (i.e. due
    # to a typo in the code), then set it to unknown
    if (NOT ARCH)
        set(ARCH unknown)
    endif()
endif()

set(CMAKE_CPU_ARCHITECTURES "${ARCH}")

endif(NOT detect_cpu_architectures_INC) # include guard
