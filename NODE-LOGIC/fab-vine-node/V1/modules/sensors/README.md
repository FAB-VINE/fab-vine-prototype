# Sensors

Sensor logic for Fab Vine Node V1.

## Current MVP Input

The current physical MVP treats neighbor contact as the primary input. V2 supports six faces, with one digital detect input per face.

Default pins:

- Face 1 detect: `D0`
- Face 2 detect: `D1`
- Face 3 detect: `D2`
- Face 4 detect: `D3`
- Face 5 detect: `D6`
- Face 6 detect: `D7`

Detection behavior:

- Each face pin is configured as `INPUT_PULLUP`.
- No neighbor connected reads `HIGH`.
- A physical neighbor/contact should connect that face pin to `GND`, which reads `LOW`.
- The firmware counts occupied faces from `0` to `6`.

The I2S microphone remains part of the hardware direction, but audio input is intentionally out of scope for this MVP so the first test can focus on reliable physical connection.

Use this folder for:

- Motion input
- Audio input
- Environmental input
- Sensor calibration
- Input normalization
