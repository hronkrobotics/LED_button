# Wall-Mount Add-on

## Purpose

The wall-mount add-on is an optional accessory for the LED button platform. It is intended to allow the device to be installed on a wall or vertical surface as a display-style or interactive panel while preserving compatibility with the existing electronics.

## Reference Files

The wall-mount accessory is represented by the provided mechanical reference file:

- [hardware/cad/LED_BUTTON_VEX_WALL_MOUNT_3D_FILES.step](../../hardware/cad/LED_BUTTON_VEX_WALL_MOUNT_3D_FILES.step)

## Compatibility

- Compatible with the existing modular PCB and enclosure layout
- Intended for use with the current ESP32 firmware and button architecture
- Requires no change to core firmware behavior unless additional mounting or sensing features are introduced

## Installation

1. Confirm the wall-mount geometry is compatible with the current enclosure and mounting surface.
2. Install the mounting hardware and brackets using the specified fasteners.
3. Verify that switch travel, display clearance, and cable routing remain unobstructed.
4. Re-test the device after installation.

## Required Hardware

- Fasteners specified by the wall-mount design
- Mounting brackets or spacer hardware included with the accessory
- Appropriate wall anchors or screws for the target surface

## Firmware Compatibility

The base firmware should remain compatible. Additional firmware behavior should only be added if the mounting accessory introduces new sensing or actuation functions.

<!-- Add wall-mount render, exploded view, or installation photo later -->