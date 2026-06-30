cmake_minimum_required(VERSION 3.14)

project(c-library-system
    VERSION     1.0.0
    DESCRIPTION "Console-based C99 library management system"
    LANGUAGES   C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Code-coverage instrumentation (Linux / macOS only)
option(ENABLE_COVERAGE "Enable gcov/lcov code coverage" OFF)
if(ENABLE_COVERAGE)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "ENABLE_COVERAGE requires GCC or Clang")
    endif()
    add_compile_options(--coverage -O0 -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

# AddressSanitizer (ASan) — detect memory errors at runtime
option(ENABLE_ASAN "Enable AddressSanitizer (GCC/Clang)" OFF)
if(ENABLE_ASAN)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "ENABLE_ASAN requires GCC or Clang")
    endif()
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -O1)
    add_link_options(-fsanitize=address)
endif()

# UndefinedBehaviorSanitizer (UBSan)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer (GCC/Clang)" OFF)
if(ENABLE_UBSAN)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "ENABLE_UBSAN requires GCC or Clang")
    endif()
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

# Export compile_commands.json for clangd / IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------
# Core library (shared between the main executable and test suites)
# -----------------------------------------------------------------
add_library(lib_core STATIC
    src/library.c
    src/librarian.c
    src/user.c
    src/utility.c
)
target_include_directories(lib_core PUBLIC src)

# -----------------------------------------------------------------
# Main executable
# -----------------------------------------------------------------
add_executable(library src/main.c)
target_link_libraries(library PRIVATE lib_core)
target_compile_definitions(library PRIVATE
    PROJECT_VERSION="${PROJECT_VERSION}"
)

# -----------------------------------------------------------------
# Install target — `cmake --install build` or `make install`
# -----------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS library RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_DATADIR}/c-library-system)

# -----------------------------------------------------------------
# Unit tests — Unity fetched from GitHub at configure time
# -----------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG        v2.6.0
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(unity)

    enable_testing()

    foreach(suite library user librarian utility)
        add_executable(test_${suite} tests/test_${suite}.c)
        target_include_directories(test_${suite} PRIVATE src)
        target_link_libraries(test_${suite} PRIVATE lib_core unity)
        add_test(NAME test_${suite} COMMAND test_${suite})
    endforeach()
endif()
