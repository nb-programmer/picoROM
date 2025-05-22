#include <cstring>
#include <stdio.h>

#include <xmodem/xmodem.h>

#include "utils.hpp"

#include "config.hpp"
#include "rom_bus.hpp"

using rom_bus::rom_contents;

void rom_transfer(const uint8_t *data, size_t data_size) {
    if (data_size > ROM_SIZE) {
        data_size = ROM_SIZE;
    }
    std::memcpy(rom_contents, data, data_size);
}

void setup_rom_contents() {
    // Fill memory with 0xff by default
    // Note: This can be set up with some default ROM content for the
    // target system
    std::memset(rom_contents, 0xFF, ROM_SIZE);
}

void rom_xmodem_xfer_blocking() {
    // Buffer to hold XMODEM rx data
    char buffer[ROM_SIZE];

    printf("Ready to program - Start XMODEM session and send data now.\n");
    printf("Maximum data size allowed: %zu bytes (excess will be ignored).\n",
           ROM_SIZE);

    // Wait (blocking) till we receive an XMODEM session with data
    int sizeReceived = xmodem_receive(buffer, ROM_SIZE, nullptr, nullptr);

    if (sizeReceived < 0) {
        xmodem_dumplog();
        printf("XMODEM transfer failed! See error log above.\n");
    }

    if (sizeReceived <= 0) {
        return;
    }

    // TODO: Support a /RESET line and pull it low during rom transfer

    printf("\n");
    printf("Transfer complete - received %d bytes\n", sizeReceived);
    printf("\n");
    rom_transfer((const uint8_t *)buffer, sizeReceived);
}
