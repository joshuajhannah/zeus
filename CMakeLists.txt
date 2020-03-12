cmake_minimum_required(VERSION 3.14)
project(zeus)

# ---- Options ---- #

# Option ENABLE_CODE_COVERAGE will build the code with --coverage.
# e.g. make sisl_coverage can be used to run all tests. (lcov required)
option(ENABLE_CODE_COVERAGE "Build with code coverage" OFF)

# Option ENABLE_TESTS will add the tests directories to the build
# e.g. make tests
option(ENABLE_TESTS "Build the tests" OFF)

# Option ENABLE_LTO will build the project link linker optimisation
option(ENABLE_LTO "Build with linker optimisation" OFF)

# Option ENABLE_DOXYGEN will generate documentation
# e.g. make sisl_doc will generate sisl libraries documentation
option(ENABLE_DOXYGEN "Generate Doxygen optimisation" OFF)

# Option ENABLE_BENCHMARKS to enable the benchmarks. Benchmarks are reccomended in release mode only
option(ENABLE_BENCHMARKS "Build including the benchmarks" OFF)

# ---- Options End ---- #

# ---- Version Generation ---- #
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)
# set (RELEASE_CANDIDATE 0) # Set this for rc

if (DEFINED RELEASE_CANDIDATE)
    set (RELEASE_CANDIDATE "-rc${RELEASE_CANDIDATE}")
endif()

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
#        "${CMAKE_CURRENT_BINARY_DIR}/version.h")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# ---- Version Generation End ---- #

# ---- General Settings ---- #

# Required packages
find_package(Threads REQUIRED)

# Enable verbose makefile output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include our custom cmake functions
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Log)

# ---- General Settings End ---- #

# ---- Compiler Settings ---- #
# Configure the compiler options
include(CompilerOptions)

# ---- Enable Testing ---- #
if(ENABLE_TESTS)
    enable_testing()
endif()

# ---- Default Output Build Location ---- #
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# ---- Coverage report ---- #
if (ENABLE_CODE_COVERAGE)
    # TODO include(CodeCoverage)

    #    # Add excluded directories
    #    set(COVERAGE_LCOV_EXCLUDES '/usr/include/*'
    #            '/usr/lib/*'
    #            '/opt/rh/devtoolset-7/*'
    #            'gtest'
    #            'gtest/*'
    #            )
    #    # Add coverage custom command
    #    setup_target_for_coverage_lcov(NAME deathstar_coverage)
endif()

# ---- Documentation Generation ---- #
if(ENABLE_DOXYGEN)
    find_package(Doxygen REQUIRED)

    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(deathstar_doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
endif()

# ---- Enable LTO ---- #
if (ENABLE_LTO)
    # Compile with LTO
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# ---- Log cmake info ---- #
cmake_log_info()

# ---- Subdirectories ---- #
add_subdirectory(third-party)
add_subdirectory(app)
add_subdirectory(lib)