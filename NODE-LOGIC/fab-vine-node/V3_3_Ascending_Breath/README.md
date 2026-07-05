# V3.3 Ascending Breath

First independent animation for the 40-pixel body strip. The strip is treated as four physical groups of ten pixels. In every group, pixel order runs from bottom to top.

The same soft light front rises through all four groups while the color blends continuously through purple, blue, and green. Use this version to confirm the four-group wiring and compare a vertical animation against the earlier all-pixels-at-once breathing effect.

## Use

1. Open `V3_3_Ascending_Breath.ino` in Arduino IDE.
2. Select `XIAO ESP32C6` and the connected serial port.
3. Install the Adafruit GFX, NeoPixel, NeoMatrix, and SSD1306 libraries if needed.
4. Upload the sketch. The 40-pixel strip data input must be connected to `D10`.

This version retains the OLED, 8x8 matrix, and six-face communication inherited from V3.1; only the 40-pixel strip animation is changed.
