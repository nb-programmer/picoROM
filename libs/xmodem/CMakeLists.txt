cmake_minimum_required(VERSION 3.20)

include("pico-sdk-import")

project(xmodem
    DESCRIPTION "XMODEM driver, taken from https://github.com/gfoot/picoprom and adapted as a library"
    LANGUAGES ASM C CXX
    VERSION 0.1
)

pico_sdk_init()

# Project sources

list(APPEND LIBRARY_SOURCES
    xmodem/xmodem.c
)

list(APPEND LIBRARY_HEADERS
    xmodem/xmodem.h
)

# Library target 
add_library(${PROJECT_NAME}
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        pico_stdlib
)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
