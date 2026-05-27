# Fab Vine Node V1

V1 is the first unified firmware version for the Fab Vine node.

All physical nodes are expected to run the same base firmware. Differences between nodes should be handled through configuration, not separate firmware folders.

## Hardware Scope

This version is intended to bring together:

- OLED expression display
- Sensor inputs
- LED output behavior
- Network or communication logic
- Shared node state

## Folder Structure

- `firmware/` - main Arduino sketch for the V1 node.
- `modules/oled-expressions/` - OLED expression logic, face states, and drawing helpers.
- `modules/sensors/` - sensor reading and input processing.
- `modules/leds/` - LED behavior, status lights, and visual feedback.
- `modules/network/` - WiFi, MQTT, or other communication logic.
- `assets/oled/` - OLED bitmaps, sprites, icons, and visual assets.
- `notes/` - test notes, design decisions, and expression behavior ideas.

## Arduino Sketch

The main sketch is:

```text
firmware/fab-vine-node-v1/fab-vine-node-v1.ino
```

Arduino IDE expects the `.ino` file to live inside a folder with the same name.

## Current OLED Test

The current V1 sketch includes an OLED expression cycle with:

- Normal eyes
- Blink
- Wink
- Surprised
- Look left
- Look right
- Yes motion
- No motion

Required Arduino libraries:

- `Adafruit GFX Library`
- `Adafruit SSD1306`

Current OLED assumptions:

- Display size: `128x64`
- I2C address: `0x3C`
- SDA pin: `D4`
- SCL pin: `D5`

## Documentation Notes

When adding a new OLED expression, document:

- Expression name
- Intended behavior
- Trigger condition
- Required bitmap or drawing function
- Known hardware assumptions

Nota en espanol: V1 es la primera version del firmware unificado. Todos los nodos deben compartir este firmware base.
