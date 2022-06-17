CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

##
## CMake is v3.10.2 under Ubuntu 18.04.2, is v3.16.3 under Ubuntu 20.04.
##

set(PROJECT_MAJOR_VERSION "1")
set(PROJECT_MINOR_VERSION "0")
set(PROJECT_PATCH_VERSION "0")
set(PROJECT_VERSION_STRING "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.0)
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

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "----------------- Build Env -------------------")
message(STATUS "CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "-----------------------------------------------")
message("")

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

include(CMakeASM_NASMInformation)
include(DetectCPUArchitectures)

enable_language(ASM_NASM)

if (CMAKE_ASM_NASM_COMPILER_LOADED)
    set(CAN_USE_ASSEMBLER TRUE)
endif()

if (CMAKE_CL_64)
    set(CMAKE_CL_ARCH x64)
else()
    set(CMAKE_CL_ARCH x86)
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

include(GetOSEnv)
include(CheckComplierEnv)
include(GetCompilerToolset)

GetOSEnv()
CheckCompilerEnvironment()
GetCompilerToolset(CMAKE_C_TOOLSET   "c")
GetCompilerToolset(CMAKE_CXX_TOOLSET "cxx")

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
message(STATUS "-----------------------------------------------")
message("")

if (NOT MSVC)
    ## For C_FLAGS
    ## -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4a -msse4.1 -msse4.2 -mavx -mavx2 -mavx512vl -mavx512f
    set(CMAKE_C_FLAGS_DEFAULT "${CMAKE_C_FLAGS} -std=c99 -march=native -mtune=native -finput-charset=gbk -fPIC")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEFAULT} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEFAULT} -g -pg -D_DEBUG")

    ## For CXX_FLAGS
    ## -Wall -Werror -Wextra -Wno-format -Wno-unused-function
    ## -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4a -msse4.1 -msse4.2 -mavx -mavx2 -mavx512vl -mavx512f
    # -fexec-charset=gbk -finput-charset=gbk
    set(CMAKE_CXX_FLAGS_DEFAULT "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -mtune=native -finput-charset=gbk -fPIC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEFAULT} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEFAULT} -g -pg -D_DEBUG")
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

include_directories(include)
include_directories(src)
include_directories(bench/benchmark)
include_directories(bench/time_hash_map)
include_directories(test)
include_directories(3rd_party)
include_directories(3rd_party/jstd_hashmap/src)

set(JSTD_SOURCE_FILES
    3rd_party/jstd_hashmap/src/jstd/dll_main.cpp
)

set(BENCHMARK_SOURCE_FILES
    bench/benchmark/benchmark.cpp
)

set(TIME_HASH_MAP_SOURCE_FILES
    bench/time_hash_map/time_hash_map.cpp
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
        src/jstd/asm/asmlib.h
        src/jstd/asm/InstructionSet_x64.asm
        src/jstd/asm/utf8_decode_sse41_x64.asm
    )
elseif ("x86" STREQUAL "${CMAKE_CPU_ARCHITECTURES}" OR
    "${CMAKE_PLATFORM_ARCH}" STREQUAL "Arch32")
    set(UTF8_ENCODING_SOURCE_FILES ${UTF8_ENCODING_SOURCE_FILES}
        src/jstd/asm/asmlib.h
        src/jstd/asm/InstructionSet_x86.asm
        src/jstd/asm/utf8_decode_sse41_x86.asm
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

add_library(jstd STATIC ${JSTD_SOURCE_FILES})
set(EXTRA_LIBS ${EXTRA_LIBS} jstd)
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

## benchmark
add_executable(benchmark ${BENCHMARK_SOURCE_FILES})

if (NOT MSVC)
    # For gcc or clang warning setting
    target_compile_options(benchmark
        PUBLIC
            -Wall -Wno-unused-function -Wno-deprecated-declarations -Wno-unused-variable -Wno-deprecated -faligned-new
    )
else()
    # Warning level 3 and all warnings as errors
    target_compile_options(benchmark PUBLIC /W3 /WX)
endif()

target_link_libraries(benchmark
PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(benchmark
PUBLIC
    "${PROJECT_BINARY_DIR}"
    ${EXTRA_INCLUDES}
)

## time_hash_map
add_executable(time_hash_map ${TIME_HASH_MAP_SOURCE_FILES})

if (NOT MSVC)
    # For gcc or clang warning setting
    target_compile_options(time_hash_map
        PUBLIC
            -Wall -Wno-unused-function -Wno-deprecated-declarations -Wno-unused-variable -Wno-deprecated -faligned-new
    )
else()
    # Warning level 3 and all warnings as errors
    target_compile_options(time_hash_map PUBLIC /W3 /WX)
endif()

target_link_libraries(time_hash_map
PUBLIC
    ${EXTRA_LIBS}
)

target_include_directories(time_hash_map
PUBLIC
    "${PROJECT_BINARY_DIR}"
    ${EXTRA_INCLUDES}
)

# add_subdirectory(./test)
