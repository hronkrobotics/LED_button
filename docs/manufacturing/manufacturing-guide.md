# Manufacturing Guide

## Scope

This guide covers the prototype manufacturing workflow for the LED button controller. The design is intended as a modular, serviceable prototype rather than a highly optimized production design.

## Required Tools

- Soldering iron and solder
- JST-XH crimping or soldering tools
- Hex drivers
- Screwdrivers
- Flush cutters
- Wire strippers
- Multimeter

## PCB Manufacturing

- Use the provided Gerber archive in [../../hardware/pcb/Gerber_LED_BUTTON_2026-07-04.zip](../../hardware/pcb/Gerber_LED_BUTTON_2026-07-04.zip).
- Confirm board outline, copper layers, and drill files before fabrication.
- Review the schematic in [../../hardware/pcb/LED_Button_PCB_Schematic.pdf](../../hardware/pcb/LED_Button_PCB_Schematic.pdf).

## PCB Assembly

1. Populate the ESP32 footprint or mount the development board.
2. Install JST-XH headers and connectors.
3. Solder power and signal connections carefully.
4. Verify continuity between the PCB, battery shield, and peripherals.

## Cable Preparation

- Prepare JST-XH leads for the OLED, LED matrix, switches, buzzer, and battery module.
- Maintain consistent wire lengths and orientation.
- Label cables where possible to reduce assembly errors.

## Quality Checks

- Confirm no shorts between power and ground.
- Verify connector polarity and orientation.
- Confirm the OLED and LED matrix receive the correct supply rails.
- Validate button inputs and buzzer output after assembly.

## Prototype Design Philosophy

This repository represents the final modular prototype before a production-oriented revision. The design intentionally uses an ESP32 development board, an external battery module, JST-XH connectors, and modular wiring to maximize flexibility, debugging simplicity, and component replacement.

<!-- Add manufacturing checklist and process photos later -->