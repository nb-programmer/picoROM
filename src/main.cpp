/**
 * PicoROM
 * Simulate a ROM chip (e.g. 28c256) with a Raspberry Pi Pico.
 * Nick Bild (nick.bild@gmail.com)
 * August 2021
 */

#include <stdio.h>

#include <hardware/clocks.h>
#include <hardware/vreg.h>
#include <pico/multicore.h>
#include <tusb.h>

#include "rom_bus.hpp"
#include "utils.hpp"

/**
 * Main entry-point for core1.
 *
 * Performs an infinite loop of reading address bus pins and
 * outputting the correct data byte from rom_contents to the
 * data bus.
 */
void core1_main() {
    // GPIO setup.
    rom_bus::setup_gpio();

    // Continually check address lines and
    // put associated data on bus.
    while (true)
        rom_bus::put_data_on_bus(rom_bus::get_requested_address());
}

/**
 * Main (core0) entry-point.
 *
 * Performs set-up of the system, launches core1 code
 * and handles XMODEM communication via serial (USB)
 */
int main() {
    // Specify contents of emulated ROM.
    setup_rom_contents();

    // Set system clock speed.
    // 400 MHz
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_pll(1600000000, 4, 1);

    // Launch core1 process that does the actual PROM emulation
    multicore_launch_core1(core1_main);

    stdio_init_all();
    while (!tud_cdc_connected())
        sleep_ms(100);

    printf("\nUSB Serial connected\n");

    while (true)
        rom_xmodem_xfer_blocking();
}
