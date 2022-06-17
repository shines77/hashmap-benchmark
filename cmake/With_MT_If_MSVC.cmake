#
# See: https://cloud.tencent.com/developer/article/1433525
#
# Use the static C library for all build types to a target.
# Set '/MT' option on the specified target during MSVC
# compilation to connect to static C/C++ library.
function (With_MT_If_MSVC target)
    if (MSVC)
        # Generator expressions
        set(_mt "/MT$<$<CONFIG:Debug>:d")
        get_target_property(_options ${target} COMPILE_OPTIONS)
        if (_options)
            # message(STATUS "${target} COMPILE_OPTIONS = ${_options}")
            if (${_options} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" _options "${_options}")
            else()
                set(_options "${_options} ${_mt}")
            endif()
        else()
            set(_options "${_mt}")
        endif()

        get_target_property(_type ${target} TYPE)

        # Determine whether ${target} is a static library
        if(_type STREQUAL "STATIC_LIBRARY")
            # Static library adds /MT option to INTERFACE_COMPILE_OPTIONS
            target_compile_options(${target} PUBLIC "${_options}")
        else()
            target_compile_options(${target} PRIVATE "${_options}")
        endif()

        # Cleanup temporary variables.
        unset(_mt)
        unset(_options)
        message(STATUS "target ${target} use static runtime /MT")
    endif(MSVC)
endfunction(With_MT_If_MSVC)
