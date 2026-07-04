# Firmware Overview

## Purpose

The firmware in this repository provides the functional behavior of the handheld button controller. The code is preserved as the primary product logic and is not rewritten here; instead, this document describes its role in the broader hardware system.

## Architecture

The firmware is organized around:

- Display control for the SH1106 OLED
- LED matrix animation and frame updates
- Button scanning with debouncing
- Audio output via the passive buzzer
- Menu and game-state handling

## GPIO Mapping

The current firmware uses the following general pin allocation:

| Function | GPIO |
| --- | --- |
| Control button | 23 |
| Top-left button | 27 |
| Bottom-left button | 26 |
| Top-right button | 33 |
| Bottom-right button | 32 |
| LED matrix data | 14 |
| Buzzer | 25 |
| OLED SCL | 21 |
| OLED SDA | 22 |

## Driver Notes

- OLED output is handled through the U8g2 library.
- LED effects are driven through the NeoPixel-compatible matrix interface.
- Buttons use input pull-up logic and debouncing.
- Audio feedback is implemented through simple tone output.

## Build and Flash

The main firmware source is [../../firmware/LED_button.ino](../../firmware/LED_button.ino).

1. Open the firmware file in Arduino IDE or PlatformIO.
2. Select the ESP32 board package.
3. Compile and upload the sketch.
4. Confirm display power, LED output, and button response.

## Power Considerations

The firmware assumes a stable regulated supply to the ESP32 and peripherals. Battery voltage should be verified before deployment.

<!-- Add firmware flow chart or state diagram later -->