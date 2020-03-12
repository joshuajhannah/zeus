# library name
set(TARGET_NAME zeus_md)

# header files
set(HEADER_FILES
        include/md/book.h
        include/md/level.h
        include/md/types.h
        include/md/itch/types.h
        include/md/itch/feed.h
        )

# source files
set(SOURCE_FILES
        src/book.cpp
        )

# Add this as a library
add_library(${TARGET_NAME} STATIC "")

# Add target sources
target_sources(${TARGET_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Add include directories for this library
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add compiler options for this library
target_compile_options(${TARGET_NAME} PRIVATE ${DEFAULT_COPTS} ${EXCEPTIONS_FLAG})

# Link dependencies -> no dependencies for this lib
target_link_libraries(${TARGET_NAME} PUBLIC zeus_core)

# Do not decay cxx standard if not specified
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

# ---- Tests ---- #
if (ENABLE_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(test)
endif ()

# ---- Benchmarks ---- #
if (ENABLE_BENCHMARKS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/benchmark)
    add_subdirectory(benchmark)
endif ()