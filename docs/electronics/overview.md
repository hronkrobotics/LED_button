# Electronics Overview

## Main Components

### ESP32-WROOM-32
The ESP32 is the main controller for the device. It handles UI rendering, button scanning, LED animation, buzzer output, and high-level game logic.

### SH1106 OLED Display
The OLED display provides status information, menus, battery and mode information, and simple user feedback. It is connected over a 4-wire JST-XH interface using VCC, GND, SDA, and SCL.

### Flexible 8x8 LED Matrix
The LED matrix is used for visual output such as games, animations, and status cues. It is connected through a 3-wire JST-XH connector carrying 5 V, GND, and a single data line.

### Dual 18650 Battery System
The portable power source consists of two 18650 cells mounted in a battery holder or shield. The system powers the ESP32 and peripherals through a regulated 5 V output from the battery shield.

### Battery Shield and Charging
The battery shield serves as the power-management module. It provides battery charging, protection, and regulated output. The USB-C extension allows charging without opening the enclosure.

### Cherry MX Switches
The four Cherry MX Blue switches are the main input devices. Each switch interfaces as a simple active-low input to the ESP32. Firmware uses internal pull-ups.

### Auxiliary Pushbuttons
The two auxiliary momentary buttons are used for system functions such as start, select, reset, or mode switching.

### Passive Buzzer
The passive buzzer provides simple audio feedback for button presses, mode changes, game events, and alarms. The firmware drives it with PWM or timed tone output.

### Custom PCB
The PCB serves as the interconnect backbone for the entire device. It distributes power and routes signals between the ESP32 module and each peripheral connector.

## JST-XH Connectors

The following connector plan is documented for the prototype build:

| Connector | Destination | Purpose |
| --- | --- | --- |
| OLED JST | SH1106 OLED | Display power and I2C interface |
| LED JST | 8x8 LED matrix | Power and data |
| Button JST x4 | Cherry MX switches | Input lines |
| Aux JST x2 | Mechanical pushbuttons | System input |
| Buzzer JST | Passive buzzer | Audio output |
| Battery JST | Battery shield / power module | Regulated 5 V input |

## Cross References

- See [../firmware/overview.md](../firmware/overview.md) for firmware-side GPIO mapping.
- See [../../hardware/pcb/README.md](../../hardware/pcb/README.md) for PCB-level connector context.

<!-- Add connector pinout diagram or schematic cross-reference here -->