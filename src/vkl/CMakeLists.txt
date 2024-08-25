add_executable(Vkl main.cpp)

# Retrieve the target name
get_target_property(target_name Vkl NAME)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${target_name} PRIVATE -march=native)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target_name} PRIVATE -march=native)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
    target_compile_options(${target_name} PRIVATE /arch:AVX2)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
else ()
    message(STATUS "Impossible to set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif ()

target_link_libraries(
        Vkl
        PRIVATE vkl::vkl_options
        vkl::vkl_warnings)

target_link_system_libraries(
        Vkl
        PRIVATE
        fmt::fmt
        spdlog::spdlog
        vkl-lib
)

target_include_directories(Vkl PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
