# PCB Documentation

## Purpose

The custom PCB acts as the central interconnect platform for the LED button controller. It provides mechanical support for the ESP32 development board and routes power, ground, and GPIO signals to the OLED display, LED matrix, buttons, buzzer, and battery system.

## Design Notes

- The PCB is designed around modular JST-XH connectors.
- Power and ground are distributed to all subsystems through the PCB.
- The ESP32 development board is mounted as a removable module.
- The board is intended for debugging, prototype assembly, and easy maintenance.

## Key Interfaces

- OLED: I2C interface via JST connector
- LED matrix: single data line plus power and ground
- Switches: input lines with pull-ups in firmware
- Buzzer: PWM or simple digital output
- Battery shield: regulated 5 V power input

## Reference Files

- [LED_Button_PCB_Schematic.pdf](LED_Button_PCB_Schematic.pdf)
- [Gerber_LED_BUTTON_2026-07-04.zip](Gerber_LED_BUTTON_2026-07-04.zip)

## Placeholder Notes

- Connector pinout table to be expanded from the schematic once the final revision is verified.
- GPIO allocation table to be linked to the firmware mapping in [../docs/firmware/overview.md](../docs/firmware/overview.md).

<!-- Add PCB layout image or fabrication drawing here -->