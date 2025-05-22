#pragma once

#include <cstddef>
#include <cstdint>

void rom_transfer(const uint8_t *data, size_t data_size);
void setup_rom_contents();
void rom_xmodem_xfer_blocking();
