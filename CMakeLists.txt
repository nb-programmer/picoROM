cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "libs/")

include("common-config")
include("pico-sdk-import")

project(picoROM
    DESCRIPTION "Simulate a ROM chip (e.g. 28c256) with a Raspberry Pi Pico"
    LANGUAGES ASM C CXX
    VERSION 0.1
)

pico_sdk_init()

## Build sub-packages

# Build XMODEM driver
add_subdirectory(libs/xmodem)

## Build main application

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/rom_bus.cpp
    src/utils.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        pico_stdlib
        pico_multicore
        xmodem::xmodem
)

if (PICO_CYW43_SUPPORTED)
    message(STATUS "Including cyw43 driver library")
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            pico_cyw43_arch
    )
endif()

set(PICO_DEFAULT_BINARY_TYPE copy_to_ram)

# Enable USB output, disable UART output.
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})

# Slow down flash during boot for heavy overclocking.
pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=4)
pico_set_boot_stage2(${PROJECT_NAME} slower_boot2)

# Add some preprocessor defines
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_definitions(${CMAKE_PROJECT_NAME}
    PUBLIC PROJECT_TARGET_NAME="${PROJECT_TARGET_NAME}"
)
