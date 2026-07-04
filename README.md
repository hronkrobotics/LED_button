# HRONK Robotics LED Button

## Project Overview

This repository documents a modular ESP32-based LED button controller designed for rapid prototyping, debugging, and community experimentation. The project combines a custom PCB, modular JST-XH wiring, an OLED display, an 8x8 addressable LED matrix, mechanical switches, and a portable battery system into a compact handheld device.

The firmware in this repository remains the primary functional layer. This repository now adds the engineering documentation, mechanical references, PCB files, manufacturing notes, and assembly guidance required to build, service, and improve the hardware.

## Demonstration Videos

The original demo recordings are preserved in [media/led-button-timer.mov](media/led-button-timer.mov) and [media/led-button-interface.mov](media/led-button-interface.mov).

## Features

- ESP32-based control platform
- 1.3-inch SH1106 OLED display
- 8x8 flexible WS2812B-compatible LED matrix
- Four Cherry MX Blue switches for primary input
- Two auxiliary momentary pushbuttons
- Passive piezo buzzer for audio feedback
- Dual 18650 battery system with external charging module
- Modular JST-XH connector architecture for easy assembly and replacement
- Firmware support for timer and reaction-style game modes

## Future UI and Connectivity Enhancements

Planned improvements for the next hardware and firmware iteration include:

- Wi-Fi connectivity for configuration, telemetry, and remote updates
- BLE support for phone or tablet pairing and control
- Multi-button serial connection for linking multiple units into a shared interactive setup

## Hardware Specifications

- Main controller: ESP32-WROOM-32
- Display: SH1106 OLED, 128x64
- LED matrix: 8x8 addressable LED panel
- Input: 4x Cherry MX Blue switches, 2x auxiliary buttons
- Power: 2x 18650 cells in a dual-cell holder/shield
- Charging: USB-C via external extension and battery shield
- Enclosure: 3D-printed housing with PCB standoffs and switch mounts

## Repository Structure

- [firmware](firmware/): firmware source and build assets
- [hardware](hardware/): PCB, CAD, manufacturing, and drawing files
- [docs](docs/): assembly, electronics, firmware, manufacturing, and add-on documentation
- [BOM](BOM/): bill of materials files and supporting documents

## Required Tools

- Arduino IDE or PlatformIO
- USB cable for firmware flashing
- Soldering iron and solder
- JST-XH crimping or soldering tools
- Hex drivers, screwdrivers, wire strippers, and flush cutters
- Multimeter for continuity and power testing

## Flashing Firmware

1. Open the firmware source in [firmware/LED_button.ino](firmware/LED_button.ino).
2. Select the appropriate ESP32 board profile in your toolchain.
3. Compile and upload the sketch.
4. Verify OLED initialization, button response, and LED output.

## Manufacturing and Assembly

See [docs/manufacturing/manufacturing-guide.md](docs/manufacturing/manufacturing-guide.md) for build instructions and [docs/assembly/assembly-guide.md](docs/assembly/assembly-guide.md) for mechanical assembly steps.

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

## Credits

- Firmware and prototype development: HRONK Robotics
- Hardware documentation and repository organization: HRONK Robotics
- Mechanical and electronic design references: included project files in this repository
- Wall-mount accessory reference: [hardware/cad/LED_BUTTON_VEX_WALL_MOUNT_3D_FILES.step](hardware/cad/LED_BUTTON_VEX_WALL_MOUNT_3D_FILES.step)

<!-- Add CAD renders, exploded views, and photographs here later -->