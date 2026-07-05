# V3.7 Neighbor Compass

Turns the calibrated 40-pixel body strip into a connection guide. It cycles only through faces that do not currently report a neighbor. Once a face recognizes a neighbor, that direction is removed from the cycle. When all six faces are connected, the module returns to organic breathing.

## Direction language

| Direction | LED behavior |
|---|---|
| Front | Red group moves upward |
| Right | Green group moves upward |
| Back / posterior | Blue group moves upward |
| Left | Purple group moves upward |
| Top | White light moves from bottom to top through all four groups |
| Bottom | White light moves from top to bottom through all four groups |

The lateral colors come directly from V3.6 Direction Calibration: red is front, green is right, blue is back, and purple is left.

## Use

1. Verify the physical orientation with V3.6 before using this program on a newly assembled module.
2. Open and upload `V3_7_Neighbor_Compass.ino` to a `XIAO ESP32C6`.
3. Leave the module disconnected and verify that all six directions appear.
4. Connect one face and wait for the neighbor timeout to update.
5. Confirm that the connected face disappears from the direction cycle.
6. Disconnect it and confirm that its direction returns after the timeout.

This version uses the six-face packet and timeout logic inherited from V3. The LED guide is diagnostic: it shows which faces remain available, not a mandatory assembly order.
