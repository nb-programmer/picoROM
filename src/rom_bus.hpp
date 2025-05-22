#pragma once

#include <cstdint>

#include "config.hpp"

namespace rom_bus {
extern uint8_t rom_contents[ROM_SIZE];

void setup_gpio();
int get_requested_address();
void put_data_on_bus(int);
} // namespace rom_bus
