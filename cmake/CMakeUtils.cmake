include(CMakeParseArguments)

macro(jstd_check_environment_variables)
  foreach(_var ${ARGN})
    if(" ${${_var}}" STREQUAL " " AND DEFINED ENV{${_var}})
      set(__value "$ENV{${_var}}")
      file(TO_CMAKE_PATH "${__value}" __value) # Assume that we receive paths
      set(${_var} "${__value}")
      message(STATUS "Update variable ${_var} from environment: ${${_var}}")
    endif()
  endforeach()
endmacro()

macro(jstd_path_join result_var P1 P2_)
  string(REGEX REPLACE "^[/]+" "" P2 "${P2_}")
  if("${P1}" STREQUAL "" OR "${P1}" STREQUAL ".")
    set(${result_var} "${P2}")
  elseif("${P1}" STREQUAL "/")
    set(${result_var} "/${P2}")
  elseif("${P2}" STREQUAL "")
    set(${result_var} "${P1}")
  else()
    set(${result_var} "${P1}/${P2}")
  endif()
  string(REGEX REPLACE "([/\\]?)[\\.][/\\]" "\\1" ${result_var} "${${result_var}}")
  if("${${result_var}}" STREQUAL "")
    set(${result_var} ".")
  endif()
  #message(STATUS "'${P1}' '${P2_}' => '${${result_var}}'")
endmacro()

macro(jstd_check_compiler_flag LANG FLAG RESULT)
  set(_fname "${ARGN}")
  if(NOT DEFINED ${RESULT})
    if(_fname)
      # nothing
    elseif("_${LANG}_" MATCHES "_CXX_")
      set(_fname "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx")
      if("${CMAKE_CXX_FLAGS} ${FLAG} " MATCHES "-Werror " OR "${CMAKE_CXX_FLAGS} ${FLAG} " MATCHES "-Werror=unknown-pragmas ")
        FILE(WRITE "${_fname}" "int main() { return 0; }\n")
      else()
        FILE(WRITE "${_fname}" "#pragma\nint main() { return 0; }\n")
      endif()
    elseif("_${LANG}_" MATCHES "_C_")
      set(_fname "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.c")
      if("${CMAKE_C_FLAGS} ${FLAG} " MATCHES "-Werror " OR "${CMAKE_C_FLAGS} ${FLAG} " MATCHES "-Werror=unknown-pragmas ")
        FILE(WRITE "${_fname}" "int main(void) { return 0; }\n")
      else()
        FILE(WRITE "${_fname}" "#pragma\nint main(void) { return 0; }\n")
      endif()
    elseif("_${LANG}_" MATCHES "_OBJCXX_")
      set(_fname "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.mm")
      if("${CMAKE_CXX_FLAGS} ${FLAG} " MATCHES "-Werror " OR "${CMAKE_CXX_FLAGS} ${FLAG} " MATCHES "-Werror=unknown-pragmas ")
        FILE(WRITE "${_fname}" "int main() { return 0; }\n")
      else()
        FILE(WRITE "${_fname}" "#pragma\nint main() { return 0; }\n")
      endif()
    else()
      unset(_fname)
    endif()
    if(_fname)
      if(NOT "x${ARGN}" STREQUAL "x")
        file(RELATIVE_PATH __msg "${CMAKE_SOURCE_DIR}" "${ARGN}")
        set(__msg " (check file: ${__msg})")
      else()
        set(__msg "")
      endif()
      MESSAGE(STATUS "Performing Test ${RESULT}${__msg}")
      TRY_COMPILE(${RESULT}
        "${CMAKE_BINARY_DIR}"
        "${_fname}"
        CMAKE_FLAGS "-DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}"   # CMP0056 do this on new CMake
        COMPILE_DEFINITIONS "${FLAG}"
        OUTPUT_VARIABLE OUTPUT)

      if(${RESULT})
        string(REPLACE ";" "," OUTPUT_LINES "${OUTPUT}")
        string(REPLACE "\n" ";" OUTPUT_LINES "${OUTPUT_LINES}")
        foreach(_regex ${OCV_COMPILER_FAIL_REGEX})
          if(NOT ${RESULT})
            break()
          endif()
          foreach(_line ${OUTPUT_LINES})
            if("${_line}" MATCHES "${_regex}")
              file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
                  "Build output check failed:\n"
                  "    Regex: '${_regex}'\n"
                  "    Output line: '${_line}'\n")
              set(${RESULT} 0)
              break()
            endif()
          endforeach()
        endforeach()
      endif()

      if(${RESULT})
        set(${RESULT} 1 CACHE INTERNAL "Test ${RESULT}")
        message(STATUS "Performing Test ${RESULT} - Success")
      else(${RESULT})
        message(STATUS "Performing Test ${RESULT} - Failed")
        set(${RESULT} "" CACHE INTERNAL "Test ${RESULT}")
        file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
            "Compilation failed:\n"
            "    source file: '${_fname}'\n"
            "    check option: '${FLAG}'\n"
            "===== BUILD LOG =====\n"
            "${OUTPUT}\n"
            "===== END =====\n\n")
      endif(${RESULT})
    else()
      set(${RESULT} 0)
    endif()
  endif()
endmacro()

macro(jstd_check_flag_support lang flag varname base_options)
  if(CMAKE_BUILD_TYPE)
    set(CMAKE_TRY_COMPILE_CONFIGURATION ${CMAKE_BUILD_TYPE})
  endif()

  if("_${lang}_" MATCHES "_CXX_")
    set(_lang CXX)
  elseif("_${lang}_" MATCHES "_C_")
    set(_lang C)
  elseif("_${lang}_" MATCHES "_OBJCXX_")
    set(_lang OBJCXX)
  else()
    set(_lang ${lang})
  endif()

  string(TOUPPER "${flag}" ${varname})
  string(REGEX REPLACE "^(/|-)" "HAVE_${_lang}_" ${varname} "${${varname}}")
  string(REGEX REPLACE " -|-|=| |\\." "_" ${varname} "${${varname}}")

  jstd_check_compiler_flag("${_lang}" "${base_options} ${flag}" ${${varname}} ${ARGN})
endmacro()

# Status report function.
# Automatically align right column and selects text based on condition.
# Usage:
#   status(<text>)
#   status(<heading> <value1> [<value2> ...])
#   status(<heading> <condition> THEN <text for TRUE> ELSE <text for FALSE> )
function(status text)
  set(status_cond)
  set(status_then)
  set(status_else)

  set(status_current_name "cond")
  foreach(arg ${ARGN})
    if(arg STREQUAL "THEN")
      set(status_current_name "then")
    elseif(arg STREQUAL "ELSE")
      set(status_current_name "else")
    else()
      list(APPEND status_${status_current_name} ${arg})
    endif()
  endforeach()

  if(DEFINED status_cond)
    set(status_placeholder_length 32)
    string(RANDOM LENGTH ${status_placeholder_length} ALPHABET " " status_placeholder)
    string(LENGTH "${text}" status_text_length)
    if(status_text_length LESS status_placeholder_length)
      string(SUBSTRING "${text}${status_placeholder}" 0 ${status_placeholder_length} status_text)
    elseif(DEFINED status_then OR DEFINED status_else)
      ocv_output_status("${text}")
      set(status_text "${status_placeholder}")
    else()
      set(status_text "${text}")
    endif()

    if(DEFINED status_then OR DEFINED status_else)
      if(${status_cond})
        string(REPLACE ";" " " status_then "${status_then}")
        string(REGEX REPLACE "^[ \t]+" "" status_then "${status_then}")
        ocv_output_status("${status_text} ${status_then}")
      else()
        string(REPLACE ";" " " status_else "${status_else}")
        string(REGEX REPLACE "^[ \t]+" "" status_else "${status_else}")
        ocv_output_status("${status_text} ${status_else}")
      endif()
    else()
      string(REPLACE ";" " " status_cond "${status_cond}")
      string(REGEX REPLACE "^[ \t]+" "" status_cond "${status_cond}")
      ocv_output_status("${status_text} ${status_cond}")
    endif()
  else()
    ocv_output_status("${text}")
  endif()
endfunction()
