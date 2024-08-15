include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(vkl_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(vkl_setup_options)
  option(vkl_ENABLE_HARDENING "Enable hardening" ON)
  option(vkl_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    vkl_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    vkl_ENABLE_HARDENING
    OFF)

  vkl_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR vkl_PACKAGING_MAINTAINER_MODE)
    option(vkl_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(vkl_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(vkl_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(vkl_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(vkl_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(vkl_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(vkl_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(vkl_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(vkl_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(vkl_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(vkl_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(vkl_ENABLE_PCH "Enable precompiled headers" OFF)
    option(vkl_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(vkl_ENABLE_IPO "Enable IPO/LTO" ON)
    option(vkl_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(vkl_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(vkl_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(vkl_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(vkl_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(vkl_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(vkl_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(vkl_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(vkl_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(vkl_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(vkl_ENABLE_PCH "Enable precompiled headers" OFF)
    option(vkl_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      vkl_ENABLE_IPO
      vkl_WARNINGS_AS_ERRORS
      vkl_ENABLE_USER_LINKER
      vkl_ENABLE_SANITIZER_ADDRESS
      vkl_ENABLE_SANITIZER_LEAK
      vkl_ENABLE_SANITIZER_UNDEFINED
      vkl_ENABLE_SANITIZER_THREAD
      vkl_ENABLE_SANITIZER_MEMORY
      vkl_ENABLE_UNITY_BUILD
      vkl_ENABLE_CLANG_TIDY
      vkl_ENABLE_CPPCHECK
      vkl_ENABLE_COVERAGE
      vkl_ENABLE_PCH
      vkl_ENABLE_CACHE)
  endif()

  vkl_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (vkl_ENABLE_SANITIZER_ADDRESS OR vkl_ENABLE_SANITIZER_THREAD OR vkl_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(vkl_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(vkl_global_options)
  if(vkl_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    vkl_enable_ipo()
  endif()

  vkl_supports_sanitizers()

  if(vkl_ENABLE_HARDENING AND vkl_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR vkl_ENABLE_SANITIZER_UNDEFINED
       OR vkl_ENABLE_SANITIZER_ADDRESS
       OR vkl_ENABLE_SANITIZER_THREAD
       OR vkl_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${vkl_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${vkl_ENABLE_SANITIZER_UNDEFINED}")
    vkl_enable_hardening(vkl_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(vkl_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(vkl_warnings INTERFACE)
  add_library(vkl_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  vkl_set_project_warnings(
    vkl_warnings
    ${vkl_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(vkl_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    vkl_configure_linker(vkl_options)
  endif()

  include(cmake/Sanitizers.cmake)
  vkl_enable_sanitizers(
    vkl_options
    ${vkl_ENABLE_SANITIZER_ADDRESS}
    ${vkl_ENABLE_SANITIZER_LEAK}
    ${vkl_ENABLE_SANITIZER_UNDEFINED}
    ${vkl_ENABLE_SANITIZER_THREAD}
    ${vkl_ENABLE_SANITIZER_MEMORY})

  set_target_properties(vkl_options PROPERTIES UNITY_BUILD ${vkl_ENABLE_UNITY_BUILD})

  if(vkl_ENABLE_PCH)
    target_precompile_headers(
      vkl_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(vkl_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    vkl_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(vkl_ENABLE_CLANG_TIDY)
    vkl_enable_clang_tidy(vkl_options ${vkl_WARNINGS_AS_ERRORS})
  endif()

  if(vkl_ENABLE_CPPCHECK)
    vkl_enable_cppcheck(${vkl_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(vkl_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    vkl_enable_coverage(vkl_options)
  endif()

  if(vkl_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(vkl_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(vkl_ENABLE_HARDENING AND NOT vkl_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR vkl_ENABLE_SANITIZER_UNDEFINED
       OR vkl_ENABLE_SANITIZER_ADDRESS
       OR vkl_ENABLE_SANITIZER_THREAD
       OR vkl_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    vkl_enable_hardening(vkl_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
