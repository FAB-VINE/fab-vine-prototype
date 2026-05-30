# LEDs

LED behavior for Fab Vine Node V1.

## Current MVP Behavior

The first physical MVP uses `40` WS2812B / NeoPixel LEDs on `D10`.

The LED strip communicates node state locally:

- `NODE_IDLE` - soft purple anxious breathing pattern while the module is alone.
- `NEIGHBOR_CONNECTED` - brighter blue-green full-strip greeting breath with no red channel.
- `CONNECTED_IDLE` - calm secure blue-green full-strip breathing pattern with no red channel.
- `LEADER` election result - stronger blue breathing pattern with no red channel.
- `NEIGHBOR_REMOVED` - short purple transition before returning to anxious idle.

Color language:

- Purple means slight social anxiety when the module is alone.
- Green/blue means social safety when the module has a neighbor.
- Connected states should keep the red channel at `0`.

## Zig Zag Map

V2 assumes the 40 LEDs are arranged as four physical lines of ten LEDs. The firmware keeps this zig zag map available for directional tests, even though the current connected state uses a full-strip blue-green breathing pattern.

Example:

- Physical position `1` lights LEDs `1, 11, 21, 31`.
- In Arduino code, those are indexes `0, 10, 20, 30`.

Default face-to-position map:

- Face 1 -> position `1`
- Face 2 -> position `3`
- Face 3 -> position `5`
- Face 4 -> position `6`
- Face 5 -> position `8`
- Face 6 -> position `10`

Edit `faceLedPositions` in the main sketch if the physical layout changes.

Required Arduino library:

- `Adafruit NeoPixel`

Main LED power should not run through the magnetic pogo connector. Use suitable 5V/GND wiring for LED power, and keep pogo pins for detect/signal behavior.

Use this folder for:

- Status light behavior
- Addressable LED patterns
- Light reactions tied to sensor or network state
- Shared visual feedback functions
