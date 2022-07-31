CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

##
## CMake is v3.10.2 under Ubuntu 18.04.2, is v3.16.3 under Ubuntu 20.04.
##

set(PROJECT_MAJOR_VERSION "1")
set(PROJECT_MINOR_VERSION "0")
set(PROJECT_PATCH_VERSION "0")
set(PROJECT_VERSION_STRING "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.0")
    # Fixed CMP0048: https://github.com/Tencent/rapidjson/issues/1154
    cmake_policy(SET CMP0048 NEW)
    project(hashmap_benchmark VERSION "${PROJECT_VERSION_STRING}")
else()
    project(hashmap_benchmark)
    set(PROJECT_VERSION_MAJOR "${PROJECT_MAJOR_VERSION}")
    set(PROJECT_VERSION_MINOR "${PROJECT_MINOR_VERSION}")
    set(PROJECT_VERSION_PATCH "${PROJECT_PATCH_VERSION}")
    set(PROJECT_VERSION       "${PROJECT_VERSION_STRING}")
endif()

message("")
message(STATUS "-----------------------------------------------")
message(STATUS "  project: ${PROJECT_NAME}, version: ${PROJECT_VERSION_STRING}")
message(STATUS "-----------------------------------------------")
message("")

set(CMAKE_SOURCE_DIR    "${CMAKE_SOURCE_DIR}/../")
set(CMAKE_BINARY_DIR    "${CMAKE_BINARY_DIR}")
set(PROJECT_SOURCE_DIR  "${PROJECT_BINARY_DIR}/../")
set(PROJECT_BINARY_DIR  "${PROJECT_BINARY_DIR}")

message(STATUS "------------------ Build Env -------------------")
message(STATUS "CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "------------------------------------------------")
message("")

message(STATUS "------------------ Current Build Env -------------------")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR   : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR   : ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR     : ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "--------------------------------------------------------")
message("")

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

##
## See: https://blog.csdn.net/u012487272/article/details/12882283
##

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## Configuration value are DEBUG, RELEASE, MINSIZEREL and RELWITHDEBINFO.

set(CMAKE_RELESE_POSTFIX "")
set(CMAKE_DEBUG_POSTFIX  "d")

message(STATUS "--------------- Build Output ------------------")
message(STATUS "EXECUTABLE_OUTPUT_PATH : ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH    : ${LIBRARY_OUTPUT_PATH}")
message(STATUS "-----------------------------------------------")
message(STATUS "CMAKE_EXECUTABLE_OUTPUT_DIRECTORY : ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY    : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY    : ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY    : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "-----------------------------------------------")
message("")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

## include(CMakeASM_NASMInformation)
include(DetectCPUArchitectures)

if (0)

enable_language(ASM_NASM)

if (CMAKE_ASM_NASM_COMPILER_LOADED)
    set(CAN_USE_ASSEMBLER TRUE)
endif()

if (CMAKE_CL_64)
    set(CMAKE_CL_ARCH x64)
else()
    set(CMAKE_CL_ARCH x86)
endif()

endif()

##
## See: https://stackoverflow.com/questions/39258250/how-to-detect-if-64-bit-msvc-with-cmake
##
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    set(CMAKE_PLATFORM_ARCH Arch64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    set(CMAKE_PLATFORM_ARCH Arch32)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(JLANG_CMAKE_SHOW_DETAIL ON)

include(CMakeUtils)
include(GetOSEnv)
include(CheckComplierEnv)
include(GetCompilerToolset)
include(DetectCXXCompiler)

GetOSEnv()
CheckCompilerEnvironment()
GetCompilerToolset(CMAKE_C_TOOLSET   "c")
GetCompilerToolset(CMAKE_CXX_TOOLSET "cxx")

set(ipo_supported NO)
set(CMakeIPOEnable FALSE)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.9")
    ##
    ## Supported INTERPROCEDURAL_OPTIMIZATION since CMake 3.9, support for Visual Studio generators since CMake 3.13.
    ##
    cmake_policy(SET CMP0069 NEW)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
        cmake_policy(SET CMP0138 NEW)
    endif()

    ##
    ## https://stackoverflow.com/questions/31355692/how-do-i-enable-link-time-optimization-lto-with-cmake
    ##
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT error)

    if (ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        set(CMakeIPOEnable FALSE)
    else()
        message(STATUS "IPO/LTO is not supported: <${error}>")
    endif()
else()
    message(STATUS "CMake version is < 3.9, is not supported IPO/LTO.")
endif()

message(STATUS "-------------- Compiler Toolset ---------------")
message(STATUS "CMAKE_C_TOOLSET   : ${CMAKE_C_TOOLSET}")
message(STATUS "CMAKE_CXX_TOOLSET : ${CMAKE_CXX_TOOLSET}")
message(STATUS "-----------------------------------------------")
message("")

message(STATUS "--------------- Architectures -----------------")
message(STATUS "CMAKE_BUILD_TYPE         : ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CL_ARCH            : ${CMAKE_CL_ARCH}")
message(STATUS "CMAKE_PLATFORM_ARCH      : ${CMAKE_PLATFORM_ARCH}")
message(STATUS "CMAKE_CPU_ARCHITECTURES  : ${CMAKE_CPU_ARCHITECTURES}")
message(STATUS "CMAKE_IPO_SUPPORTED      : ${ipo_supported}")
message(STATUS "-----------------------------------------------")
message("")

##
## https://github.com/abseil/abseil-cpp/blob/master/CMake/README.md
## https://abseil.io/docs/cpp/tools/cmake-installs
##

# Abseil currently supports C++11, C++14, and C++17.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.8")
    set(ABSL_PROPAGATE_CXX_STD ON)
endif()

# Import Abseil's CMake targets
find_package(absl REQUIRED)

if (NOT MSVC)
    ## For C_FLAGS
    ## -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4a -msse4.1 -msse4.2 -mavx -mavx2 -mavx512vl -mavx512f
    set(CMAKE_C_COMPILER                "/usr/bin/clang")
    if (CMakeIPOEnable)
        set(CMAKE_C_FLAGS_DEFAULT       "${CMAKE_C_FLAGS} -std=c99 -march=native -finput-charset=utf-8 -flto=thin -fPIC -fpic")
    else()
        set(CMAKE_C_FLAGS_DEFAULT       "${CMAKE_C_FLAGS} -std=c99 -march=native -finput-charset=utf-8 -fPIC -fpic")
    endif()
    set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_DEFAULT} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEFAULT} -g -pg -D_DEBUG")
    set(CMAKE_C_FLAGS_MINSIZEREL        "${CMAKE_C_FLAGS_DEFAULT} -Os -DNDEBUG")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO    "${CMAKE_C_FLAGS_DEFAULT} -O2 -g")

    ## For CXX_FLAGS
    ## -Wall -Werror -Wextra -Wno-format -Wno-unused-function
    ## -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4a -msse4.1 -msse4.2 -mavx -mavx2 -mavx512vl -mavx512f
    # -fexec-charset=gbk -finput-charset=gbk
    set(CMAKE_CXX_COMPILER              "/usr/bin/clang++")
    if (CMakeIPOEnable)
        set(CMAKE_CXX_FLAGS_DEFAULT     "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -march=native -finput-charset=utf-8 -flto=thin -fPIC -fpic")
    else()
        set(CMAKE_CXX_FLAGS_DEFAULT     "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -march=native -finput-charset=utf-8 -fPIC -fpic")
    endif()
    set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_DEFAULT} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEFAULT} -g -pg -D_DEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL      "${CMAKE_CXX_FLAGS_DEFAULT} -Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_DEFAULT} -O2 -g")

    set(CMAKE_AR        "/usr/bin/llvm-ar")
    set(CMAKE_LINKER    "/usr/bin/llvm-link")
    set(CMAKE_NM        "/usr/bin/llvm-nm")
    set(CMAKE_OBJDUMP   "/usr/bin/llvm-objdump")
    set(CMAKE_RANLIB    "/usr/bin/llvm-ranlib")

    ##
    ## GCC C++ Lib: -lstdc++ stdlib=libstdc++, Clang C++ Lib: -lc++ -stdlib=libc++
    ##
    ## Clang C++ lib: libc++
    set(CMAKE_EXE_LINKER_FLAGS "-lc++ -stdlib=libc++")
    ## GCC C++ lib: GLibCxx
    ## set(CMAKE_EXE_LINKER_FLAGS "-lstdc++ stdlib=libstdc++")

    ##
    ## See: https://cloud.tencent.com/developer/ask/101584
    ##
    ## "/usr/bin/c++", "/usr/bin/g++", "/usr/bin/clang++"
    message("  CMAKE_C_COMPILER:   ${CMAKE_C_COMPILER}")
    message("  CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        # using Clang
        if (CONAN_LIBCXX STREQUAL "libstdc++11")
            add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
        elseif (CONAN_LIBCXX STREQUAL "libstdc++")
            add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
        endif()
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        # using GCC
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
        # using Intel C++
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        # using Visual Studio C++
    endif()

    ## -faligned-new: GCC >= 7.1, CLANG >= 4.0
    ## See: https://trac.wildfiregames.com/ticket/5139
    if (CMAKE_COMPILER_IS_CLANGCXX)
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "4.0")
            message("")
            message(STATUS "Clang >= 4.0, Added CXX_FLAGS: -faligned-new")
            message("")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-new")
        endif()
    elseif (CMAKE_COMPILER_IS_GNUCXX)
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "7.1")
            message("")
            message(STATUS "GCC >= 7.1, Added CXX_FLAGS: -faligned-new")
            message("")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-new")
        endif()
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -D_DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
else()
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DNDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif()

include(With_MT_If_MSVC)

if (MSVC AND MSVC_STATIC_CRT)
    set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELWITHDEBINFO
    )
    foreach (CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif()

if (WIN32)
    ## add_compile_options(-D__SSE3__ -D__SSE4A__ -D__SSE4_1__ -D__SSE4_2__)
    add_compile_options(-D_WIN32_WINNT=0x0601 -D_CRT_SECURE_NO_WARNINGS)
    set(EXTRA_LIBS ${EXTRA_LIBS} ws2_32 mswsock)
endif()

if (UNIX)
    set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
else()
    set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif()

include_directories(../include)
include_directories(../src)
include_directories(../test)
include_directories(../3rd_party)
include_directories(../3rd_party/jstd_hashmap/src)
include_directories(../3rd_party/emhash)
include_directories(../install/include)
## include_directories(../3rd_party/abseil-cpp)

# Process Abseil's CMake build system
add_subdirectory("../3rd_party/abseil-cpp" "${CMAKE_CURRENT_BINARY_DIR}/abseil-cpp")

set(JSTD_SOURCE_FILES
    ../3rd_party/jstd_hashmap/src/jstd/dll_main.cpp
)

# add_subdirectory(main EXCLUDE_FROM_ALL src/main/asm)

##
## See: https://stackoverflow.com/questions/39258250/how-to-detect-if-64-bit-msvc-with-cmake
##
if (0)
if ("amd64" STREQUAL "${CMAKE_CPU_ARCHITECTURES}" OR
    "x64" STREQUAL "${CMAKE_CPU_ARCHITECTURES}" OR
    "${CMAKE_PLATFORM_ARCH}" STREQUAL "Arch64")
    set(UTF8_ENCODING_SOURCE_FILES ${UTF8_ENCODING_SOURCE_FILES}
        ../src/jstd/asm/asmlib.h
        ../src/jstd/asm/InstructionSet_x64.asm
        ../src/jstd/asm/utf8_decode_sse41_x64.asm
    )
elseif ("x86" STREQUAL "${CMAKE_CPU_ARCHITECTURES}" OR
    "${CMAKE_PLATFORM_ARCH}" STREQUAL "Arch32")
    set(UTF8_ENCODING_SOURCE_FILES ${UTF8_ENCODING_SOURCE_FILES}
        ../src/jstd/asm/asmlib.h
        ../src/jstd/asm/InstructionSet_x86.asm
        ../src/jstd/asm/utf8_decode_sse41_x86.asm
    )
else()
    message("This platform does not support our asm code.")
endif()
endif()

## list(LENGTH ASM_SOURCE_FILES asm_files_length)
## if (${asm_files_length} GREATER 0)
##     ## STATIC, SHARED
##     add_library(asmlib STATIC ${ASM_SOURCE_FILES})
##     set(EXTRA_LIBS ${EXTRA_LIBS} asmlib)
##     set(EXTRA_INCLUDES ${EXTRA_INCLUDES} "${PROJECT_SOURCE_DIR}/src")
## endif()

## add_library(jstd STATIC ${JSTD_SOURCE_FILES})
## set(EXTRA_LIBS ${EXTRA_LIBS} jstd)
set(EXTRA_INCLUDES ${EXTRA_INCLUDES} "${PROJECT_SOURCE_DIR}/src")

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
)

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Switch CMAKE_BUILD_TYPE to Release"
)

add_subdirectory("../bench" "${CMAKE_CURRENT_BINARY_DIR}/bench")
