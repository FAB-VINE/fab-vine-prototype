# V3.6 Direction Calibration

Calibration program for identifying the four ten-pixel groups after the strip is installed on the twisted cylindrical body. One group is shown at a time, and a bright marker travels from its bottom pixel to its top pixel.

## Confirmed physical color map

| Group | Color | Physical face |
|---|---|---|
| 1, pixels 0-9 | Red | Front |
| 2, pixels 10-19 | Green | Right |
| 3, pixels 20-29 | Blue | Back / posterior |
| 4, pixels 30-39 | Purple | Left |

Each group remains ordered from bottom to top. The colors are identifiers for physical orientation, not connection states.

## Calibration procedure

1. Place the module upright and choose the OLED/display side as the front reference.
2. Open and upload `V3_6_Direction_Calibration.ino` to a `XIAO ESP32C6`.
3. Watch one complete cycle: red, green, blue, then purple.
4. Confirm that the bright marker in every color moves from bottom to top.
5. Confirm that the illuminated paths correspond to front, right, back, and left using the table above.
6. If a future body is assembled differently, update the group-to-face mapping before using Neighbor Compass.

The strip data input is `D10`. This program is intended for assembly checks and should not be mistaken for the final social animation.
