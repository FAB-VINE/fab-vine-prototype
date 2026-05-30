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
- `tools/` - local preview utilities for testing expressions before uploading to hardware.
- `notes/` - test notes, design decisions, and expression behavior ideas.

## Arduino Sketch

The main sketch is:

```text
firmware/fab-vine-node-v1/fab-vine-node-v1.ino
```

Arduino IDE expects the `.ino` file to live inside a folder with the same name.

## Current MVP Firmware

The current V1 sketch is an offline physical MVP for the Seeed Studio XIAO ESP32-C6.

It demonstrates:

- OLED expression display on a `128x64` SSD1306 screen.
- 40 WS2812B / NeoPixel LEDs with social-state breathing patterns.
- Physical neighbor detection across six module faces.
- A local "living greeting" when one or more neighbor contacts are detected.

This MVP intentionally does not include WiFi, MQTT, or microphone behavior yet. The first goal is to prove that a node can sense a physical neighbor and react clearly without network dependencies.

## MVP Behavior

Face connector assumptions:

- `GND` - shared ground between neighboring modules.
- `DETECT` - one digital contact detect line per face.

Detection behavior:

- Each face `DETECT` uses `INPUT_PULLUP`.
- No neighbor connected: that face reads `HIGH`.
- Neighbor connected: that face is connected to `GND` and reads `LOW`.
- The firmware counts occupied faces from `0` to `6`.

Visual behavior:

- No neighbor: normal OLED eyes with a soft purple LED breath. The purple represents slight social anxiety while the module is alone, without feeling like an alarm.
- First neighbor connected: OLED performs a soft yes/nod expression and LEDs pulse brighter green/blue. The red disappears completely when the module feels socially connected.
- Still connected: calm connected idle state with occasional wink and a full-strip blue-green breathing pattern.
- Neighbor removed: OLED performs a short no/head-shake expression, the anxious purple returns briefly, and the module settles back into idle.

## Pin Defaults

All MVP pins are defined at the top of the Arduino sketch so they can be changed quickly:

- OLED SDA: `D4`
- OLED SCL: `D5`
- LED data: `D10`
- Face 1 detect: `D0`
- Face 2 detect: `D1`
- Face 3 detect: `D2`
- Face 4 detect: `D3`
- Face 5 detect: `D6`
- Face 6 detect: `D7`
- LED count: `40`

Do not carry main LED power through pogo pins. Use face connectors for detect/signal only, and keep 5V/GND LED power on suitable power wiring.

## LED Zig Zag Map

The MVP V2 assumes the LED strip is physically arranged as four lines of ten LEDs.

Physical positions repeat every ten LEDs:

- Position `1` lights LEDs `1, 11, 21, 31`.
- Position `2` lights LEDs `2, 12, 22, 32`.
- Position `10` lights LEDs `10, 20, 30, 40`.

Default face-to-position map for directional tests:

- Face 1 -> position `1`
- Face 2 -> position `3`
- Face 3 -> position `5`
- Face 4 -> position `6`
- Face 5 -> position `8`
- Face 6 -> position `10`

This map is editable at the top of the Arduino sketch through `faceLedPositions`.

## OLED Expression Set

The firmware keeps these expression drawing helpers available:

- Normal eyes
- Blink
- Wink
- Surprised
- Look left
- Look right
- Yes motion
- No motion, with pupils moving side to side

Required Arduino libraries:

- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `Adafruit NeoPixel`

Current OLED assumptions:

- Display size: `128x64`
- I2C address: `0x3C`
- SDA pin: `D4`
- SCL pin: `D5`

Upload/test assumptions:

- Board: Seeed Studio XIAO ESP32-C6
- FQBN: `esp32:esp32:XIAO_ESP32C6`
- Compile in Arduino IDE if `arduino-cli` is not installed locally.

Local preview:

```text
tools/oled-expression-preview.html
```

Open this file through a local web server to preview OLED expressions before uploading to the physical node.

## Documentation Notes

When adding a new OLED expression, document:

- Expression name
- Intended behavior
- Trigger condition
- Required bitmap or drawing function
- Known hardware assumptions

Detailed OLED expression manual:

```text
modules/oled-expressions/README.md
```

Browsable HTML manual:

```text
../../../MANUALS/oled-expressions.html
```

Nota en espanol: V1 es la primera version del firmware unificado. Todos los nodos deben compartir este firmware base.
