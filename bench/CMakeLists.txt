
##
## benchmark
##
set(BENCHMARK_SOURCE_FILES
    ../bench/benchmark/benchmark.cpp
)

add_executable(benchmark ${BENCHMARK_SOURCE_FILES})

if (NOT MSVC)
    # For gcc or clang warning setting
    target_compile_options(benchmark
        PUBLIC
            -Wall -Wno-unused-function -Wno-deprecated-declarations -Wno-unused-variable -Wno-deprecated
    )
else()
    # Warning level 3 and all warnings as errors
    target_compile_options(benchmark PUBLIC /W3 /WX)
endif()

# Enforce that your_lib requires C++17. Important to document for clients that they
# must set CMAKE_CXX_STANDARD to 17 or higher for proper Abseil ABI compatibility
# (since otherwise, Abseil library targets could be compiled with a lower C++
# standard than your_lib).

## target_compile_features(benchmark PUBLIC cxx_std_14)

target_link_libraries(benchmark
PUBLIC
    ${EXTRA_LIBS} absl::flat_hash_map absl::node_hash_map
)

target_include_directories(benchmark
PUBLIC
    "${PROJECT_SOURCE_DIR}/bench/time_hash_map"
    "${PROJECT_SOURCE_DIR}/install/include"
    "${PROJECT_BINARY_DIR}"
    ${EXTRA_INCLUDES}
)

##
## time_hash_map
##
set(TIME_HASH_MAP_SOURCE_FILES
    ../bench/time_hash_map/time_hash_map.cpp
)

add_executable(time_hash_map ${TIME_HASH_MAP_SOURCE_FILES})

if (NOT MSVC)
    # For gcc or clang warning setting
    target_compile_options(time_hash_map
        PUBLIC
            -Wall -Wno-unused-function -Wno-deprecated-declarations -Wno-unused-variable -Wno-deprecated
    )
else()
    # Warning level 3 and all warnings as errors
    target_compile_options(time_hash_map PUBLIC /W3 /WX)
endif()

# Enforce that your_lib requires C++17. Important to document for clients that they
# must set CMAKE_CXX_STANDARD to 17 or higher for proper Abseil ABI compatibility
# (since otherwise, Abseil library targets could be compiled with a lower C++
# standard than your_lib).

## target_compile_features(time_hash_map PUBLIC cxx_std_14)

target_link_libraries(time_hash_map
PUBLIC
    ${EXTRA_LIBS} absl::flat_hash_map absl::node_hash_map
)

target_include_directories(time_hash_map
PUBLIC
    "${PROJECT_SOURCE_DIR}/bench/time_hash_map"
    "${PROJECT_SOURCE_DIR}/install/include"
    "${PROJECT_BINARY_DIR}"
    ${EXTRA_INCLUDES}
)

##
## cardinal_bench
##
set(CARDINAL_BENCH_SOURCE_FILES
    ../bench/cardinal_bench/cardinal_bench.cpp
)

add_executable(cardinal_bench ${CARDINAL_BENCH_SOURCE_FILES})

if (NOT MSVC)
    # For gcc or clang warning setting
    target_compile_options(cardinal_bench
        PUBLIC
            -Wall -Wno-unused-function -Wno-deprecated-declarations -Wno-unused-variable -Wno-deprecated
    )
else()
    # Warning level 3 and all warnings as errors
    target_compile_options(cardinal_bench PUBLIC /W3 /WX)
endif()

# Enforce that your_lib requires C++17. Important to document for clients that they
# must set CMAKE_CXX_STANDARD to 17 or higher for proper Abseil ABI compatibility
# (since otherwise, Abseil library targets could be compiled with a lower C++
# standard than your_lib).

## target_compile_features(cardinal_bench PUBLIC cxx_std_14)

target_link_libraries(cardinal_bench
PUBLIC
    ${EXTRA_LIBS} absl::flat_hash_map absl::node_hash_map
)

target_include_directories(cardinal_bench
PUBLIC
    "${PROJECT_SOURCE_DIR}/bench/cardinal_bench"
    "${PROJECT_SOURCE_DIR}/install/include"
    "${PROJECT_BINARY_DIR}"
    ${EXTRA_INCLUDES}
)
